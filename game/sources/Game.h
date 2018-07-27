#ifndef __GAME_H
#define __GAME_H

#include <engine.h>

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

	void						PointerDown( u32 x, u32 y );
	void						PointerDrag( u32 x, u32 y );
	void						PointerUp( u32 x, u32 y );

	void						KeyPress( u8 key, bool isDown );

	const std::string			GetFsPath( const std::string& fs);

	inline CWorld*				GetWorld()			{ return m_world; }

protected:
	std::string					m_fsDir;
	CWorld*						m_world;
};


typedef struct 
{
	const char*					fsDir;

	CGame*						game;
	CDriver*					driver;
	CEffect*					effect;
	CWorld*						world;
	CCamera*					camera;
	CCanvas*					canvas;

	struct
	{
		CFont*					engine;
	} font;

} TGlobal;

extern TGlobal		O;

#endif // __GAME_H

