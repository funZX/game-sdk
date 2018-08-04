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

	m_radius = 1.0f;
	Vec3ToZero( &m_box );
	Vec3ToZero( &m_center );

	Vec3ToZero( &m_transform.translation );
	Vec3ToZero( &m_transform.rotation );
	Vec3ToOne( &m_transform.scale );

	Vec3ToDirX( &m_transform.axis.side);
	Vec3ToDirY( &m_transform.axis.up);
	Vec3ToDirZ( &m_transform.axis.direction);

	QuatReset( &m_transform.quaternion);
	Matrix4ToIdentity( &m_transform.matrix.orientation );
	Matrix4ToIdentity( &m_transform.matrix.world );
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
	CSceneNode* node = m_children->Delete( child->GetID() );

	if ( node )
		child->SetParent( nullptr );
}

// ----------------------------------------------------------------------//

CSceneNode* CSceneNode::GetChild( const std::string& name )
{
	u32 h = hash::Get( name );

	auto node = m_children->Search( h );

	return node ? node->GetData() : nullptr;
}

// ----------------------------------------------------------------------//

void CSceneNode::Update( f32 dt, void *userData )
{

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
	m_children->Print( m_children->m_root, 0 );
}

const std::string& GetName( CSceneNode* node )
{
	return node->GetName();
}

#endif

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
