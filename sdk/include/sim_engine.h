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

#ifndef __SIM_ENGINE_H
#define __SIM_ENGINE_H

//////////////////////////////////////////////////
namespace sim
{
//------------------------------------------------
	namespace io
	{
		class CFileStream;
		class CMemStream;
		class CJsonStream;
		class CFileSystem;
	};
//------------------------------------------------
	namespace sys
	{
		class CMutex;
		class CThread;
	};
//------------------------------------------------
	namespace sm
	{
		class CStateMachine;
	};
//------------------------------------------------
	namespace mat
	{
		union Mat3;
		union Mat4;

		union Vec2;
		union Vec3;
		union Vec4;
		union Quat;
		struct Mat4Stack;
		union Plane;
	};
//------------------------------------------------
	namespace rnr
	{
		class CDriver;

		class CTexture;
		class CRenderTexture;
		class CSpriteTexture;

		class CMesh;
		class CVertexGroup;
		class CVertexSource;
		class Material;
		class CEffect;
		class CShader;
		class CSkyBox;

		class CScene;
		class CSceneNode;
		class CCamera;
		class CLight;
		class CActor;

		class CBone;
		class CBoneMask;
		class CBoneHierarchy;
		class CAnimationPose;
		class CAnimationFrame;
		class CBoneAnimation;
		class CAnimationBlend;
		class CAnimationPlayer;

        class CFontAtlas;
        class CFont;
        class CRect2D;
        class CDrawable;
		class CCanvas;		

		struct TColor;
	};
//------------------------------------------------
	namespace phy
	{
		class CPhysic;
	};
//------------------------------------------------
	namespace snd
	{
		class CSoundData;
		class CSoundSource;
	};
//------------------------------------------------
	namespace net
	{
		
	};
//------------------------------------------------
	namespace vm
	{
		class CScript;
		class CSquirrel;
	};

//------------------------------------------------
};
//////////////////////////////////////////////////
#include <core/sim_core.h>
#include <core/sim_interfaces.h>
#include <core/sim_list.h>
#include <core/sim_binary_tree.h>
#include <core/sim_balance_tree.h>
#include <core/sim_shellsort.h>
#include <core/sim_singleton.h>
#include <core/sim_stack.h>

#include <render/sim_render.h>
#include <render/sim_glaux.h>

#include <sound/sim_sound.h>

using namespace sim;
using namespace sim::stl;
using namespace sim::io;
using namespace sim::sys;
using namespace sim::sm;
using namespace sim::rnr;
using namespace sim::phy;
using namespace sim::snd;
using namespace sim::vm;

namespace sim
{
// ----------------------------------------------------------------------//
class CEngine : public sim::IUpdatable, public sim::IRenderable, public CSingleton<CEngine>, public sigcxx::Trackable
{
public:
	CEngine();
	virtual ~CEngine();
	
	inline CDriver*					GetDriver()		{ return m_driver; }
	inline CSquirrel*				GetVM()			{ return m_vm; }

    virtual void					Resize( u32 width, u32 height );
	virtual void					Start();
	virtual void					Run();
	virtual void					Quit();

	u64								GetTime()			{ return m_currentTime; }
	u64								GetFrameTime()		{ return m_frameTime; }
	f32								GetDeltaTime()		{ return m_deltaTime; }

	virtual void					Update( f32 dt, void *userData );
	virtual void					Render( CDriver *driver );

	void							GoNext( IState* state );
	void							GoBack();

	void							SetCamera( CCamera *camera );
    CCamera*                        GetCamera();

	void							PointerDown( u32 x, u32 y );
	void							PointerDrag( u32 x, u32 y );
	void							PointerUp( u32 x, u32 y );


protected:
	void							InitOpenGL();
	void							InitOpenAL();
    void							InitFont();
    void							InitEffect();
	void							InitTexture();
    void							InitMaterial();
    void							InitCanvas();
    void							Shutdown();
	
	void							On2D();
	void							Off2D();
	void							On3D();
	void							Off3D();

	void							OnGui( CCanvas* canvas, sigcxx::SLOT slot = nullptr );
	f32								Smooth( f32 dt );

protected:

	static const u32				MaxFps	= 60;
	static const u32				MinDt	= 1000 / MaxFps;

	// ------------------------------------------------------------------//
	rnr::CDriver*					m_driver;
	vm::CSquirrel*					m_vm;
	sm::CStateMachine*				m_sm;

	rnr::CCamera*					m_activeCamera;
	rnr::CCamera*					m_camera;
	rnr::CTexture*					m_texture;
	rnr::CMaterial*					m_material;
	rnr::CEffect*					m_effect;
	rnr::CCanvas*					m_canvas;
    rnr::CFont*                     m_font;
    rnr::CFontAtlas*                m_fontAtlas;

	ALCdevice*						m_ALDevice;
	ALCcontext*						m_ALContext;

	u64								m_currentTime;
	u64								m_updateTime;
	u64								m_frameTime;
	f32								m_deltaTime;

	u32								m_drawCount;
	u32								m_vertexCount;
	f32								m_fps;
	f32								m_dtfilter[11];
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace sim

#endif // __SIM_ENGINE_H
