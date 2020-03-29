#ifndef __STATE_APP_LOADING_H
#define __STATE_APP_LOADING_H

#include <sim_engine.h>

class CState_Loading : public IState
{
public:
    CState_Loading( const std::vector<std::string>& fsLoad, IState* nextState );
	~CState_Loading();
// ----------------------------------------------------------------------//	
    void				ShowGui( CCanvas* canvas );
    void				Update( f32 dt, void *userData );
	void				Render( CDriver *driver );

    void				OnEnter( bool isPushed );
    void				OnExit( bool isPoped );

    static CFileSystem* GetFs(const std::string& fsName);

protected:
    IState*             m_nextState;

    static std::map<std::string, CFileSystem*> m_fsList;

// ----------------------------------------------------------------------//
};

#endif // __STATE_APP_LOADING_H
