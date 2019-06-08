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

#include <sigcxx/sigcxx.hpp>

#include <core/sim_pool.h>
#include <core/sim_interfaces.h>
#include <core/sim_balance_tree.h>
#include <core/sim_core.h>

#ifndef __SIM_OCTREE_H
#define __SIM_OCTREE_H

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

	inline void				SetBox(Vec3* box) { m_box = zpl_vec3fv(box->e); }
	inline const Vec3*		GetBox() const  { return &m_box; }

	inline void				SetCenter( Vec3* center) { m_center = zpl_vec3fv(center->e); }
	inline const Vec3*		GetCenter() const { return &m_center; }

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
	Vec3					m_box;
	Vec3					m_center;
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

	Vec3					m_center;
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