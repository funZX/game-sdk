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

#include <render/animation/sim_bone_mask.h>
#include <render/animation/sim_animation_pose.h>

using namespace sim::mat;

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CAnimationPose::CAnimationPose( u32 nBones )
{
	SIM_ASSERT( nBones > 0 );

	m_bones		= SIM_NEW CBone::TTransform[ nBones ];
	m_root		= m_bones;

	m_nBones	= nBones;
}

// ----------------------------------------------------------------------//

CAnimationPose::~CAnimationPose()
{
	SIM_SAFE_DELETE_ARRAY( m_bones );
}

// ----------------------------------------------------------------------//

void CAnimationPose::Slerp( const CAnimationPose *from, const CAnimationPose *to, f32 weight, CBoneMask *mask )
{
	f32 w0 = 1.0f - weight;

	if ( mask != NULL )
	{
		if ( mask->IsSet( 0 ) )
		{
			m_root->v.x		= from->m_root->v.x * w0 + to->m_root->v.x * weight;
			m_root->v.y		= from->m_root->v.y * w0 + to->m_root->v.y * weight;
			m_root->v.z		= from->m_root->v.z * w0 + to->m_root->v.z * weight;

			m_root->s.x		= from->m_root->s.x * w0 + to->m_root->s.x * weight;
			m_root->s.y		= from->m_root->s.y * w0 + to->m_root->s.y * weight;
			m_root->s.z		= from->m_root->s.z * w0 + to->m_root->s.z * weight;

			m_velocity.x	= from->m_velocity.x * w0 + to->m_velocity.x * weight;
			m_velocity.y	= from->m_velocity.y * w0 + to->m_velocity.y * weight;
			m_velocity.z	= from->m_velocity.z * w0 + to->m_velocity.z * weight;
		}

		for ( u32 k = 0; k < m_nBones; k++ )
			if ( mask->IsSet( k ) )
				QuatSlerp( &m_bones[ k ].q, &from->m_bones[ k ].q, weight, &to->m_bones[ k ].q );
	} 
	else
	{
		m_root->v.x		= from->m_root->v.x * w0 + to->m_root->v.x * weight;
		m_root->v.y		= from->m_root->v.y * w0 + to->m_root->v.y * weight;
		m_root->v.z		= from->m_root->v.z * w0 + to->m_root->v.z * weight;

		m_root->v.x		= from->m_root->s.x * w0 + to->m_root->s.x * weight;
		m_root->v.y		= from->m_root->s.y * w0 + to->m_root->s.y * weight;
		m_root->v.z		= from->m_root->s.z * w0 + to->m_root->s.z * weight;

		m_velocity.x	= from->m_velocity.x * w0 + to->m_velocity.x * weight;
		m_velocity.y	= from->m_velocity.y * w0 + to->m_velocity.y * weight;
		m_velocity.z	= from->m_velocity.z * w0 + to->m_velocity.z * weight;
		
		for ( u32 k = 0; k < m_nBones; k++)
			QuatSlerp( &m_bones[ k ].q, &from->m_bones[ k ].q, weight, &to->m_bones[ k ].q );
	}
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
