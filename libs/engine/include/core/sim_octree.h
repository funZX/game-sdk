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
#include <sigcxx/sigcxx.hpp>

#include <core/sim_pool.h>
#include <core/sim_interfaces.h>
#include <core/sim_balance_tree.h>
#include <core/sim_core.h>

#include <math/sim_vec3.h>

#ifndef __SIM_OCTREE_H
#define __SIM_OCTREE_H

using namespace sim::mat;

namespace sim
{
namespace stl
{
// ----------------------------------------------------------------------//

class COctreeVolume
{
public:
	COctreeVolume();

	inline const u32		GetID() { return id::Get();  }

	inline void				SetBox(const TVec3* box) { Vec3Copy(&m_box, box); }
	inline const TVec3*		GetBox() const { return &m_box; }

	inline void				SetCenter(const TVec3* center) { Vec3Copy(&m_center, center); }
	inline const TVec3*		GetCenter() const { return &m_center; }

	inline void				SetRadius(f32 radius) { m_radius = radius; }
	inline f32				GetRadius() const { return m_radius; }

	// ----------------------------------------------------------------------//
	inline f32				GetMaxX() const { return m_center.x + m_radius; }
	inline f32				GetMaxY() const { return m_center.y + m_radius; }
	inline f32				GetMaxZ() const { return m_center.z + m_radius; }
	// ----------------------------------------------------------------------//
	inline f32				GetMinX() const { return m_center.x - m_radius; }
	inline f32				GetMinY() const { return m_center.y - m_radius; }
	inline f32				GetMinZ() const { return m_center.z - m_radius; }
	// ----------------------------------------------------------------------//

public:
	// ------------------------------------------------------------------//
	sigcxx::Signal<>		OctreeUpdateSignal;
	// ------------------------------------------------------------------//

protected:
	TVec3					m_box;
	TVec3					m_center;
	f32						m_radius;
};





// ----------------------------------------------------------------------//

class COctreeNode : public sigcxx::Trackable, public IDummySlot
{
	friend class COctree;
public:
	// ----------------------------------------------------------------------//
	
	COctreeNode()
	{
		m_octreeparent = nullptr;
		
		SIM_MEMSET( m_octreenodes, 0, sizeof( m_octreenodes ) );
	}

	// ----------------------------------------------------------------------//
	
	virtual ~COctreeNode()
	{
	}

	// ----------------------------------------------------------------------//
	typedef stl::CBalanceTree<u32, COctreeVolume*> TVolumeTree;
	// ----------------------------------------------------------------------//

	bool					IsLeaf() { return !m_octreenodes[ 0 ]; }

	// ----------------------------------------------------------------------//
	inline f32				GetMaxX() { return m_center.x + m_radius; }
	inline f32				GetMaxY() { return m_center.y + m_radius; }
	inline f32				GetMaxZ() { return m_center.z + m_radius; }
	// ----------------------------------------------------------------------//
	inline f32				GetMinX() { return m_center.x - m_radius; }
	inline f32				GetMinY() { return m_center.y - m_radius; }
	inline f32				GetMinZ() { return m_center.z - m_radius; }
	// ----------------------------------------------------------------------//
	u32						IsOutside(const COctreeVolume* volume);

	void					NewVolume(COctreeVolume* volume);
	void					DelVolume(COctreeVolume* volume);

	void					Split();
	void					OnDummySlot() {}
	// ----------------------------------------------------------------------//
protected:
	// ----------------------------------------------------------------------//
	enum
	{ k_TFL, k_TFR, k_TBL, k_TBR, k_BFL, k_BFR, k_BBL, k_BBR };
	// ----------------------------------------------------------------------//
	void					OnOctreeUpdate(sigcxx::SLOT slot = nullptr);
	// ----------------------------------------------------------------------//

	COctreeNode*			m_octreenodes[8];
	COctreeNode*			m_octreeparent;
	TVolumeTree				m_volumetree;

	TVec3					m_center;
	f32						m_radius;
};


// ----------------------------------------------------------------------//

class COctree
{
public:
	// ----------------------------------------------------------------------//
	COctree();
	virtual ~COctree();
	// ----------------------------------------------------------------------//
	void			Insert(COctreeVolume* volume);
	void			Delete(COctreeVolume* volume);
	// ----------------------------------------------------------------------//

protected:
	// ----------------------------------------------------------------------//
	COctreeNode*				m_root;
	CPool<COctreeNode>*			m_pool;
	// ----------------------------------------------------------------------//
	void			Expand(COctreeVolume* volume, u32 axis);
	void			Shrink();

	COctreeNode*	NewNode();
	void			DelNode(COctreeNode* node);

// ----------------------------------------------------------------------//
};

} // namespace stl;
} // namespace sim;

#endif // __SIM_OCTREE_H