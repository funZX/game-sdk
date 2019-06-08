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

#ifndef __SIM_BONE_H
#define __SIM_BONE_H

#include <core/sim_interfaces.h>
#include <render/sim_render.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CBone : public IEngineItem
{
	friend class CBoneAnimation;

public:
	// ------------------------------------------------------------------//

	typedef struct
	{
		Vec3 v;
		Vec3 s;
		Quat q;

	} TTransform;

	// ------------------------------------------------------------------//

	CBone();
	CBone( const std::string& name );

	virtual ~CBone();

	// ------------------------------------------------------------------//
	inline void						SetParent( s32 parent )				{ m_parent = parent; }
	inline s32						GetParent()							{ return m_parent; }

	inline void						SetID(s32 iD) { m_iD = iD; }
	inline s32						GetID() { return m_iD; }
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	s32								m_iD;
	s32								m_parent;
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_BONE_H
