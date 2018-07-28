#ifndef __GAME_H
#define __GAME_H

#include <sim_engine.h>

class CWorld;
class CWindow;

class CGame : public CEngine
{
	typedef std::map<std::string, rnr::CFont*>					TFontList;
	typedef std::map<std::string, rnr::CFont*>::iterator		TFontListIter;

public:
	CGame( const std::string& fsDir );
	~CGame();

	void						Update( f32 dt, void *userData );
	const std::string			GetFsPath( const std::string& fs);
	inline CWorld*				GetWorld()			{ return m_world; }

	void						KeyPress( u8 key, bool isDown );
	bool						IsKeyDown( u8 key ) { return m_downKeys[ key ]; }

protected:
	bool						m_downKeys[ 256 ];

	std::string					m_fsDir;
	CWorld*						m_world;
};


typedef struct 
{
	const char*					fsDir;

	CGame*						game;

	CDriver*					driver;	
	CWorld*						world;
	CCamera*					camera;
	CCanvas*					canvas;

	CMaterial*					material;

	CFont*						font;
	CEffect*					effect;
	

} TGlobal;

extern TGlobal		O;

#endif // __GAME_H

