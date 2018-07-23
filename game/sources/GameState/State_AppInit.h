#ifndef __STATE_APP_INIT_H
#define __STATE_APP_INIT_H

#include <engine.h>

class CState_AppInit : public IState
{
protected:


public:
	CState_AppInit();
	~CState_AppInit();
// ----------------------------------------------------------------------//	
	void				Update( f32 dt, void *userData );

	void				Render2D( CDriver *driver );
	void				Render3D( CDriver *driver );

	void				OnEnter();
	void				OnExit();

// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_INIT_H
