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

#include <render/scene/sim_scene_node.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CSceneNode::CSceneNode()
	: COctreeVolume()
{
	m_iD		= id::Get();

	m_parent	= nullptr;
	m_children	= SIM_NEW TChildren();

	m_radius    = 1.0f;

	Vec3ToZero( &m_box );
	Vec3ToZero( &m_center );

	Vec3ToZero( &m_transform.translation );
	Vec3ToZero( &m_transform.rotation );
	Vec3ToOne( &m_transform.scale );

	Vec3ToDirX( &m_transform.axis.side );
	Vec3ToDirY( &m_transform.axis.up );
	Vec3ToDirZ( &m_transform.axis.direction );

	QuatReset( &m_transform.quaternion);
	Matrix4ToIdentity( &m_transform.matrix.orientation );
	Matrix4ToIdentity( &m_transform.matrix.world );

    m_state.shape = Shape::Box;
    m_state.type = Type::Default;
    m_state.isVisible = true;
    m_state.isCulled = false;
    m_state.isPhysic = false;
    m_state.mass = 0.0f;
    m_state.restitution = 0.0f;
    m_state.friction = 0.0f;
    Vec3ToZero(&m_state.inertia);
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
    //Matrix4FromQuat( &m_orientationMatrix, &m_quaternion );
    BindOrientationMatrix();
    BindWorldMatrix();

    Matrix4GetSide(&m_transform.matrix.orientation, &m_transform.axis.side);
    Matrix4GetUp(&m_transform.matrix.orientation, &m_transform.axis.up);
    Matrix4GetFront(&m_transform.matrix.orientation, &m_transform.axis.direction);
    //QuatGetDir( &m_quaternion, &m_direction );
}

// ----------------------------------------------------------------------//

void CSceneNode::Render( CDriver *driver )
{

}

// ----------------------------------------------------------------------//

void CSceneNode::BindOrientationMatrix()
{
	Matrix4ToIdentity( &m_transform.matrix.orientation );

	if( m_transform.rotation.y != 0.0f ) {
		Matrix4RotateY( &m_transform.matrix.orientation, m_transform.rotation.y );
	}

	if( m_transform.rotation.x != 0.0f ) {
		Matrix4RotateX( &m_transform.matrix.orientation, m_transform.rotation.x );
	}

	if( m_transform.rotation.z != 0.0f ) {
		Matrix4RotateZ( &m_transform.matrix.orientation, m_transform.rotation.z );
	}
}

// ----------------------------------------------------------------------//

void CSceneNode::BindWorldMatrix()
{
	Matrix4ToTranslate( &m_transform.matrix.world, &m_transform.translation );
	Matrix4Multiply( &m_transform.matrix.world, &m_transform.matrix.orientation );
	Matrix4Scale( &m_transform.matrix.world, &m_transform.scale );
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
}; // namespace rnr
}; // namespace sim
