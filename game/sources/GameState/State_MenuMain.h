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
	void				Render( CDriver *driver );

	void				OnEnter();
    void				OnGui(rnr::CDriver* driver);
	void				OnExit();

// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_INIT_H
