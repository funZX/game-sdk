/*
*
*    MIT License
*
*    SIM - OpenGL ES 2.0 portable game engine
*    Copyright (C) 2012  Adrian SIMINCIUC
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*/

#include <sim_engine.h>

#include <core/sys/sim_thread.h>
#include <core/sim_state_machine.h>
#include <core/io/sim_mem_stream.h>

#include <vm/sim_squirrel.h>

#include <render/scene/sim_camera.h>

#include <render/sim_material.h>
#include <render/sim_effect.h>
#include <render/sim_render_texture.h>
#include <render/sim_driver.h>
#include <render/sim_canvas.h>
#include <render/sim_font.h>
#include <render/sim_font_atlas.h>

#include <imgui.h>

namespace sim
{

// ----------------------------------------------------------------------//

CEngine::CEngine( IState* initState )
{
	m_driver			= SIM_NEW CDriver();
	m_vm				= SIM_NEW CSquirrel();

    InitOpenGL();
    InitOpenAL();

    m_effect            = SIM_NEW CEffect("engine.Effect");
    m_material          = SIM_NEW CMaterial("engine.Material");
	m_texture			= SIM_NEW CTexture("engine.Texture");
    InitEffect();
	InitTexture();
    InitMaterial();

    m_fontAtlas         = SIM_NEW CFontAtlas( "engine.Atlas" );
    m_font              = SIM_NEW CFont( m_fontAtlas, "engine.Font" );

    InitFont();

	m_canvas			= SIM_NEW CCanvas( "engine.Canvas", m_fontAtlas );
    InitCanvas();

    m_sm                = SIM_NEW CStateMachine( m_canvas, initState );
	m_camera			= SIM_NEW ren::CCamera( "engine.Camera" );
    m_crtCamera         = m_camera;

	m_currentTime		=  0;
	m_updateTime		= -1;
	m_frameTime			=  0;
	m_deltaTime			=  0.00f;

	m_drawCount			= 0;
	m_vertexCount		= 0;
	m_fps				= 0;
	SIM_MEMSET( &m_dtfilter[ 0 ], 0, sizeof( m_dtfilter ) );
}

// ----------------------------------------------------------------------//

CEngine::~CEngine()
{
    Shutdown();
    
	SIM_SAFE_DELETE( m_camera );
    SIM_SAFE_DELETE( m_sm );
    SIM_SAFE_DELETE( m_canvas );
	SIM_SAFE_DELETE( m_effect );
	SIM_SAFE_DELETE( m_material );
    SIM_SAFE_DELETE( m_font );
    SIM_SAFE_DELETE( m_fontAtlas );

    SIM_SAFE_DELETE( m_vm );
	SIM_SAFE_DELETE( m_driver );
}

// ----------------------------------------------------------------------//

void CEngine::Shutdown()
{
}

// ----------------------------------------------------------------------//
void CEngine::InitEffect()
{
    static const s8* vsource =
        "precision highp float;"

        "attribute vec3 a_WorldPosL;"
        "attribute vec2 a_TexCoord_0;"

        "uniform mat4 u_Matrix_WorldViewProjection;"
        "uniform vec4 u_Color;"

        "varying vec2 v_Tex0;"
        "varying vec4 v_Color;"

        "void main()"
        "{"
        "	v_Tex0			= a_TexCoord_0;"
        "	v_Color			= u_Color;"

        "	gl_Position		= u_Matrix_WorldViewProjection * vec4(a_WorldPosL, 1.0);"
        "}";

	// ----------------------------------------------------------------------//

    static const s8* psource =
        "precision mediump float;"

        "uniform lowp sampler2D	u_Sampler_Tex_0;"

        "varying vec2 v_Tex0;"
        "varying vec4 v_Color;"

        "void main()"
        "{"
        "	vec4 tex = texture2D( u_Sampler_Tex_0, v_Tex0 );"
        "	vec4 col = tex * v_Color;"

        "	gl_FragColor = col;"
        "}";

    static const s8* attributes[] =
    {
        "a_WorldPosL",
        "a_TexCoord_0",
    };

    u32 nAttrib = 2;
    for (u32 k = 0; k < nAttrib; k++)
        m_effect->AddAttribute( attributes[k] );

    static const s8* uniforms[] =
    {
        "u_Matrix_WorldViewProjection",
        "u_Color",
        "u_Sampler_Tex_0"
    };

    u32 nUniform = 3;
    m_effect->InitUniforms( nUniform );
    for (u32 k = 0; k < nUniform; k++)
        m_effect->AddUniform(uniforms[k], k);

	m_effect->Load( vsource, psource );

	m_effect->m_technique.depthtest = true;
	m_effect->m_technique.depthmask = true;
	m_effect->m_technique.cullface  = true;
	m_effect->m_technique.alphatest = false;

	m_effect->m_technique.blending = false;
	m_effect->m_technique.blendfunc.equation = GL_FUNC_ADD;
	m_effect->m_technique.blendfunc.src = GL_SRC_ALPHA;
	m_effect->m_technique.blendfunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    m_effect->m_technique.blendColor = col::White;

	m_effect->m_technique.depthfunc.equation = GL_LESS;
}

// ----------------------------------------------------------------------//

void CEngine::InitTexture()
{
    s32 texWidth	= 32;
    s32 texHeight	= 32;
    s32 texMemSize  = texWidth * texHeight * 3;
    u8* texBuf		= SIM_NEW u8[texMemSize];

	SIM_MEMSET( texBuf, 255, texMemSize);

    u32 tex = m_driver->BindTexture(CDriver::TextureTarget::Texture2D, 0);
    m_texture->Generate(texBuf
        , texWidth
        , texHeight
        , CTexture::Type::MIP
        , CTexture::Wrap::Clamp
        , CTexture::Filter::Nearest
        , CTexture::Format::RGBA4
    );
	m_driver->BindTexture(CDriver::TextureTarget::Texture2D, tex);

	SIM_SAFE_DELETE_ARRAY( texBuf );
}

// ----------------------------------------------------------------------//

void CEngine::InitMaterial()
{
	m_material->SetTexture(m_texture, 0);
	m_material->SetEffect(m_effect);
}

// ----------------------------------------------------------------------//
void CEngine::InitFont()
{
	const unsigned char buffer[] =
	#include "engine_font.inl"

    io::CMemStream ms( (void*)&buffer[ 0 ], sizeof(buffer) );

    m_fontAtlas->AddFont( "engine.Font", &ms, 15.0f );
    m_fontAtlas->Create();
}

// ----------------------------------------------------------------------//
void CEngine::InitCanvas()
{
    m_canvas->OnGui.Connect( this, &CEngine::OnGui );
}
// ----------------------------------------------------------------------//

void CEngine::InitOpenGL( void )
{
	m_driver->Initialize();
}

// ----------------------------------------------------------------------//

void CEngine::InitOpenAL()
{
	m_ALDevice = alcOpenDevice( nullptr );

	if( m_ALDevice != nullptr )
	{	
		m_ALContext = alcCreateContext( m_ALDevice, nullptr );
		alcMakeContextCurrent( m_ALContext );
	}
}

// ----------------------------------------------------------------------//

void CEngine::Resize( u32 width, u32 height )
{
    m_driver->SetScreenSize( width, height );
    m_canvas->Resize( (f32)width, (f32)height );

    m_crtCamera->SetOrthographic( m_canvas );
    m_crtCamera->SetPerspective( m_canvas );
}

// ----------------------------------------------------------------------//

void CEngine::Start()
{
    
}

// ----------------------------------------------------------------------//

f32 CEngine::Smooth( f32 deltaTime )
{
	f32 sum		= 0.0f;
	f32 min1, min2, max1, max2;
	f32 dt		= 0.0f;

    min1 = min2 = m_dtfilter[0];
    max1 = max2 = 0.0f;

	for( s32 k = 0; k < 11; k++ )
	{
		dt = m_dtfilter[ k ];

		if ( dt < min1 && dt < min2 )
			min1 = dt;
		else if (dt < min2)
			min2 = dt;

		if ( dt > max1 && dt > max2 )
			max1 = dt;
		else if ( dt > max2 )
			max2 = dt;

		sum += dt;

        SIM_ASSERT(min1 >= 0.0f);
        SIM_ASSERT(min2 >= 0.0f);
        SIM_ASSERT(max1 >= 0.0f);
        SIM_ASSERT(max2 >= 0.0f);
	}
    
	sum -= ( min1 + min2 + max1 + max2 );
	sum *= 0.1428571f;
    SIM_ASSERT(sum >= 0.0f);

	dt	 = zpl_lerp( deltaTime, sum, 0.5f );
    SIM_ASSERT( dt >= 0.0f );

	for( s32 k = 0; k < 10; k++ )
		m_dtfilter[ k ] = m_dtfilter[ k + 1 ];

    m_dtfilter[ 10 ] = dt;

	return dt;
}

// ----------------------------------------------------------------------//

void CEngine::Run()
{
	//SIM_CHECK_OPENGL();

	u64 begin		= ::GetTimeMicro();

	m_currentTime	= begin;
	m_frameTime		= m_currentTime - m_updateTime;
	m_updateTime	= m_currentTime;
	m_deltaTime		= Smooth((f32) m_frameTime) / 1000000.0f;

	Update( m_deltaTime, this );
	Render( m_driver );

	u64 end			= ::GetTimeMicro();
	u64 minDtMicro	= MinDt * 1000;
	if( m_frameTime > minDtMicro )
		end += m_frameTime - minDtMicro;

	s64 dtMicro		= end - begin;
	s64 wtMicro		= min( minDtMicro, max( 1, minDtMicro - dtMicro ) );
	
	CThread::Wait( wtMicro );
}

// ----------------------------------------------------------------------//

void CEngine::Quit()
{
	alcMakeContextCurrent( nullptr );
	
	if( m_ALContext != nullptr )
	{
		alcDestroyContext( m_ALContext );
		
		alcCloseDevice( m_ALDevice );
	}
}

// ----------------------------------------------------------------------//

void CEngine::Update( f32 dt, void *userData )
{
    m_sm->Update(dt, userData);
    m_canvas->Update(dt, userData);

    m_camera->Update(dt, userData);
    m_driver->Tick(dt);
}

// ----------------------------------------------------------------------//

void CEngine::Render( CDriver *driver )
{
	driver->Clear( col::Blueish );

	// 3D rendering
	On3D();
	{
		m_sm->Render( driver );
	}
	Off3D();

	// 2D rendering
	On2D();
	{
        m_canvas->Render( driver );
    }
	Off2D();
}

// ----------------------------------------------------------------------//

void CEngine::GoNext( IState* state )
{
	m_canvas->Reset();
	
	m_sm->GoNext( state );
}

// ----------------------------------------------------------------------//

void CEngine::GoPop(IState* state)
{
    m_canvas->Reset();

    m_sm->GoPop( state );
}

// ----------------------------------------------------------------------//

void CEngine::GoBack()
{
	m_canvas->Reset();

	m_sm->GoBack();
}

// ----------------------------------------------------------------------//

CCamera* CEngine::GetCamera()
{
    return m_crtCamera;
}

// ----------------------------------------------------------------------//

void CEngine::SetCamera( CCamera* camera )
{
    m_crtCamera = camera;
}

// ----------------------------------------------------------------------//

void CEngine::On2D()
{
	m_driver->SetMatrixMode( CDriver::MatrixMode::Projection );
	m_driver->MatrixLoad( m_crtCamera->GetOrthographicMatrix() );

	m_driver->SetMatrixMode( CDriver::MatrixMode::View );
	m_driver->MatrixLoadIdentity();

	m_driver->SetMatrixMode( CDriver::MatrixMode::World );
	m_driver->MatrixLoadIdentity();
}

// ----------------------------------------------------------------------//

void CEngine::Off2D()
{

}

// ----------------------------------------------------------------------//

void CEngine::On3D()
{
	m_driver->SetMatrixMode( CDriver::MatrixMode::Projection );
	m_driver->MatrixLoad( m_crtCamera->GetPerspectiveMatrix() );

	m_driver->SetMatrixMode( CDriver::MatrixMode::View );
	m_driver->MatrixLoad( m_crtCamera->GetMatrix() );

	m_driver->SetMatrixMode( CDriver::MatrixMode::World );
	m_driver->MatrixLoadIdentity();

    m_crtCamera->Render( m_driver );
}

// ----------------------------------------------------------------------//

void CEngine::Off3D()
{

}

// ----------------------------------------------------------------------//

void CEngine::OnGui( CCanvas* canvas, sigcxx::SLOT slot )
{
    static u64 lastTime = GetTime();
    static u64 frameSum = 0;
    static u32 frameCount = 0;
    static s32 prevDrawCount = 0;
    static s32 prevVrtxCount = 0;

    // fps
    u64 currentTime = GetTime();
    u64 frameTime   = GetFrameTime();

    if (currentTime - lastTime >= 1000000)
    {
        f32 avg = (f32)frameSum / (f32)frameCount;

        m_fps = 1000000.0f / avg;
        lastTime = currentTime;

        frameCount = 0;
        frameSum = 0;
    }

    ++frameCount;
    frameSum += frameTime;

    // batch2D
    u32 drawCount = m_driver->GetDrawCallCount();
    u32 vrtxCount = m_driver->GetVertexCount();

    m_drawCount   = drawCount - prevDrawCount;
    m_vertexCount = vrtxCount - prevVrtxCount;

    prevDrawCount = drawCount;
    prevVrtxCount = vrtxCount;

    ImGui::SetNextWindowSize({ 120, 110 });
    if (ImGui::Begin("engine.Stats", 0,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar))
    {
		ImGui::TextWrapped("Draw calls: %d", m_drawCount);
        ImGui::TextWrapped("Vertices: %d", m_vertexCount);
		ImGui::TextWrapped("Triangles: %d", m_vertexCount / 3);
        ImGui::TextWrapped("FPS: %.1f", m_fps);
    }
    ImGui::End();
}
// ----------------------------------------------------------------------//
void CEngine::MouseDown(int button)
{
    m_canvas->MouseDown( button );
}
// ----------------------------------------------------------------------//
void CEngine::MouseUp(int button)
{
    m_canvas->MouseUp( button );
}
// ----------------------------------------------------------------------//
void CEngine::MouseMove(f32 x, f32 y)
{
    m_canvas->MouseMove( x, y );
}
// ----------------------------------------------------------------------//
void CEngine::KeyDown(int Key, bool KeyShift, bool KeyCtrl, bool KeyAlt)
{
    m_canvas->KeyDown( Key, KeyShift, KeyCtrl, KeyAlt );
}
// ----------------------------------------------------------------------//
void CEngine::KeyUp(int Key, bool KeyShift, bool KeyCtrl, bool KeyAlt)
{
    m_canvas->KeyUp(Key, KeyShift, KeyCtrl, KeyAlt);
}
// ----------------------------------------------------------------------//
}; // namespace sim
