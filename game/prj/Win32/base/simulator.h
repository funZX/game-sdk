#ifndef __PLATFORM_H
#define __PLATFORM_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include <pthread.h>
#include <sched.h>
#include <semaphore.h>

#include <iostream>

typedef struct
{
	float x;
	float y;

} CGPoint;

typedef void( OnTouchBeganCallback( CGPoint *, int ) );
typedef void( OnTouchMoveCallback( CGPoint * ) );
typedef void( OnTouchEndCallback( CGPoint * ) );
typedef void( OnRenderCallback( void ) );
typedef void( OnResizeCallback( int , int ) );
typedef void( OnKeyCallback( unsigned char, bool isDown ) );
typedef void( OnQuitCallback() );

void SimulatorSetTouchesBeganCallback( OnTouchBeganCallback * );
void SimulatorSetTouchesMovedCallback( OnTouchMoveCallback * );
void SimulatorSetTouchesEndedCallback( OnTouchEndCallback * );
void SimulatorSetRenderCallback( OnRenderCallback * );
void SimulatorSetResizeFromLayerCallback( OnResizeCallback * );
void SimulatorSetKeyCallback( OnKeyCallback * );
void SimulatorSetQuitCallback( OnQuitCallback * );

void SimulatorRun( int width, int height );

#endif // __PLATFORM_H