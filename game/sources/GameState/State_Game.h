#ifndef __STATE_GAME_H
#define __STATE_GAME_H

#include <sim_engine.h>
#include "../Debug/Debug.h"

class CState_Game : public IState, public sigcxx::Trackable
{
protected:

public:
	CState_Game();
	~CState_Game();
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
	void				DrawToWidget(CDriver* driver, sigcxx::SLOT slot = nullptr);
	// ----------------------------------------------------------------------//	
protected:
    CWorld*             m_world;

	CDrawable*	        m_drawable;
    CDebug*             m_debug;
// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_INIT_H
