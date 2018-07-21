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
#include <render/animation/sim_bone_mask.h>

#include <render/animation/sim_animation_blend.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CAnimationBlend::CAnimationBlend( u32 nBones, IAnimationBlend* from, IAnimationBlend* to )
{
	SIM_ASSERT( from );
	SIM_ASSERT( to );

	m_fromPose		= SIM_NEW CAnimationPose( nBones );
	m_toPose		= SIM_NEW CAnimationPose( nBones );

	m_fromBlend		= from;
	m_toBlend		= to;

	m_weight		= 0.5f;
}

// ----------------------------------------------------------------------//

CAnimationBlend::~CAnimationBlend()
{
	SIM_SAFE_DELETE( m_fromPose );
	SIM_SAFE_DELETE( m_toPose );
}

// ----------------------------------------------------------------------//

void CAnimationBlend::Blend( CAnimationPose *pose, CBoneMask *mask )
{
	m_fromBlend->Blend( m_fromPose, mask );
	m_toBlend->Blend( m_toPose, mask );
		
	pose->Slerp( m_fromPose, m_toPose, m_weight, mask );
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
