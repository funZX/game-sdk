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

#include <render/scene/sim_scene.h>
#include <render/scene/sim_camera.h>
#include <render/scene/sim_scene_node.h>
#include <sim_engine.h>

namespace sim
{
namespace ren
{
// ----------------------------------------------------------------------//

CSceneNode::CSceneNode()
	: COctreeVolume()
{
	m_iD		            = id::Get();

	m_parent	            = nullptr;
	m_children	            = SIM_NEW TChildren();

	m_radius                = 1.0f;

    m_box                   = axis::Origin.xyz;
    m_center                = axis::Origin.xyz;

    m_state.shape           = Shape::Box;
    m_state.type            = Type::Default;
    m_state.isVisible       = true;
    m_state.isCulled        = false;
    m_state.isPhysic        = false;
    m_state.isStatic        = true;
    m_state.mass            = 0.0f;
    m_state.restitution     = 0.0f;
    m_state.friction        = 0.0f;
    m_state.inertia         = axis::Origin.xyz;

    m_transform.translation = axis::Origin.xyz;
    m_transform.scale       = axis::Unit.xyz;
    m_transform.quaternion  = axis::Look;

    zpl_mat4_identity( &m_matrix );
}

// ----------------------------------------------------------------------//

CSceneNode::CSceneNode( const std::string &name )
	: CSceneNode()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CSceneNode::~CSceneNode()
{
	m_children->RemoveAll();

	SIM_SAFE_DELETE( m_children );
}

// ----------------------------------------------------------------------//

void CSceneNode::AddChild( CSceneNode* child )
{
	SIM_ASSERT( child != this );

	CSceneNode* parent = child->GetParent();
	if ( parent )
		parent->DelChild( child );

	m_children->Insert( child->GetID(), child );

	child->SetParent( this );
}

// ----------------------------------------------------------------------//

void CSceneNode::DelChild( CSceneNode* child )
{
	m_children->Delete( child->GetID() );
	child->SetParent( nullptr );
}

// ----------------------------------------------------------------------//

CSceneNode* CSceneNode::GetChild( const std::string& name )
{
	auto node = m_children->Search(hash::Get(name));

	return node ? *(node->m_data) : nullptr;
}

// ----------------------------------------------------------------------//

void CSceneNode::Update( f32 dt, void *userData )
{
    if ( IsVisible() )
    {
        static CEngine* engine = CEngine::GetSingletonPtr();

        CScene* scene   = (CScene*)userData;
        CCamera* camera = engine->GetCamera();

        SIM_ASSERT(camera);

        m_state.isCulled = !camera->SphereIn(&m_transform.translation, m_radius);

        if (!m_state.isCulled)
            m_state.isCulled = !camera->BoxIn(&m_transform.translation, &m_box);
    }

    if ( !IsStatic() )
    {
        zpl_mat4_from_quat(&m_matrix, m_transform.quaternion);

        m_matrix.x.x *= m_transform.scale.x;
        m_matrix.y.y *= m_transform.scale.y;
        m_matrix.z.z *= m_transform.scale.z;

        m_matrix.w.xyz = m_transform.translation;
    }


}

// ----------------------------------------------------------------------//

void CSceneNode::Render( CDriver *driver )
{

}

// ----------------------------------------------------------------------//

void CSceneNode::OnResize()
{
	OctreeUpdateSignal.Emit();
}

// ----------------------------------------------------------------------//

void CSceneNode::OnMove()
{
	OctreeUpdateSignal.Emit();
}

// ----------------------------------------------------------------------//

#if SIM_DEBUG

void CSceneNode::Print()
{
	SIM_PRINT("\n\n");
	m_children->Print( m_children->GetRoot(), 0 );
}

#endif

// ----------------------------------------------------------------------//
}; // namespace ren
}; // namespace sim
