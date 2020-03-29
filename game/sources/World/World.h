#ifndef __WORLD_H
#define __WORLD_H

#include <sim_engine.h>

class CDebug;

class CWorld : public sim::IUpdatable
{
public:
	CWorld( io::CFileSystem* fs );
	~CWorld();
	
	void						Update( f32 dt, void *userData );
	void						Render( CDriver *driver );

	void						SetEnabled(bool enabled) {m_isEnabled = enabled;}
	bool						IsEnabled() {return m_isEnabled;}

	void						SetVisible(bool visible) {m_isVisible = visible;}
	bool						IsVisible() {return m_isVisible;}

	void						AddActor( CActor* actor );

protected:
	CPhysic*					m_physic;

	bool						m_isEnabled;
	bool						m_isVisible;

	CSkyBox*					m_skybox;
	CScene*						m_scene;
};

#endif // __WORLD_H
