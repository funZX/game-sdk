/* 
*    SIM - OpenGL ES 2.0 portable game engine
*    Copyright (C) 2012  Adrian SIMINCIUC
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <render/animation/sim_animation_pose.h>
#include <render/animation/sim_animation_frame.h>
#include <render/animation/sim_bone_animation.h>
#include <render/animation/sim_bone_mask.h>
#include <render/animation/sim_animation_player.h>

namespace sim
{
namespace rnr
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
}; // namespace rnr
}; // namespace sim
