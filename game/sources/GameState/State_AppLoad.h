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
    void				ShowGui( CCanvas* canvas );
    void				Update( f32 dt, void *userData );
	void				Render( CDriver *driver );

    void				OnEnter();
    void				OnExit();

protected:
	void				InitO();
	CFileSystem*		m_fsui;
	CFileSystem*		m_fsworld;
	CFileSystem*		m_fsstrawberry;
	CFileSystem*		m_fsCrt;

// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_LOAD_H
