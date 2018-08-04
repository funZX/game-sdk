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

#include <core/sim_core.h>
#include <math/sim_vec3.h>

#include <core/sim_balance_tree.h>

using namespace sim::mat;

#ifndef __SIM_OCTREE_H
#define __SIM_OCTREE_H

namespace sim
{
namespace stl
{

// ----------------------------------------------------------------------//

#define CNode COctreeNode<DATA>
#define CTree COctree<DATA>

template<class DATA>
class COctreeData
{
public:
	COctreeData()
	{
		Vec3ToZero( &m_box );
		Vec3ToZero( &m_center );

		m_radius = 1.0f;
	}

	inline void								SetBox( const TVec3* box)			{ Vec3Copy( &m_box, box ); }
	inline const TVec3*						GetBox() const						{ return &m_box; }

	inline void								SetCenter( const TVec3* center){ Vec3Copy( &m_center, center ); }
	inline const TVec3*						GetCenter() const					{ return &m_center; }

	inline void								SetRadius( f32 radius )				{ m_radius = radius; }
	inline f32								GetRadius() const					{ return m_radius; }

	virtual const u32						GetID() = 0;

	// ----------------------------------------------------------------------//
	inline f32 GetMaxX()					{ return m_center.x + m_radius; }
	inline f32 GetMaxY()					{ return m_center.y + m_radius; }
	inline f32 GetMaxZ()					{ return m_center.z + m_radius; }
	// ----------------------------------------------------------------------//
	inline f32 GetMinX()					{ return m_center.x - m_radius; }
	inline f32 GetMinY()					{ return m_center.y - m_radius; }
	inline f32 GetMinZ()					{ return m_center.z - m_radius; }
	// ----------------------------------------------------------------------//

protected:
	TVec3							m_box;
	TVec3							m_center;
	f32								m_radius;
};
// ----------------------------------------------------------------------//
	
template <class DATA>
class COctreeNode
{
public:
	// ----------------------------------------------------------------------//
	typedef stl::CBalanceTree<u32, DATA>			TData;
	// ----------------------------------------------------------------------//
	typedef enum
	{
		k_Type_TFL,
		k_Type_TFR,
		k_Type_TBL,
		k_Type_TBR,
		k_Type_BFL,
		k_Type_BFR,
		k_Type_BBL,
		k_Type_BBR,

	} K_TYPE;
	// ----------------------------------------------------------------------//
	
	COctreeNode()
	{
		m_parent	= nullptr;

		SIM_MEMSET( m_children, 0, sizeof( m_children ) );
	}

	// ----------------------------------------------------------------------//
	
	~COctreeNode()
	{
		for ( int k = 0; k < 8; k++ )
			SIM_SAFE_DELETE( m_children[ k ] );
	}

	// ----------------------------------------------------------------------//
	
	bool IsLeaf() { return !m_children[ 0 ]; }

	// ----------------------------------------------------------------------//
	f32 GetMaxX() { return m_center.x + m_radius; }
	f32 GetMaxY() { return m_center.y + m_radius; }
	f32 GetMaxZ() { return m_center.z + m_radius; }
	// ----------------------------------------------------------------------//
	f32 GetMinX() { return m_center.x - m_radius; }
	f32 GetMinY() { return m_center.y - m_radius; }
	f32 GetMinZ() { return m_center.z - m_radius; }
	// ----------------------------------------------------------------------//

	u32 IsOutside( DATA data )
	{
		if ( data->GetMaxX() > GetMaxX() )
			return 1;

		if ( data->GetMaxY() > GetMaxY() )
			return 2;

		if ( data->GetMaxZ() > GetMaxZ() )
			return 3;

		if ( data->GetMinX() < GetMinX() )
			return -1;

		if ( data->GetMinY() < GetMinY() )
			return -2;

		if ( data->GetMinZ() < GetMinZ() )
			return -3;

		return 0;
	}

	// ----------------------------------------------------------------------//
	
	void Split()
	{
	
	}

	// ----------------------------------------------------------------------//
public:
	COctreeNode*					m_children[8];
	COctreeNode*					m_parent;
	TData							m_data;

	TVec3							m_center;
	f32								m_radius;

	static const u32				k_Data_Threshold = 16;
};

template <class DATA> 
class COctree
{
public:
	// ----------------------------------------------------------------------//
	COctree()
	{
		m_root = nullptr;
	}

	~COctree()
	{
		SIM_SAFE_DELETE( m_root );
	}

	// ----------------------------------------------------------------------//
	void Insert( DATA data )
	{
		if ( !m_root )
		{
			m_root = CreateNode();

			m_root->m_radius = Vec3Max( data->GetBox() );
			Vec3Copy( &m_root->m_center, data->GetCenter() );

			m_root->m_data.Insert( data->GetID(), data );

			return;
		}

		while ( u32 axis = m_root->IsOutside( data ) )
			Expand( data, axis );

		CNode* subRoot = m_root;

		if ( subRoot->m_data.GetCount() > CNode::k_Data_Threshold ) 
			subRoot->Split();

		Shrink();
	}
	// ----------------------------------------------------------------------//
	void Delete( DATA data )
	{
		
	}
	// ----------------------------------------------------------------------//
protected:
	// ----------------------------------------------------------------------//
	void Expand( DATA data, u32 axis )
	{
		CNode* newRoot = CreateNode();

		newRoot->m_radius = m_root->m_radius * 2.0f;
		m_root = newRoot;
	}
	// ----------------------------------------------------------------------//
	void Shrink()
	{
		s32 count = 0, full = 0;

		for ( u32 k = 0; k < 8; k++ )
		{
			if ( m_root->m_children[ k ]->m_data.m_root )
			{
				full = k;
				++count;
			}
		}

		if ( count == 1 )
		{
			for ( u32 k = 0; k < 8; k++ )
				if ( k != full )
					SIM_SAFE_DELETE( m_root->m_children[ k ] );

			m_root = m_root->m_children[ full ];
		}
		else
		{
			if ( count == 0 )
				SIM_SAFE_DELETE( m_root );
		}
	}
	// ----------------------------------------------------------------------//
protected:
	// ----------------------------------------------------------------------//
	CNode*			m_root;
	// ----------------------------------------------------------------------//

	virtual CNode* CreateNode()
	{
		return SIM_NEW CNode();
	}

// ----------------------------------------------------------------------//
};

#undef CTree
#undef CNode

// ----------------------------------------------------------------------//
} // namespace stl;
} // namespace sim;

#endif // __SIM_OCTREE_H