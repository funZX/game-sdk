#ifndef __GAME_H
#define __GAME_H

#include <core/io/sim_file_system.h>
#include <sim_engine.h>

class CWorld;
class CWindow;

class CGame : public CEngine
{
public:
	CGame( const std::string& fsDir, IState* initState );
	~CGame();

	void						Update( f32 dt, void *userData );
	const std::string			GetFsPath( const std::string& fs);

protected:
	std::string					m_fsDir;
};


typedef std::map<std::string, CFileSystem*>				TFsList;
typedef std::map<std::string, CFileSystem*>::iterator	TFsListIterator;

typedef struct _TGlobal
{
	CGame*						game;

	CDriver*					driver;	
	CWorld*						world;
	CCamera**					camera;
	CCanvas*					canvas;

	CMaterial*					material;
	CFont*						font;
	CEffect*					effect;

    TFsList						fsList = {
        { "debug.7z",       nullptr },
        { "ui.7z",          nullptr },
        { "world.7z",       nullptr },
        { "strawberry.7z",  nullptr },
    };

    inline CFileSystem* GetFs( const std::string& fsName )
    {
		TFsListIterator it = fsList.find(fsName);
        return it == fsList.end() ? nullptr : fsList[fsName];
    }

    ~_TGlobal();

} TGlobal;

extern TGlobal		g;

#define UNLOAD_FNDTOR(x) [x]() \
{ \
    for (auto& fsName : fsList) \
    { \
        TFsListIterator it = g.fsList.find(fsName); \
        if (it == g.fsList.end()) \
            continue; \
        if (it->second == nullptr) \
            continue; \
        SIM_PRINT("\nUNLOAD_FNDTOR: %s",fsName.c_str()); \
        it->second->~CFileSystem(); \
        it->second = nullptr; \
    } \
}; \

#endif // __GAME_H

