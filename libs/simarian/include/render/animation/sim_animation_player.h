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

#ifndef __SIM_ANIMATION_PLAYER_H
#define __SIM_ANIMATION_PLAYER_H

#include <core/sim_interfaces.h>
#include <render/sim_render.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
class CAnimationPose;
class CBoneMask;
class CBoneAnimation;
// ----------------------------------------------------------------------//

class CAnimationPlayer : public IAnimationBlend, public IUpdatable
{
public:
	CAnimationPlayer( CBoneAnimation* anim, f32 start_time );
	virtual ~CAnimationPlayer();
	// ------------------------------------------------------------------//
	void						SetFrame( f32 frame );
	void						Update( u32 dt, void *userData  );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	void						Blend( CAnimationPose *pose, CBoneMask *mask );
	// ------------------------------------------------------------------//

protected:
	// ------------------------------------------------------------------//
	f32							m_frame;
	f32							m_weight;
	u32							m_keyFrame;

	CBoneAnimation*				m_animation;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_ANIMATION_PLAYER_H
