#include <GLES2/gl2.h>

#include <core/sim_core.h>

#include <render/scene/sim_camera.h>
#include <render/sim_glaux.h>
#include <render/sim_driver.h>

#include "Debug.h"
#include "../Game.h"
// ----------------------------------------------------------------------//
CDebug::CDebug( io::CFileSystem* fs )
	: m_debugMode(0)
	, m_fs( fs )
{
	m_debugSphere	= gluNewSphere(32, 1.0f);
    m_debugSphere->vertexGroup->SetMaterial(g.material);

	m_debugCube		= gluNewCube(1.0f);
	m_debugCube->vertexGroup->SetMaterial(g.material);
}
// ----------------------------------------------------------------------//
CDebug::~CDebug()
{
	m_debugSphere	= gluDelSphere( m_debugSphere );
	m_debugCube		= gluDelCube( m_debugCube );
}
// ----------------------------------------------------------------------//
void CDebug::Render( CDriver *driver )
{
	CCamera* cam = g.game->GetCamera();

	Vec3 p1 = { -1, 0, -10 };
	Vec3 p2 = {  1, 0, -10 };

    driver->MatrixPush();
    driver->MatrixTranslate(p1);
    gluRenderSphere(driver, m_debugSphere);
    driver->MatrixPop();

    driver->MatrixPush();
    driver->MatrixTranslate(p2);
    gluRenderCube(driver, m_debugCube);
    driver->MatrixPop();
}
// ----------------------------------------------------------------------//
void CDebug::ShowGui( CCanvas* canvas )
{

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