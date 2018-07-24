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

#ifndef __SIM_BONE_ANIMATION_H
#define __SIM_BONE_ANIMATION_H

#include <core/sim_interfaces.h>
#include <render/sim_render.h>

namespace sim
{
// ----------------------------------------------------------------------//
namespace io { class CMemStream; };
// ----------------------------------------------------------------------//
namespace rnr
{
// ----------------------------------------------------------------------//
class CBoneHierarchy;
class CAnimationFrame;
// ----------------------------------------------------------------------//
class CBoneAnimation : public IEngineItem
{
public:
	CBoneAnimation();
	CBoneAnimation( const std::string &name );
	virtual ~CBoneAnimation();
	// ------------------------------------------------------------------//
	inline u32						GetNumFrames()			{ return m_nFrames; }
	inline f32						GetFPS()				{ return m_fps; }
	inline CAnimationFrame*			GetFrame( u32 index )	{ return &m_frames[ index ]; }

	void							Load( io::CMemStream* memstream );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	CBoneHierarchy*					m_hierarchy;
	
	CAnimationFrame*				m_frames;
	u32								m_nFrames;

	f32								m_fps;
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_BONE_ANIMATION_H
