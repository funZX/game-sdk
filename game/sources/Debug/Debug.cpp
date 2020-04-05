#include <GLES2/gl2.h>

#include <core/sim_core.h>
#include <core/io/sim_file_system.h>

#include <render/scene/sim_camera.h>
#include <render/scene/sim_light.h>
#include <render/sim_glaux.h>
#include <render/sim_driver.h>

#include <imgui.h>

#include "Debug.h"
#include "../Game.h"
// ----------------------------------------------------------------------//
CDebug::CDebug( io::CFileSystem* fs )
	: m_debugMode(0)
	, m_fs( fs )
{
	m_debugLight = SIM_NEW CLight();
	m_debugLight->GetState()->isStatic = false;

    CTexture* nm = fs->GetTexture("nm.0.pvr.clamp.linear.pvr");

	m_debugSphere	= gluNewSphere(32, 1.0f);
    m_debugSphere->vertexGroup->SetMaterial(g.material);
	m_debugSphere->vertexGroup->GetMaterial()->SetEffect(fs->GetEffect("debug.normals"));
	m_debugSphere->vertexGroup->GetMaterial()->SetTexture(nm, 1);

	m_debugCube		= gluNewCube(1.0f);
	m_debugCube->vertexGroup->SetMaterial(g.material);
	m_debugCube->vertexGroup->GetMaterial()->SetEffect(fs->GetEffect("lighting.phong"));
    m_debugCube->vertexGroup->GetMaterial()->SetTexture(nm, 1);
}
// ----------------------------------------------------------------------//
CDebug::~CDebug()
{
	m_debugSphere	= gluDelSphere( m_debugSphere );
	m_debugCube		= gluDelCube( m_debugCube );
	m_fs			= nullptr;

	SIM_SAFE_DELETE( m_debugLight );
}
// ----------------------------------------------------------------------//
void CDebug::Update(f32 dt, void* userData)
{
	Transform* tr = m_debugLight->GetTransform();

	tr->translation = { 0, 5, -10 };
	//tr->quaternion = zpl_quat_axis_angle({ 0, 1, 0 }, 1);

	m_debugLight->Update( dt, userData );
}
// ----------------------------------------------------------------------//
void CDebug::Render( CDriver *driver )
{
	CCamera* cam = g.game->GetCamera();

	Vec3 p1 = { -2, 0, -10 };
	Vec3 p2 = {  2, 0, -10 };

	m_debugLight->Render( driver );

	f32 r = driver->GetTimer();
	f32 s = 1.0f + 0.5f * zpl_sin(r);

    driver->MatrixPush();
	driver->MatrixTranslate(p1);
    driver->MatrixRotate({ 0, 1, 0 }, zpl_to_radians(100.0f * r));
    driver->MatrixScale({ s, s, s });
    gluRenderSphere(driver, m_debugSphere);
    driver->MatrixPop();

    driver->MatrixPush();
    driver->MatrixTranslate(p2);
    driver->MatrixRotate({ 1, 0, 0 }, zpl_to_radians(100.0f*driver->GetTimer()));
	driver->MatrixScale({ s, s, s });;
    gluRenderCube(driver, m_debugCube);
    driver->MatrixPop();
}
// ----------------------------------------------------------------------//
void CDebug::ShowGui( CCanvas* canvas, sigcxx::SLOT slot )
{
	ImGui::ShowDemoWindow();
}

// ----------------------------------------------------------------------//
void CDebug::drawLine( const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor )
{

}
// ----------------------------------------------------------------------//
void CDebug::drawLine(const btVector3& from, const btVector3& to, const btVector3& color )
{

}
// ----------------------------------------------------------------------//
void CDebug::drawSphere( const btVector3& p, btScalar radius, const btVector3& color )
{
	CDriver* driver = g.driver;

	driver->MatrixPush();
	driver->MatrixLoadIdentity();
    driver->MatrixTranslate({ p.getX(), p.getY(), p.getZ() });
    driver->MatrixScale({ radius, radius, radius });
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