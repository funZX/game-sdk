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

CBoneAnimation::CBoneAnimation(CBoneHierarchy* hierarchy)
{
	m_hierarchy		= hierarchy;

	m_nFrames		= 0;
	m_frames		= nullptr;

	m_fps			= 25;
}
// ----------------------------------------------------------------------//

CBoneAnimation::CBoneAnimation( const std::string &name, CBoneHierarchy* hierarchy)
	: CBoneAnimation(hierarchy)
{
	m_name = name;
}

// ----------------------------------------------------------------------//

CBoneAnimation::~CBoneAnimation()
{
	SIM_SAFE_DELETE_ARRAY( m_frames );
}

// ----------------------------------------------------------------------//

bool CBoneAnimation::Load( io::CMemStream* memstream )
{
	u32 nFrames = memstream->ReadU32();

	SIM_ASSERT( nFrames > 1 );

	m_frames	= SIM_NEW CAnimationFrame[ nFrames ];

	for ( u32 k = 0; k < nFrames; k++ )
	{
		CAnimationFrame* frame = &m_frames[ k ];

		frame->m_pose = SIM_NEW CAnimationPose( m_hierarchy->GetBonesCount() );

		for ( u32 j = 0; j < m_hierarchy->GetBonesCount(); j++ )
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

	m_frames[ nFrames - 1 ].m_pose->m_velocity = m_frames[ 0 ].m_pose->m_velocity;

    return true;
}
// ----------------------------------------------------------------------//
bool CBoneAnimation::Save(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
