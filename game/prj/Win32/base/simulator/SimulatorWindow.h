#ifndef __SIMULATOR_WINDOW_H
#define __SIMULATOR_WINDOW_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>

#include "../simulator.h"

#define	WINDOW_CLASS "IPhone / IPad for Windows (c) Adrian SIMINCIUC 2010"

typedef struct
{
	GLint					active;

	GLint					width;
	GLint					height;

	EGLNativeWindowType		hWnd;
	EGLDisplay				eglDisplay;
	EGLContext				eglContext;
	EGLSurface				eglSurface;

	OnTouchBeganCallback	*OnTouchBegan;
	OnTouchMoveCallback		*OnTouchMove;
	OnTouchEndCallback		*OnTouchEnd;
	OnKeyCallback			*OnKey;
	OnRenderCallback		*OnRender;
	OnResizeCallback		*OnResize;

	OnQuitCallback			*OnQuit;

} TSimulator;

#endif