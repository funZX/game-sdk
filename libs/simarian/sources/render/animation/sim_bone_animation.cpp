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

#include <core/io/sim_mem_stream.h>

#include <render/animation/sim_animation_frame.h>
#include <render/animation/sim_animation_pose.h>
#include <render/animation/sim_bone_hierarchy.h>
#include <render/animation/sim_bone_animation.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CBoneAnimation::CBoneAnimation( const std::string &name )
{
	m_name			= name;

	m_hierarchy		= NULL;

	m_nFrames		= 0;
	m_frames		= NULL;

	m_fps			= 25;
}

// ----------------------------------------------------------------------//

CBoneAnimation::~CBoneAnimation()
{
	SIM_SAFE_DELETE_ARRAY( m_frames );
	SIM_SAFE_DELETE_ARRAY( m_hierarchy );
}

// ----------------------------------------------------------------------//

void CBoneAnimation::Load( io::CMemStream* memstream )
{
	u32 nBones	= memstream->ReadU32();

	m_hierarchy = SIM_NEW CBoneHierarchy( nBones );

	for ( u32 k = 0; k < nBones; k++ )
		m_hierarchy->m_bones[ k ].m_parent = memstream->ReadS16();

	u32 nFrames = memstream->ReadU32();

	SIM_ASSERT( nFrames > 1 );

	m_frames	= SIM_NEW CAnimationFrame[ nFrames ];

	for ( u32 k = 0; k < nFrames; k++ )
	{
		CAnimationFrame* frame = &m_frames[ k ];

		frame->m_pose = SIM_NEW CAnimationPose( nBones );

		for ( u32 j = 0; j < nBones; j++ )
		{
			CBone::TTransform* bone = &frame->m_pose->m_bones[ j ];

			bone->v.x = memstream->ReadF32();
			bone->v.y = memstream->ReadF32();
			bone->v.z = memstream->ReadF32();

			bone->s.x = memstream->ReadF32();
			bone->s.y = memstream->ReadF32();
			bone->s.z = memstream->ReadF32();

			bone->q.x = memstream->ReadF32();
			bone->q.y = memstream->ReadF32();
			bone->q.z = memstream->ReadF32();
			bone->q.w = memstream->ReadF32();

			if ( k != 0 )
			{
				CAnimationFrame* f = &m_frames[ k - 1 ];

    			f->m_pose->m_velocity.x = (frame->m_pose->m_root->v.x - f->m_pose->m_root->v.x) * m_fps;
    			f->m_pose->m_velocity.y = (frame->m_pose->m_root->v.y - f->m_pose->m_root->v.y) * m_fps;
    			f->m_pose->m_velocity.z = (frame->m_pose->m_root->v.z - f->m_pose->m_root->v.z) * m_fps;					
			}
		}

		if ( k != 0 )
		{
			frame->m_pose->m_root->v.x = m_frames[ 0 ].m_pose->m_root->v.x;
			frame->m_pose->m_root->v.y = m_frames[ 0 ].m_pose->m_root->v.y;
			frame->m_pose->m_root->v.z = m_frames[ 0 ].m_pose->m_root->v.z;
		}
	}

	Vec3Copy( &m_frames[ nFrames - 1 ].m_pose->m_velocity, &m_frames[ 0 ].m_pose->m_velocity );
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
