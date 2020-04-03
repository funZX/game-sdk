#ifndef __STATE_APP_MENUMAIN_H
#define __STATE_APP_MENUMAIN_H

#include <sim_engine.h>

class CState_MenuMain : public IState, public sigcxx::Trackable
{
protected:

public:
	CState_MenuMain( IState::TFnDtor fnDtor );
	~CState_MenuMain();
// ----------------------------------------------------------------------//	
protected:

    void				ShowGui( CCanvas* canvas );
    void				Update( f32 dt, void *userData );
	void				Render( CDriver *driver );

    void				OnEnter();
    void				OnExit();

    // ----------------------------------------------------------------------//	
    void				OnEvent( CCanvas* canvas, CCanvas::TEvent* ev, sigcxx::SLOT slot = nullptr );
    // ----------------------------------------------------------------------//	

// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_MENUMAIN_H
