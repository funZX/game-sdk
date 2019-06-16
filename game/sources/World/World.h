#ifndef __WORLD_H
#define __WORLD_H

#include <sim_engine.h>

class CDebug;

class CWorld : public sim::IUpdatable
{
public:
	CWorld();
	~CWorld();
	
	void						Update( f32 dt, void *userData );
	void						Render( CDriver *driver );

	void						SetEnabled(bool enabled) {m_isEnabled = enabled;}
	bool						IsEnabled() {return m_isEnabled;}

	void						SetVisible(bool visible) {m_isVisible = visible;}
	bool						IsVisible() {return m_isVisible;}

	CFileSystem*				GetFs()		{ return m_fs; }

	void						AddActor( CActor* actor );

protected:
	CFileSystem*				m_fs;
	CPhysic*					m_physic;

	bool						m_isEnabled;
	bool						m_isVisible;

	CScene*						m_scene;
};

#endif // __WORLD_H
