/*
*
*    MIT License
*
*    SIM - OpenGL ES 2.0 portable game engine
*    Copyright (C) 2012  Adrian SIMINCIUC
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*/

#include <render/animation/sim_animation_pose.h>
#include <render/animation/sim_animation_frame.h>
#include <render/animation/sim_bone_animation.h>
#include <render/animation/sim_bone_mask.h>
#include <render/animation/sim_animation_player.h>

namespace sim
{
namespace ren
{
// ----------------------------------------------------------------------//

CAnimationPlayer::CAnimationPlayer( CBoneAnimation* anim, f32 start_time )
{
	m_animation		= anim;

    m_frame			= start_time * anim->GetFPS();
    m_keyFrame		= (u32) m_frame;
    m_weight		= m_frame - m_keyFrame;
}

// ----------------------------------------------------------------------//

CAnimationPlayer::~CAnimationPlayer()
{
}

// ----------------------------------------------------------------------//

void CAnimationPlayer::SetFrame( f32 frame )
{
	m_frame = frame;

	u32 nFrames = m_animation->GetNumFrames() - 1;

    while ( m_frame >= nFrames )
		m_frame -= nFrames;

	m_keyFrame	= (u32) m_frame;
	m_weight	= m_frame - m_keyFrame;
}

// ----------------------------------------------------------------------//

void CAnimationPlayer::Update( u32 dt, void *userData  )
{
   m_frame += dt;

   u32 nFrames = m_animation->GetNumFrames() - 1;

   while ( m_frame >= nFrames )
	   m_frame -= nFrames;

	m_keyFrame	= (u32) m_frame;
	m_weight	= m_frame - m_keyFrame;
}

// ----------------------------------------------------------------------//

void CAnimationPlayer::Blend( CAnimationPose *pose, CBoneMask *mask )
{
	CAnimationFrame* frame0	= m_animation->GetFrame( m_keyFrame + 0 );
	CAnimationFrame* frame1	= m_animation->GetFrame( m_keyFrame + 1 );

	CAnimationPose* pose0	= frame0->GetPose();
	CAnimationPose* pose1	= frame1->GetPose();

	pose->Slerp( pose0, pose1, m_weight, mask );
}

// ----------------------------------------------------------------------//
}; // namespace ren
}; // namespace sim
