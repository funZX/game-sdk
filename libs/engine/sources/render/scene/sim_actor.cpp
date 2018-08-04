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

#include <btBulletDynamicsCommon.h>

#include <render/scene/sim_camera.h>
#include <render/scene/sim_actor.h>

#include <render/sim_driver.h>
#include <render/sim_mesh.h>
#include <render/sim_material.h>
#include <render/sim_effect.h>

#include <physic/sim_physic.h>

#if SIM_DEBUG
#	include		<render/sim_glaux.h>
#endif // SIM_DEBUG

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CActor::CActor()
	: CSceneNode()
{
	m_mesh			            = nullptr;

	m_properties.shape			= k_Shape_Box;
	m_properties.type			= k_Type_Default;
	m_properties.isVisible		= true;
	m_properties.isCulled		= false;
	m_properties.isPhysic		= false;

	m_collisionShape			= nullptr;
	m_rigidBody					= nullptr;

	m_mass						= 0.0f;
	m_restitution				= 0.0f;
	m_friction					= 0.0f;
	Vec3ToZero( &m_inertia );
}
// ----------------------------------------------------------------------//

CActor::CActor( const std::string &name )
	: CActor()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CActor::~CActor()
{
	SIM_SAFE_DELETE(m_collisionShape);
}

// ----------------------------------------------------------------------//

void CActor::SetMesh( CMesh *mesh )
{
	m_mesh = mesh;
	BindSize();
}

// ----------------------------------------------------------------------//

void CActor::BindSize()
{
	m_radius = m_mesh->GetRadius();

	Vec3Copy( &m_center, m_mesh->GetCenter() );
	Vec3Copy( &m_box, m_mesh->GetBox() );
	Vec3Multiply( &m_box, &m_transform.scale );

	m_radius *= Vec3Max( &m_transform.scale );

	CSceneNode::OnResize();
}

// ----------------------------------------------------------------------//

void CActor::AddPhysic(phy::CPhysic *physic)
{
	if (!IsPhysic())
	{
		switch( m_properties.shape )
		{
		case k_Shape_Box:
			{
				m_collisionShape = SIM_NEW btBoxShape(btVector3(m_box.x, m_box.y, m_box.z));
				break;
			}

		case k_Shape_Sphere:
			{
				m_collisionShape = SIM_NEW btSphereShape(m_radius);
				break;
			}

		case k_Shape_Cylinder:
			{
				m_collisionShape = SIM_NEW btCylinderShape(btVector3(m_box.x, m_box.y, m_box.z));
				break;
			}

		case k_Shape_Cone:
			{
				m_collisionShape = SIM_NEW btConeShape(m_radius, m_box.y);
				break;
			}
		}

		btScalar mass(m_mass);
		btScalar restitution(m_restitution);
		btScalar friction(m_friction);
		btVector3 inertia(0.0f, 0.0f, 0.0f);

		if (mass != 0.0f)
		{
			m_collisionShape->calculateLocalInertia(mass, inertia);

			m_inertia.x = inertia.getX();
			m_inertia.y = inertia.getY();
			m_inertia.z = inertia.getZ();
		}

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin( btVector3(0.0f, 0.0f, 0.0f) );

		btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, m_collisionShape, inertia);

		m_rigidBody = SIM_NEW btRigidBody(rbInfo);

		physic->Add(this);

		m_properties.isPhysic = true;
	}
}

// ----------------------------------------------------------------------//

void CActor::DelPhysic(phy::CPhysic *physic)
{
	if (IsPhysic())
		physic->Del(this);
}

// ----------------------------------------------------------------------//

void CActor::Update( f32 dt, void *userData )
{
	if ( IsVisible() )
	{
		CCamera *camera = (CCamera*) userData;
		m_properties.isCulled = !camera->SphereIn( &m_transform.translation, m_radius );

		if( !m_properties.isCulled )
			m_properties.isCulled = !camera->BoxIn( &m_transform.translation, &m_box, &m_transform.matrix.orientation );
	}

	if ( IsPhysic() )
	{
		btTransform worldTransform;
		m_rigidBody->getMotionState()->getWorldTransform( worldTransform );
		
		const btVector3& origin = worldTransform.getOrigin();
		const btQuaternion& quat = worldTransform.getRotation();

		Vec3Set( &m_transform.translation, origin.getX(), origin.getY(), origin.getZ());
		QuatSet( &m_transform.quaternion, quat.getX(), quat.getY(), quat.getZ(), quat.getW() );
		QuatGetRot( &m_transform.quaternion, &m_transform.rotation );

		//Matrix4FromQuat( &m_orientationMatrix, &m_quaternion );
		BindOrientationMatrix();
		BindWorldMatrix();

		Matrix4GetSide( &m_transform.matrix.orientation, &m_transform.axis.side );
		Matrix4GetUp( &m_transform.matrix.orientation, &m_transform.axis.up );
		Matrix4GetFront( &m_transform.matrix.orientation, &m_transform.axis.direction );
		//QuatGetDir( &m_quaternion, &m_direction );

		CSceneNode::OnMove();
	}
}

// ----------------------------------------------------------------------//

void CActor::Render( CDriver *driver )
{
	switch( m_properties.type )
	{
	case k_Type_Default:
		driver->EnableCulling( true );
		break;

	case k_Type_NotCulled:
		driver->EnableCulling( false );
		break;

	case k_Type_Billboard:
		break;
	}

	driver->MatrixPush();
	{
		driver->MatrixMultiply( &m_transform.matrix.world );
		m_mesh->Render( driver );
	}
	driver->MatrixPop();
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
