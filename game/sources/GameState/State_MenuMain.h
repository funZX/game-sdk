#ifndef __STATE_APP_INIT_H
#define __STATE_APP_INIT_H

#include <sim_engine.h>

class CState_MenuMain : public IState
{
protected:

public:
	CState_MenuMain();
	~CState_MenuMain();
// ----------------------------------------------------------------------//	
	void				Update( f32 dt, void *userData );

	void				Render2D( CDriver *driver );
	void				Render3D( CDriver *driver );

	void				OnEnter();
	void				OnExit();

// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_INIT_H
