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
	m_mesh			        = nullptr;

	m_collisionShape			= nullptr;
	m_rigidBody					= nullptr;
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
	if (IsPhysic())
		return;

	switch (Value(m_state.shape))
	{
	case Shape::Box:
	{
		m_collisionShape = SIM_NEW btBoxShape(btVector3(m_box.x, m_box.y, m_box.z));
		break;
	}

	case Shape::Sphere:
	{
		m_collisionShape = SIM_NEW btSphereShape(m_radius);
		break;
	}

	case Shape::Cylinder:
	{
		m_collisionShape = SIM_NEW btCylinderShape(btVector3(m_box.x, m_box.y, m_box.z));
		break;
	}

	case Shape::Cone:
	{
		m_collisionShape = SIM_NEW btConeShape(m_radius, m_box.y);
		break;
	}
	}

	btScalar mass(m_state.mass);
	btScalar restitution(m_state.restitution);
	btScalar friction(m_state.friction);
	btVector3 inertia(0.0f, 0.0f, 0.0f);

	if (mass != 0.0f)
	{
		m_collisionShape->calculateLocalInertia(mass, inertia);

        m_state.inertia.x = inertia.getX();
        m_state.inertia.y = inertia.getY();
        m_state.inertia.z = inertia.getZ();
	}

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));

	btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, m_collisionShape, inertia);

	m_rigidBody = SIM_NEW btRigidBody(rbInfo);

	physic->Add(this);

	m_state.isPhysic = true;
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
		m_state.isCulled = !camera->SphereIn( &m_transform.translation, m_radius );

		if( !m_state.isCulled )
			m_state.isCulled = !camera->BoxIn( &m_transform.translation, &m_box, &m_transform.matrix.orientation );
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
	switch(Value(m_state.type))
	{
	case Type::Default:
		driver->EnableCulling( true );
		break;

	case Type::NotCulled:
		driver->EnableCulling( false );
		break;

	case Type::Billboard:
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
