#ifndef __STATE_APP_LOADING_H
#define __STATE_APP_LOADING_H

#include <sim_engine.h>

class CState_Loading : public IState
{
public:
    CState_Loading( const IState::TFsList& fsLoad, IState* nextState );
	~CState_Loading();
// ----------------------------------------------------------------------//	
protected:
    void				ShowGui( CCanvas* canvas );
    void				Update( f32 dt, void *userData );
	void				Render( CDriver *driver );

    void				OnEnter();
    void				OnExit();

protected:
    IState*             m_childState;

// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_LOADING_H
