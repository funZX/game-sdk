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
