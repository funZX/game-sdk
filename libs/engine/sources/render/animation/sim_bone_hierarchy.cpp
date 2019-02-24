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

#include <render/animation/sim_bone_hierarchy.h>
#include <core/io/sim_mem_stream.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CBoneHierarchy::CBoneHierarchy()
{
	m_nBones = 0;

	m_bones = nullptr;
}

// ----------------------------------------------------------------------//

CBoneHierarchy::~CBoneHierarchy()
{
	SIM_SAFE_DELETE_ARRAY( m_bones );
}

// ----------------------------------------------------------------------//

void CBoneHierarchy::Load(io::CMemStream* ms)
{
	m_nBones = ms->ReadU16();

	SIM_ASSERT(m_nBones > 0);

	m_bones = SIM_NEW CBone[m_nBones];
}

// ----------------------------------------------------------------------//

void CBoneHierarchy::Save(io::CMemStream* ms)
{
	ms->WriteU16(m_nBones);


}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
