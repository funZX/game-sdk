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

#ifndef __SIM_INTERFACES_H
#define __SIM_INTERFACES_H

#include <core/sim_core.h>

namespace sim
{
// ----------------------------------------------------------------------//

namespace rnr { class CDriver; class CCanvas; class CAnimationPose; class CBoneMask; };
namespace vm  { class CVirtualMachine; };

// ----------------------------------------------------------------------//

struct IScriptable
{
	virtual void 				Register( vm::CVirtualMachine *vm ) = 0;
};

// ----------------------------------------------------------------------//

struct IRenderable
{
	virtual void 				Render( rnr::CDriver *driver ) = 0;
};

// ----------------------------------------------------------------------//

struct IUpdatable
{
	virtual void 				Update( f32 dt, void *userData ) = 0;
};

// ----------------------------------------------------------------------//

struct IAnimationBlend
{
	virtual void				Blend( rnr::CAnimationPose *pose, rnr::CBoneMask *mask ) = 0;
};

// ----------------------------------------------------------------------//

struct ISortable
{
public:
	virtual s32 				Compare( const ISortable *other ) = 0;
};

// ----------------------------------------------------------------------//

struct IState : public IUpdatable
{
public:
	virtual void				Render2D( rnr::CDriver *driver ) = 0;
	virtual void				Render3D( rnr::CDriver *driver ) = 0;

	virtual void				OnEnter()	= 0;
	virtual void				OnExit()	= 0;
};

// ----------------------------------------------------------------------//

class IEngineItem
{
public:
	IEngineItem() {}
	IEngineItem( const std::string& name ) { m_name = name; }

	const std::string& 			GetName() { return m_name; }

protected:
	std::string					m_name;
};

// ----------------------------------------------------------------------//
}; // namespace sim

#endif // __SIM_INTERFACES_H
