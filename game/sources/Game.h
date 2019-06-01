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
	inline CWorld*				GetWorld()			{ return m_world; }

	void						KeyPress( u8 key, bool isDown );
	bool						IsKeyDown( u8 key ) { return m_downKeys[ key ]; }

protected:
	bool						m_downKeys[ 256 ];

	std::string					m_fsDir;
	CWorld*						m_world;

	bool						m_enableScriptDebugger;
};


typedef struct 
{
	CGame*						game;

	CDriver*					driver;	
	CWorld*						world;
	CCamera*					camera;
	CCanvas*					canvas;

	CMaterial*					material;

	struct 
	{
		CFont*					engine;

		struct 
		{
			CFont*				bold12;
			CFont*				bold16;
			CFont*				bold20;

			CFont*				italic12;
			CFont*				italic16;
			CFont*				italic20;

			CFont*				light12;
			CFont*				light16;
			CFont*				light20;

			CFont*				medium12;
			CFont*				medium16;
			CFont*				medium20;

			CFont*				regular12;
			CFont*				regular16;
			CFont*				regular20;

			CFont*				thin12;
			CFont*				thin16;
			CFont*				thin20;

		} roboto;
	} font;
	
	struct 
	{
		CEffect*				engine;

		CEffect*				color;
		CEffect*				vertex;
		CEffect*				texture;

		struct
		{
			CEffect*			normals;
		} debug;

		struct 
		{
			CEffect*			texture;

			struct 
			{
				CEffect*		color;
				CEffect*		texture;
			} vertex;

		} fill;

		struct 
		{
			CEffect*			color;
			CEffect*			texture;

			struct
			{
				CEffect*		color;
				CEffect*		texture;
			} vertex;

		} diffuse;

		struct
		{
			CEffect*			bumpmap;
			CEffect*			parallax;
			CEffect*			phong;
			CEffect*			reflection;
		} lighting;

		struct
		{
			CEffect*			sprite;
		} particle;

	} effect;
	

} TGlobal;

extern TGlobal		O;

#endif // __GAME_H

