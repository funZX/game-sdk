#ifndef __GAME_H
#define __GAME_H

#include <sim_engine.h>

class CWorld;
class CWindow;

class CGame : public CEngine
{
public:
	CGame( const std::string& fsDir );
	~CGame();

	void						Update( f32 dt, void *userData );
	const std::string			GetFsPath( const std::string& fs);

protected:
	std::string					m_fsDir;
};


typedef struct 
{
	CGame*						game;

	CDriver*					driver;	
	CWorld*						world;
	CCamera**					camera;
	CCanvas*					canvas;

	CMaterial*					material;
	CFont*						font;
	CEffect*					effect;

} TGlobal;

extern TGlobal		g;

#endif // __GAME_H

