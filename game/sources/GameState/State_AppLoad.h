#ifndef __STATE_APP_LOAD_H
#define __STATE_APP_LOAD_H

#include <sim_engine.h>

class CState_AppLoad : public IState
{
protected:

public:
	CState_AppLoad();
	~CState_AppLoad();
// ----------------------------------------------------------------------//	
	void				Update( f32 dt, void *userData );

	void				Render2D( CDriver *driver );
	void				Render3D( CDriver *driver );

	void				OnEnter();
	void				OnExit();

protected:
	CFileSystem*		m_fsui;
	CFileSystem*		m_fsworld;
	CFileSystem*		m_fsCrt;

// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_LOAD_H
