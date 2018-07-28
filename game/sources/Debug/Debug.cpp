#include <GLES2/gl2.h>

#include <core/io/sim_json_stream.h>

#include <core/io/sim_file_system.h>

#include <render/scene/sim_light.h>
#include <render/scene/sim_camera.h>

#include <render/gui/sim_widget_drawable.h>

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
// ----------------------------------------------------------------------//
CDebug::CDebug(CFileSystem* fs)
	: m_debugMode(0)
{

	m_debugSphere	= gluNewSphere(32, 1.0f);
	m_debugCube		= gluNewCube(1.0f);

	m_debugLight = SIM_NEW CLight( "debug_light" );

	m_fs	= fs;
	m_mesh	= fs->GetMesh( "melonman/melonman" );

	m_drawable = SIM_NEW CWidgetDrawable();
	m_drawable->MoveTo( 0.0f, 30.0f );
	m_drawable->Resize( 400, 300 );

	m_drawable->OnDraw.Connect( this, &CDebug::DrawToWidget );
}
// ----------------------------------------------------------------------//
CDebug::~CDebug()
{
	SIM_SAFE_DELETE( m_drawable );
	SIM_SAFE_DELETE( m_debugLight );

	m_debugSphere	= gluDelSphere( m_debugSphere );
	m_debugCube		= gluDelCube( m_debugCube );
}
// ----------------------------------------------------------------------//
void CDebug::Render(CDriver *driver)
{
	f32 dr = driver->GetTimerRot();

	driver->MatrixPush();
	{
		driver->MatrixTranslateX(0.3f);
		driver->MatrixTranslateZ(-4.0f);
		m_mesh->Render(driver);
	}
	driver->MatrixPop();

	m_drawable->Draw( driver );
}
// ----------------------------------------------------------------------//
void CDebug::Render2D(CDriver *driver)
{
	static CEffect* fill = m_fs->GetEffect("color/fill_color");
	static CEffect* filltex = m_fs->GetEffect("color/fill_color_texture_color");
	f32 dr = 20.0f * driver->GetTimerRot();

	CMaterial m;
	CRect2D r;

	r.SetMaterial(&m);

	m.SetEffect(fill);
	m.SetTexture(0, 0);

	r.Bound(10.0f, 30.0f, 150.0f, 150.0f);
	r.Rotate(dr);
	r.Render(driver, CRect2D::OneSizeRect);

	bool isBatchEnabled =
	driver->EnableBatch2D(false);

	CEffect::TTechnique techique;
	filltex->CopyTechnique(&techique);
	filltex->m_technique.depthtest = false;

	m_drawable->SetMaterial( &m );
	m.SetEffect(filltex);
	m.SetTexture( m_drawable->GetTexture(), 0 );
	r.Zoom(-6.0f, -6.0f);
	r.Rotate(dr);
	r.Render(driver, CRect2D::OneSizeRectFlip);

	filltex->SetTechnique(&techique);
	driver->EnableBatch2D(isBatchEnabled);
}
// ----------------------------------------------------------------------//
void CDebug::DrawToWidget( CDriver* driver, sigcxx::SLOT slot )
{
	f32 dr = driver->GetTimerRot();

	driver->MatrixPush();
	driver->MatrixTranslateY(-0.5f);
	driver->MatrixTranslateZ(-2.0f);
	driver->MatrixRotateY(dr * 60.0f);
	m_mesh->Render(driver);
	driver->MatrixPop();

	driver->MatrixPush();
	driver->MatrixTranslate(-0.4f, -0.2f, -1.5f);
	driver->MatrixRotateY(dr * 20.0f);
	m_mesh->Render(driver);
	driver->MatrixPop();

	driver->MatrixPush();
	driver->MatrixTranslate(0.4f, -0.2f, -1.5f);
	driver->MatrixScale(0.5f, 0.5f, 0.5f);
	driver->MatrixRotateY(dr * 40.0f);
	m_mesh->Render(driver);
	driver->MatrixPop();
}
// ----------------------------------------------------------------------//
void CDebug::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{

}
// ----------------------------------------------------------------------//
void CDebug::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{

}
// ----------------------------------------------------------------------//
void CDebug::drawSphere(const btVector3& p, btScalar radius, const btVector3& color)
{
	CDriver* driver = O.driver;
	driver->MatrixPush();
	driver->MatrixLoadIdentity();
	driver->MatrixTranslate(p.getX(), p.getY(), p.getZ());
	driver->MatrixScale(radius, radius, radius);
	gluRenderSphere(driver, m_debugSphere);
	driver->MatrixPop();
}
// ----------------------------------------------------------------------//
void CDebug::drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha)
{

}
// ----------------------------------------------------------------------//
void CDebug::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{

}
// ----------------------------------------------------------------------//
void CDebug::draw3dText(const btVector3& location, const char* textString)
{

}
// ----------------------------------------------------------------------//
void CDebug::reportErrorWarning(const char* warningString)
{
	SIM_PRINT("%s\n", warningString);
}
// ----------------------------------------------------------------------//