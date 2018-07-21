#ifndef __STATE_APP_LOAD_H
#define __STATE_APP_LOAD_H

#include <simarian.h>

class CState_AppLoad : public IState
{
protected:

public:
	CState_AppLoad();
	~CState_AppLoad();
// ----------------------------------------------------------------------//	
	void				Update( f32 dt, void *userData );

	void				Render2D( CDriver *driver );
	void				Render3D( CDriver *driver );

	void				OnEnter();
	void				OnExit();

// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_LOAD_H
