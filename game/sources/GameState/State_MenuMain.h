#ifndef __STATE_APP_MENUMAIN_H
#define __STATE_APP_MENUMAIN_H

#include <sim_engine.h>

class CState_MenuMain : public IState, public sigcxx::Trackable
{
protected:

public:
	CState_MenuMain();
	~CState_MenuMain();
// ----------------------------------------------------------------------//	
    void				ShowGui( CCanvas* canvas );
    void				Update( f32 dt, void *userData );
	void				Render( CDriver *driver );

    void				OnEnter( bool isPushed );
    void				OnExit( bool isPoped );

protected:
    // ----------------------------------------------------------------------//	
    void				MouseDown(CCanvas* canvas, int button, sigcxx::SLOT slot = nullptr);
    void				MouseUp(CCanvas* canvas, int button, sigcxx::SLOT slot = nullptr);
    void				MouseMove(CCanvas* canvas, f32 x, f32 y, sigcxx::SLOT slot = nullptr);
    void				KeyDown(CCanvas* canvas, int Key, bool KeyShift, bool KeyCtrl, bool KeyAlt, sigcxx::SLOT slot = nullptr);
    void				KeyUp(CCanvas* canvas, int Key, bool KeyShift, bool KeyCtrl, bool KeyAlt, sigcxx::SLOT slot = nullptr);
    // ----------------------------------------------------------------------//	

// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_MENUMAIN_H
