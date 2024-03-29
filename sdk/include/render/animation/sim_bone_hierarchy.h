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

#ifndef __SIM_BONE_HIERARCHY_H
#define __SIM_BONE_HIERARCHY_H

#include <render/sim_render.h>
#include <render/animation/sim_bone.h>

namespace sim
{
// ----------------------------------------------------------------------//
namespace io
{
	class CMemStream;
}
// ----------------------------------------------------------------------//
namespace ren
{
// ----------------------------------------------------------------------//
class CBone;
// ----------------------------------------------------------------------//

class CBoneHierarchy
{
	friend class CBoneAnimation;

public:
	CBoneHierarchy();
	virtual ~CBoneHierarchy();

	// ------------------------------------------------------------------//
	u32			        GetBonesCount()			{ return m_nBones; }
	CBone*		        GetBone( u32 index )	{ return &m_bones[index]; }

	virtual bool		Load( io::CMemStream* ms );
	virtual bool		Save( io::CMemStream* ms );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
    CBone* m_bones;
    u32			        m_nBones;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace ren
}; // namespace sim

#endif // __SIM_BONE_HIERARCHY_H
