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

class CSceneNode: public stl::COctreeVolume, public IUpdatable, public IRenderable, public IEngineItem
{
public:
	// ------------------------------------------------------------------//
        // ------------------------------------------------------------------//
    enum class Shape : u32
    {
        Box,
        Sphere,
        Cylinder,
        Cone,
    };
    // ------------------------------------------------------------------//
    enum class Type : u32
    {
        Default,
        NotCulled,
        Billboard,
    };
    // ------------------------------------------------------------------//
	typedef struct
	{
		TVec3				translation;
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
    typedef struct
    {
        Shape					shape;
        Type					type;
        bool					isVisible;
        bool					isCulled;
        bool					isPhysic;
        f32						mass;
        f32						restitution;
        f32						friction;
        TVec3					inertia;

    } TState;

	// ------------------------------------------------------------------//
	typedef stl::CBalanceTree<u32, CSceneNode*>			TChildren;
	// ------------------------------------------------------------------//

	CSceneNode();
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

	const u32						GetID() { return m_iD; }

	inline void						SetTranslation( f32 x, f32 y, f32 z )
									{ Vec3Set( &m_transform.translation, x, y, z ); };

	inline TVec3*					GetTranslation()					{ return &m_transform.translation; };

	inline void						SetRotation( f32 x, f32 y, f32 z )
									{ Vec3Set( &m_transform.rotation, x, y, z ); };

	inline TVec3*					GetRotation()						{ return &m_transform.rotation; };

	inline void						SetScale( f32 x, f32 y, f32 z )
									{ Vec3Set( &m_transform.scale, x, y, z ); }

	inline TVec3*					GetScale()							{ return &m_transform.scale; };

	inline TMatrix4*				GetOrientationMatrix()				{ return &m_transform.matrix.orientation; }
	inline TMatrix4*				GetWorldMatrix()					{ return &m_transform.matrix.world; }

    inline void					    SetShape(Shape shape) { m_state.shape = shape; }
    inline Shape				    GetShape() { return m_state.shape; }

    inline void					    SetVisible(bool vis) { m_state.isVisible = vis; }

    inline bool					    IsVisible() { return m_state.isVisible; }
    inline bool					    IsCulled() { return m_state.isCulled; }
    inline bool					    IsPhysic() { return m_state.isPhysic; }

	// ------------------------------------------------------------------//

#if SIM_DEBUG
	virtual void					Print();
#endif

	// ------------------------------------------------------------------//

protected:
    void							BindOrientationMatrix();
    void							BindWorldMatrix();

	virtual void					OnResize();
	virtual void					OnMove();
	// ------------------------------------------------------------------//
	u32								m_iD;

	CSceneNode*						m_parent;
	TChildren*						m_children;

	TTransform						m_transform;

    TState					        m_state;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_SCENE_H
