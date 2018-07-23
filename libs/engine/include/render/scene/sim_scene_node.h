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

#ifndef __SIM_SCENE_NODE_H
#define __SIM_SCENE_NODE_H

#include <math/sim_matrix4.h>
#include <math/sim_vec3.h>

#include <core/sim_interfaces.h>
#include <core/sim_balance_tree.h>
#include <core/sim_octree.h>

using namespace sim::mat;

namespace sim
{
// ----------------------------------------------------------------------//
namespace rnr
{
// ----------------------------------------------------------------------//

class CSceneNode: public stl::COctreeData<CSceneNode*>, public IUpdatable, public IRenderable
{
public:
	// ------------------------------------------------------------------//
	
	typedef struct
	{
		TVec3				position;
		TVec3				rotation;
		TVec3				scale;

		TQuat				quaternion;

		struct
		{
			TVec3			side;
			TVec3			up;
			TVec3			direction;
		} axis;

		struct
		{
			TMatrix4		orientation;
			TMatrix4		world;

		} matrix;
	
	} TTransform;

	// ------------------------------------------------------------------//

	typedef stl::CBinaryTreeNode<u32, CSceneNode*>		TChild;
	typedef stl::CBalanceTree<u32, CSceneNode*>			TChildren;

	CSceneNode( const std::string &name );
	virtual ~CSceneNode();

	// ------------------------------------------------------------------//
	virtual void 					Update( f32 dt, void *userData );
	virtual void 					Render( CDriver *driver );

	CSceneNode*						GetParent()	{ return m_parent; }
	void							SetParent( CSceneNode* parent )	{ m_parent = parent; }

	void							AddChild( CSceneNode* child );
	void							DelChild( CSceneNode* child );

	TChildren*						GetChildren()	{ return m_children; }

	CSceneNode*						GetChild( const std::string& name );

	const std::string&				GetName()	{ return m_name; }
	const u32						GetID()		{ return m_iD; }

	inline void						SetPosition( f32 x, f32 y, f32 z )
									{ Vec3Set( &m_transform.position, x, y, z ); };

	inline TVec3*					GetPosition()						{ return &m_transform.position; };

	inline void						SetRotation( f32 x, f32 y, f32 z )
									{ Vec3Set( &m_transform.rotation, x, y, z ); };

	inline TVec3*					GetRotation()						{ return &m_transform.rotation; };

	inline void						SetScale( f32 x, f32 y, f32 z )
									{ Vec3Set( &m_transform.scale, x, y, z ); }

	inline TVec3*					GetScale()							{ return &m_transform.scale; };

	void							BindOrientationMatrix();
	void							BindWorldMatrix();

	inline TMatrix4*				GetOrientationMatrix()				{ return &m_transform.matrix.orientation; }
	inline TMatrix4*				GetWorldMatrix()					{ return &m_transform.matrix.world; }

	// ------------------------------------------------------------------//

#if SIM_DEBUG
	virtual void					Print();
#endif

	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	std::string						m_name;
	u32								m_iD;

	CSceneNode*						m_parent;
	TChildren*						m_children;

	TTransform						m_transform;

	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_SCENE_H
