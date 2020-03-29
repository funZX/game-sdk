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

    void				OnEnter( bool isPushed );
    void				OnExit( bool isPoped );

protected:
// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_LOAD_H
