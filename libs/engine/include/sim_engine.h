/* 
*    SIM - OpenGL ES 2.0 portable game engine
*    Copyright (C) 2012  Adrian SIMINCIUC
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
		union TMatrix3;
		union TMatrix4;

		union TVec2;
		union TVec3;
		union TVec4;
		union TQuat;
		struct TMatrix4Stack;
		union TPlane;
	};
//------------------------------------------------
	namespace rnr
	{
		class CDriver;
		class CBatch2D;
		class CFont;
		class CFontAtlas;

		class CTexture;
		class CSvgTexture;
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

		class CRect2D;
		class CCanvas;

		class CWidget;
		class CWidgetLabel;
		class CWidgetButton;
		class CWidgetSvg;
		class CWidgetDrawable;
		class CWidgetSprite;

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
#include <core/sim_pool.h>
#include <core/sim_interfaces.h>
#include <core/sim_list.h>
#include <core/sim_binary_tree.h>
#include <core/sim_balance_tree.h>
#include <core/sim_shellsort.h>
#include <core/sim_singleton.h>
#include <core/sim_stack.h>

#include <math/sim_math.h>

#include <render/sim_render.h>
#include <render/sim_color.h>
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
using namespace sim::mat;

namespace sim
{
// ----------------------------------------------------------------------//
class CEngine : public sim::IUpdatable, public sim::IRenderable, public CSingleton<CEngine>
{
public:
	CEngine();
	virtual ~CEngine();
	
	inline CDriver*					GetDriver()		{ return m_driver; }
	inline CSquirrel*				GetVM()			{ return m_vm; }
	// ------------------------------------------------------------------//

	virtual void					Start( int width, int height );
	virtual void					Run( void );
	virtual void					Quit( void );

	u64								GetTime()			{ return m_currentTime; }
	u64								GetFrameTime()		{ return m_frameTime; }
	f32								GetDeltaTime()		{ return m_deltaTime; }

	virtual void					Update( f32 dt, void *userData );
	virtual void					Render( CDriver *driver );

	void							Print( CDriver* driver, s32 x, s32 y, const std::string& text );
	void							Print( CDriver* driver, s32 x, s32 y, char *format, ... );

	void							GoNext( IState* state );
	void							GoBack();

	void							SetCamera( CCamera *camera );

	void							PointerDown( u32 x, u32 y );
	void							PointerDrag( u32 x, u32 y );
	void							PointerUp( u32 x, u32 y );


protected:
	void							Initialize();
	void							InitOpenGL();
	void							InitOpenAL();
	
	void							On2D();
	void							Off2D();
	void							On3D();
	void							Off3D();

	void							ShowStats( CDriver* driver );
	f32								Smooth( f32 dt );

	void							InitFont();
	void							InitEffect();
	void							InitMaterial();
	void							InitVirtualMachine();

protected:

	static const u32				MaxFps	= 60;
	static const u32				MinDt	= 1000 / MaxFps;

	// ------------------------------------------------------------------//
	rnr::CDriver*					m_driver;
	vm::CSquirrel*					m_vm;
	sm::CStateMachine*				m_sm;

	rnr::CCamera*					m_activeCamera;
	rnr::CCamera*					m_camera;
	rnr::CMaterial*					m_material;
	rnr::CEffect*					m_effect;
	rnr::CCanvas*					m_canvas;

	rnr::CFontAtlas*				m_fontAtlas;
	rnr::CFont*						m_font;

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
