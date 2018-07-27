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
	void						Render2D(CDriver *driver);

	void						SetFocus( bool focus ) {m_isFocused = focus;}
	bool						IsFocused() { return m_isFocused; }

	void						SetEnabled(bool enabled) {m_isEnabled = enabled;}
	bool						IsEnabled() {return m_isEnabled;}

	void						SetVisible(bool visible) {m_isVisible = visible;}
	bool						IsVisible() {return m_isVisible;}

	CFileSystem*				GetFs()			{ return m_fs; }

	void						AddActor( CActor* actor );

protected:
	CFileSystem*				m_fs;
	CPhysic*					m_physic;

	bool						m_isFocused;
	bool						m_isEnabled;
	bool						m_isVisible;

	CScene*						m_scene;

	CDebug*						m_debug;
};

#endif // __WORLD_H
