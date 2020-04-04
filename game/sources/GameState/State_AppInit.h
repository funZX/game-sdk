#ifndef __STATE_APP_INIT_H
#define __STATE_APP_INIT_H

#include <sim_engine.h>

class CState_AppInit : public IState
{
protected:


public:
	CState_AppInit();
    virtual ~CState_AppInit();
// ----------------------------------------------------------------------//	
protected:
    void				ShowGui( CCanvas* canvas );

    void				Update( f32 dt, void *userData );
	void				Render( CDriver *driver );

    void				OnEnter();
    void				OnExit();

// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_INIT_H
