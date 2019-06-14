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

#ifndef __SIM_CAMERA_H
#define __SIM_CAMERA_H

#include <core/sim_core.h>

#include <render/scene/sim_scene_node.h>
#include <render/sim_render.h>


namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CRect2D;
class CDriver;
class CShader;

class CCamera : public CSceneNode
{
public:
	// ------------------------------------------------------------------//
	CCamera();
	CCamera( const std::string &name );
	virtual ~CCamera();
	// ------------------------------------------------------------------//
	void						SetPerspective( CRect2D *canvas );
	void						SetOrthographic( CRect2D *canvas );

    inline Mat4*                GetPerspectiveMatrix();
    inline Mat4*                GetOrthographicMatrix();

    inline void					SetFieldOfView(f32 fov);
    inline f32					GetFieldOfView();

    inline void					SetNearPlane(f32 nearPlane);
    inline f32					GetNearPlane();

    inline void					SetFarPlane(f32 farPlane);
    inline f32					GetFarPlane();

	void						ExtractClipPlanes();

	bool						SphereIn( Vec3 *pos, const f32 rad );
	bool						PointIn( Vec3 *pos );
	bool						BoxIn( Vec3 *pos, Vec3 *bounds );

	virtual void				Update( f32 dt, void *userData );
	virtual void				Render( CDriver *driver );

    virtual bool	            Load(io::CMemStream* ms);
    virtual bool	            Save(io::CMemStream* ms);
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	Plane						m_leftClipPlane;
	Plane						m_rightClipPlane;
	Plane						m_topClipPlane;
	Plane						m_bottomClipPlane;
	Plane						m_nearClipPlane;
	Plane						m_farClipPlane;

	Mat4					    m_perspectiveMatrix;
	Mat4					    m_orthographicMatrix;

	f32							m_fieldOfView;
	f32							m_nearPlane;
	f32							m_farPlane;
	// ------------------------------------------------------------------//
};

inline Mat4* CCamera::GetPerspectiveMatrix() 
{ 
    return &m_perspectiveMatrix; 
}

inline Mat4* CCamera::GetOrthographicMatrix()
{ 
    return &m_orthographicMatrix; 
}

inline void	 CCamera::SetFieldOfView(f32 fov) 
{ 
    m_fieldOfView = fov;
}

inline f32	 CCamera::GetFieldOfView() 
{ 
    return m_fieldOfView; 
}

inline void	 CCamera::SetNearPlane(f32 nearPlane) 
{ 
    m_nearPlane = nearPlane;
}

inline f32	 CCamera::GetNearPlane()
{
    return m_nearPlane; 
}

inline void	 CCamera::SetFarPlane(f32 farPlane)
{ 
    m_farPlane = farPlane;
}

inline f32	 CCamera::GetFarPlane()
{
    return m_farPlane; 
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_CAMERA_H
