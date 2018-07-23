#ifndef __STATE_GAME_H
#define __STATE_GAME_H

#include <engine.h>

class CState_Game : public IState
{
protected:

public:
	CState_Game();
	~CState_Game();
// ----------------------------------------------------------------------//	
	void				Update( f32 dt, void *userData );

	void				Render2D( CDriver *drivers );
	void				Render3D( CDriver *driver );

	void				OnEnter()	{};
	void				OnExit()	{};

// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_INIT_H
