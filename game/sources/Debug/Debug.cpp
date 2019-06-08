#include <GLES2/gl2.h>

#include <core/sim_core.h>

#include <render/sim_glaux.h>
#include <render/sim_driver.h>

#include "Debug.h"
// ----------------------------------------------------------------------//
CDebug::CDebug()
	: m_debugMode(0)
{

	m_debugSphere	= gluNewSphere(32, 1.0f);
	m_debugCube		= gluNewCube(1.0f);
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

}
// ----------------------------------------------------------------------//
void CDebug::Render2D( CDriver *driver )
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
	CDriver* driver = CDriver::GetSingletonPtr();
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