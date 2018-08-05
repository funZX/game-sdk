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

#ifndef __SIM_ACTOR_H
#define __SIM_ACTOR_H

#include <render/sim_render.h>
#include <render/scene/sim_scene_node.h>

class btCollisionShape;
class btRigidBody;

namespace sim
{
// ----------------------------------------------------------------------//
namespace phy { class CPhysic; };
// ----------------------------------------------------------------------//

namespace rnr
{
// ----------------------------------------------------------------------//

class CMaterial;
class CMesh;
class CTexture;
class CEffect;
class CCamera;

class CActor : public CSceneNode
{
public:
	CActor();
	CActor( const std::string &name );
	virtual ~CActor();

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
		Shape					shape;
		Type					type;
		bool					isVisible;
		bool					isCulled;
		bool					isPhysic;

	} TProperties;
	// ------------------------------------------------------------------//

	void						BindSize();

	void						AddPhysic(phy::CPhysic* physic);
	void						DelPhysic(phy::CPhysic* physic);

	inline void					SetVisible( bool vis)			{ m_properties.isVisible = vis; }

	inline bool					IsVisible()						{ return m_properties.isVisible; }
	inline bool					IsCulled()						{ return m_properties.isCulled; }
	inline bool					IsPhysic()						{ return m_properties.isPhysic; }

	TProperties*				GetProperties()					{ return &m_properties; }

	void						Update( f32 dt, void *userData  );
	void						Render( CDriver *driver );

	inline void					SetShape( Shape shape )			{ m_properties.shape = shape; }
	inline Shape				GetShape()						{ return m_properties.shape; }

	void						SetMesh( CMesh *mesh );
	inline CMesh*				GetMesh()						{ return m_mesh; }

	inline btRigidBody*			GetRigidBody()					{ return m_rigidBody; }
	inline btCollisionShape*	GetCollisionShape()				{ return m_collisionShape; }
	// ------------------------------------------------------------------//

protected:
	CMesh*						m_mesh;
	TProperties					m_properties;

	btCollisionShape*			m_collisionShape;
	btRigidBody*				m_rigidBody;

	f32							m_mass;
	f32							m_restitution;
	f32							m_friction;
	TVec3						m_inertia;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_ACTOR_H
