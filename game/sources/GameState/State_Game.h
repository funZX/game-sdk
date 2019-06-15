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
	void				Update( f32 dt, void *userData );

	void				Render2D( CDriver *drivers );
	void				Render3D( CDriver *driver );

	void				OnEnter()	{};
	void				OnExit()	{};

protected:
	// ----------------------------------------------------------------------//	
	void				DrawToWidget(CDriver* driver, sigcxx::SLOT slot = nullptr);
	// ----------------------------------------------------------------------//	
protected:
	CFileSystem*		m_fs;

	CMesh*				m_mesh;
	CDrawable*	        m_drawable;
// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_INIT_H
