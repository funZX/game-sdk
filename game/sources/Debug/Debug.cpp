#include <GLES2/gl2.h>

#include <core/io/sim_json_stream.h>

#include <core/io/sim_file_system.h>

#include <render/scene/sim_light.h>
#include <render/scene/sim_camera.h>

#include <render/sim_canvas.h>
#include <render/sim_render_texture.h>
#include <render/sim_texture.h>
#include <render/sim_material.h>
#include <render/sim_shader.h>
#include <render/sim_effect.h>
#include <render/sim_glaux.h>
#include <render/sim_mesh.h>

#include <math/sim_math.h>

#include "../Application.h"
#include "../Game.h"

#include "Debug.h"

CDebug::CDebug( CFileSystem* fs )
	: m_debugMode(0)
{

	m_debugSphere	= gluNewSphere( 32, 1.0f );
	m_debugCube		= gluNewCube( 1.0f );

	m_debugLight	= SIM_NEW CLight( "debug_light" );

	m_fs			= fs;

	m_framebuffer	= SIM_NEW CRenderTexture("Debug_FrameBuffer");
	m_framebuffer->Generate( 400, 300 );
}

CDebug::~CDebug()
{
	SIM_SAFE_DELETE( m_framebuffer );
	SIM_SAFE_DELETE( m_debugLight );

	m_debugSphere	= gluDelSphere( m_debugSphere );
	m_debugCube		= gluDelCube( m_debugCube );
}

void CDebug::Render( CDriver *driver )
{
	static CMesh* mesh = m_fs->GetMesh( "melonman/melonman" );
	static CLight light("static_light");

	TVec3 pos;
	Vec3Set(&pos, 0.0f, 2.0f, -0.f);
	light.SetPosition(&pos);

	f32 dr = driver->GetTimerRot();

	driver->MatrixPush();
	{
		driver->MatrixTranslateZ(-4.0f);
		driver->MatrixTranslateX(0.3f);
		mesh->Render(driver);
	}
	driver->MatrixPop();


	CRenderTexture* fb = 0;

	CCamera cam("");
	CRect2D r;

	r.Bound(0, 0, m_framebuffer->GetWidth(), m_framebuffer->GetHeight());
	cam.SetPerspective(&r);

	O.game->SetCamera(&cam);
	{
		driver->MatrixTranslateY(-0.5f);
		driver->MatrixTranslateZ(-2.0f);
		driver->MatrixRotateY(dr * 50.0f);
		
		fb = 
		driver->BindRenderTexture(m_framebuffer);
		mesh->Render(driver);
		driver->BindRenderTexture(fb);
	}
	O.game->SetCamera(0);
}

void CDebug::Render2D(CDriver *driver)
{
	static CEffect* effect = m_fs->GetEffect("color/fill_color_texture_color");
	static CRect2D texRect(0, 0, 1, 1);

	CMaterial m("");
	CRect2D r("");

	m.SetEffect(effect);
	m.SetTexture(m_framebuffer, 0);

	r.Bound(10.0f, 30.0f, 150.0f, 150.0f);
	r.Flip();
	r.SetMaterial(&m);

	CDriver::K_SELECT_BATCH batchSelect =
		driver->SelectBatch(CDriver::k_Select_Batch_None);

	r.Render( driver, &texRect );

	driver->SelectBatch(batchSelect);
}

 void CDebug::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
 {
 
 }

 void CDebug::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
 {
 
 }

 void CDebug::drawSphere (const btVector3& p, btScalar radius, const btVector3& color)
 {
	 CDriver* driver = O.driver;
	 driver->MatrixPush();
	 driver->MatrixLoadIdentity();
	 driver->MatrixTranslate( p.getX(), p.getY(), p.getZ() );
	 driver->MatrixScale( radius, radius, radius );
	 gluRenderSphere(driver, m_debugSphere);
	 driver->MatrixPop();
 }

 void CDebug::drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha)
 {
 
 }

 void CDebug::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
 {
 
 }

 void CDebug::draw3dText(const btVector3& location, const char* textString)
 {
 
 }

 void CDebug::reportErrorWarning(const char* warningString)
 {
	 SIM_PRINT( "%s\n", warningString );
 }