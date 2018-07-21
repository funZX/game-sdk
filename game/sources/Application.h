#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <simarian.h>

class CGame;

class CApplication : public CSingleton< CApplication >
{
protected:
	static const u32		MaxFps	= 60;
	static const u32		MinDt	= 1000 / MaxFps;

	CSimarian*				m_simarian;
	CGame*					m_Game;

	ALCdevice*				m_ALDevice;
	ALCcontext*				m_ALContext;

	u32						m_currentTime;
	u32						m_updateTime;
	u32						m_frameTime;
	f32						m_deltaTime;


protected:
	void					InitOpenGL();
	void					InitOpenAL();

public:
	CApplication();
	~CApplication();
	
	void					Start( int width, int height );
	void					Run( void );
	void					Quit( void );

	inline u32				GetCurrentTime()	{ return m_currentTime; }
	inline u32				GetFrameTime()		{ return m_frameTime; }
	inline f32				GetDeltaTime()		{ return m_deltaTime; }

	void					PointerDown( u32 x, u32 y );
	void					PointerDrag( u32 x, u32 y );
	void					PointerUp( u32 x, u32 y );
	void					KeyPress( u8 key, bool isDown );
};

#endif // __APPLICATION_H
