#ifndef __STATE_GAME_H
#define __STATE_GAME_H

#include <sim_engine.h>
#include "../Debug/Debug.h"

class CState_Game : public IState, public sigcxx::Trackable
{
protected:

public:
	CState_Game( IState::TFnDtor fnDtor );
	~CState_Game();
// ----------------------------------------------------------------------//	
protected:
    void				ShowGui( CCanvas* canvas );
    void				Update( f32 dt, void *userData );
	void				Render( CDriver *driver );

    void				OnEnter();
    void				OnExit();
    void				OnEvent(CCanvas* canvas, CCanvas::TEvent* ev, sigcxx::SLOT slot = nullptr);
    void				OnDrawableDraw(CDriver* driver, sigcxx::SLOT slot = nullptr);

protected:
    CWorld*             m_world;

	CDrawable*	        m_drawable;
    CDebug*             m_debug;
// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_INIT_H
