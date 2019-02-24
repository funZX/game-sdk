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

	if ( mask != nullptr )
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
