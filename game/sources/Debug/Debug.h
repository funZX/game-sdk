#ifndef __DEBUG_H
#define __DEBUG_H

#include <LinearMath/btIDebugDraw.h>
#include <simarian.h>

class CGame;

class CDebug : public IRenderable, btIDebugDraw
{
public:
	class CTiming
	{
	protected:
		u64		m_beginTime;
		u64		m_endTime;
		char*	m_debugName;
	public:
		CTiming( char *dbgName )
		{
			m_beginTime = GetTimeMicro();
			m_debugName	= dbgName;
		}

		~CTiming()
		{
			m_endTime = GetTimeMicro();
			SIM_PRINT( "\nCTiming[ \"%s\" ] in %lld micro.", m_debugName, m_endTime - m_beginTime );
		}
	};

public:
	CDebug( CFileSystem* fs );
	~CDebug();

	void						Render( CDriver *driver );
	void						Render2D(CDriver *driver);

	// btIDebugDraw interface
	void						drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);
	void						drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void						drawSphere (const btVector3& p, btScalar radius, const btVector3& color);
	void						drawTriangle(const btVector3& a,const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha);
	void						drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void						draw3dText(const btVector3& location, const char* textString);
	void						setDebugMode(int debugMode) { m_debugMode = debugMode; }
	int							getDebugMode() const { return m_debugMode;}
	void						reportErrorWarning(const char* warningString);

public:
	TSphere*					m_debugSphere;
	TCube*						m_debugCube;

	sim::s32					m_debugMode;
	CLight*						m_debugLight;

	CFileSystem*				m_fs;
	CRenderTexture*				m_rendertexture;
};

#endif // __WINDOW_H

