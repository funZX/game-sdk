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

#include <core/sim_interfaces.h>
#include <core/sim_balance_tree.h>
#include <core/sim_octree.h>

namespace sim
{
// ----------------------------------------------------------------------//
namespace ren
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
    typedef struct
    {
        Shape		shape;
        bool		isVisible;
        bool		isCulled;
        bool		isPhysic;
        bool        isStatic;
        f32			mass;
        f32			restitution;
        f32			friction;
        Vec3		inertia;

    } TState;

	// ------------------------------------------------------------------//
	typedef stl::CBalanceTree<u32, CSceneNode*>			TChildren;
	// ------------------------------------------------------------------//

	CSceneNode();
	CSceneNode( const std::string &name );
	virtual ~CSceneNode();

	// ------------------------------------------------------------------//
	virtual void 					Update( f32 dt, void *userData );
	void 					        Render( CDriver *driver );

	CSceneNode*						GetParent()	{ return m_parent; }
	void							SetParent( CSceneNode* parent )	{ m_parent = parent; }

	void							AddChild( CSceneNode* child );
	void							DelChild( CSceneNode* child );

	TChildren*						GetChildren()	{ return m_children; }

	CSceneNode*						GetChild( const std::string& name );

    inline const u32				GetID();

    inline Transform*               GetTransform();
    inline Mat4*                    GetMatrix();
    inline TState*                  GetState();

    inline void					    SetShape(Shape shape);
    inline Shape				    GetShape();

    inline void					    SetVisible(bool vis);

    inline bool					    IsVisible();
    inline bool					    IsCulled();
    inline bool					    IsPhysic();
    inline bool					    IsStatic();

	// ------------------------------------------------------------------//

#if SIM_DEBUG
	virtual void					Print();
#endif

    sigcxx::Signal<f32, void*>	    OnUpdate;
    sigcxx::Signal<CDriver*>	    OnRender;
	// ------------------------------------------------------------------//

protected:
	// ------------------------------------------------------------------//
	u32								m_iD;

	CSceneNode*						m_parent;
	TChildren*						m_children;

	Transform						m_transform;
    Mat4                            m_matrix;

    TState					        m_state;
	// ------------------------------------------------------------------//
};


inline const u32 CSceneNode::GetID()
{ 
    return m_iD; 
}

inline Transform* CSceneNode::GetTransform()
{
    return &m_transform;
}

inline Mat4* CSceneNode::GetMatrix()
{
    return &m_matrix;
}

inline CSceneNode::TState* CSceneNode::GetState()
{
    return &m_state;
}

inline void CSceneNode::SetShape(Shape shape) 
{
    m_state.shape = shape;
}

inline CSceneNode::Shape CSceneNode::GetShape()
{ 
    return m_state.shape; 
}

inline void CSceneNode::SetVisible(bool vis)
{ 
    m_state.isVisible = vis; 
}

inline bool CSceneNode::IsVisible() 
{ 
    return m_state.isVisible; 
}

inline bool CSceneNode::IsCulled() 
{ 
    return m_state.isCulled;
}

inline bool CSceneNode::IsPhysic() 
{
    return m_state.isPhysic;
}

inline bool CSceneNode::IsStatic()
{
    return m_state.isStatic;
}
// ----------------------------------------------------------------------//
}; // namespace ren
}; // namespace sim
#endif // __SIM_SCENE_H
