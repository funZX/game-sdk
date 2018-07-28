#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "SimulatorWindow.h"
// ----------------------------------------------------------------------//
void SaveWindowPosition();
void LoadWindowPosition();

static TSimulator SIM;
// ----------------------------------------------------------------------//
LRESULT WINAPI SimulatorWindowProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
	TSimulator *sim = (TSimulator*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

	LRESULT  lRet = 1; 

	switch (uMsg) 
	{ 
	case WM_CREATE:
		LoadWindowPosition();
		break;

	case WM_CLOSE:
	case WM_DESTROY:
		SaveWindowPosition();
		sim->active = false;          
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			if( sim->OnKey )
				sim->OnKey( (unsigned char) wParam, uMsg == WM_KEYDOWN );
		}
		break;

	case WM_MOUSEMOVE:
		{
			POINT		point;
			CGPoint		p;

			GetCursorPos( &point );
			ScreenToClient( hWnd, &point );

			p.x = (float) point.x;
			p.y = (float) point.y;

			if ( sim && sim->touchBegan && sim->OnTouchMove )
				sim->OnTouchMove( &p );
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		{
			POINT      point;

			GetCursorPos( &point );
			ScreenToClient( hWnd, &point );

			if( uMsg == WM_LBUTTONDOWN )
			{
				if ( sim->OnTouchBegan )
				{
					CGPoint p;

					p.x = (float) point.x;
					p.y = (float) point.y;

					sim->touchBegan = true;
					sim->OnTouchBegan( &p, 1 );
				}
			}
			else if( uMsg == WM_LBUTTONUP )
			{
				if ( sim->OnTouchEnd )
				{
					CGPoint p;

					p.x = (float) point.x;
					p.y = (float) point.y;

					sim->touchBegan = false;
					sim->OnTouchEnd( &p );
				}
			}
		}
		break;



	default: 
		lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
		break; 
	} 

	return lRet; 
}
// ----------------------------------------------------------------------//
GLboolean SimulatorWinCreate ( TSimulator *sim, const char *title )
{
	WNDCLASS wndclass = {0}; 
	DWORD    wStyle   = 0;
	RECT     windowRect;
	HINSTANCE hInstance = GetModuleHandle(NULL);


	wndclass.style         = CS_OWNDC;
	wndclass.lpfnWndProc   = (WNDPROC)SimulatorWindowProc; 
	wndclass.hInstance     = hInstance; 
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
	wndclass.lpszClassName = WINDOW_CLASS; 

	if (!RegisterClass (&wndclass) ) 
		return FALSE; 

	wStyle = WS_POPUPWINDOW | WS_CAPTION;

	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = sim->width;
	windowRect.bottom = sim->height;

	AdjustWindowRect ( &windowRect, wStyle, FALSE );


	sim->touchBegan = false;
	sim->hWnd = CreateWindow(
		WINDOW_CLASS,
		title,
		wStyle,
		0,
		0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	SetWindowLongPtr (  sim->hWnd, GWL_USERDATA, (LONG) (LONG_PTR) sim );


	if ( sim->hWnd == NULL )
		return GL_FALSE;

	ShowWindow ( sim->hWnd, TRUE );

	return GL_TRUE;
}
// ----------------------------------------------------------------------//
EGLBoolean SimulatorCreateEGLContext ( EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
	EGLContext* eglContext, EGLSurface* eglSurface,
	EGLint attribList[])
{
	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

	display = eglGetDisplay(GetDC(hWnd));
	if ( display == EGL_NO_DISPLAY )
		return EGL_FALSE;

	if ( !eglInitialize(display, &majorVersion, &minorVersion) )
		return EGL_FALSE;

	if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
		return EGL_FALSE;

	if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) )
		return EGL_FALSE;

	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
	if ( surface == EGL_NO_SURFACE )
		return EGL_FALSE;

	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );

	if ( context == EGL_NO_CONTEXT )
		return EGL_FALSE; 

	if ( !eglMakeCurrent(display, surface, surface, context) )
		return EGL_FALSE;

	*eglDisplay = display;
	*eglSurface = surface;
	*eglContext = context;

	return EGL_TRUE;
} 
// ----------------------------------------------------------------------//
GLboolean  SimulatorCreateWindow ( TSimulator *sim, const char* title, GLint width, GLint height )
{
	EGLint attribList[] =
	{
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 6,
		EGL_BLUE_SIZE, 5,
		EGL_DEPTH_SIZE, 16,
		EGL_STENCIL_SIZE, 8,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SAMPLE_BUFFERS, 0,
		EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
		EGL_NONE
	};

	sim->width = width;
	sim->height = height;

	if ( !SimulatorWinCreate ( sim, title) )
		return GL_FALSE;


	if ( !SimulatorCreateEGLContext ( sim->hWnd,
		&sim->eglDisplay,
		&sim->eglContext,
		&sim->eglSurface,
		attribList) ) {
		return GL_FALSE;
	}

	return GL_TRUE;
}
// ----------------------------------------------------------------------//
void SimulatorWinLoop ( TSimulator *sim )
{
	MSG msg = { 0 };

	while( SIM.active )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}

		if ( sim->OnRender != NULL )
			sim->OnRender();
		
		ValidateRect( sim->hWnd, NULL );

		eglSwapBuffers ( sim->eglDisplay, sim->eglSurface );
	}

	if ( sim->OnQuit != NULL )
		sim->OnQuit();

	DestroyWindow( sim->hWnd );

	PostQuitMessage(0);   
}
// ----------------------------------------------------------------------//
void SimulatorSetRenderCallback( OnRenderCallback *OnRender )
{
	SIM.OnRender = OnRender;
}
// ----------------------------------------------------------------------//
void SimulatorSetResizeFromLayerCallback( OnResizeCallback *OnResize )
{
	SIM.OnResize = OnResize;
}
// ----------------------------------------------------------------------//
void SimulatorSetTouchesBeganCallback( OnTouchBeganCallback *OnTouchBegan )
{
	SIM.OnTouchBegan = OnTouchBegan;
}
// ----------------------------------------------------------------------//
void SimulatorSetTouchesMovedCallback( OnTouchMoveCallback *OnTouchMove )
{
	SIM.OnTouchMove = OnTouchMove;
}
// ----------------------------------------------------------------------//
void SimulatorSetTouchesEndedCallback( OnTouchEndCallback *OnTouchEnd )
{
	SIM.OnTouchEnd = OnTouchEnd;
}
// ----------------------------------------------------------------------//
void SimulatorSetKeyCallback( OnKeyCallback *OnKey )
{
	SIM.OnKey = OnKey;
}
// ----------------------------------------------------------------------//
void SimulatorSetQuitCallback( OnQuitCallback *OnQuit )
{
	SIM.OnQuit = OnQuit;
}
// ----------------------------------------------------------------------//
void SimulatorRun( int width, int height )
{
	SimulatorCreateWindow ( &SIM, 
		WINDOW_CLASS, 
		width, 
		height );

	if( SIM.OnResize )
		SIM.OnResize( width, height );

	SIM.active = true;

	SimulatorWinLoop ( &SIM );
}
// ----------------------------------------------------------------------//
void SaveWindowPosition()
{

}
// ----------------------------------------------------------------------//
void LoadWindowPosition()
{

}
// ----------------------------------------------------------------------//