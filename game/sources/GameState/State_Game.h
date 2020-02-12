#ifndef __STATE_GAME_H
#define __STATE_GAME_H

#include <sim_engine.h>

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

    void				OnEnter();
    void				OnExit();

protected:
	// ----------------------------------------------------------------------//	
	void				DrawToWidget(CDriver* driver, sigcxx::SLOT slot = nullptr);
	// ----------------------------------------------------------------------//	
protected:
	CFileSystem*		m_fs;

	CDrawable*	        m_drawable;
// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_INIT_H
