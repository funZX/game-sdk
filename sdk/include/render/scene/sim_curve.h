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

#ifndef __SIM_CURVE_H
#define __SIM_CURVE_H

#include <core/sim_core.h>

#include <render/scene/sim_scene_node.h>
#include <render/sim_render.h>
#include <render/sim_driver.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CDriver;

class CCurve : public CSceneNode
{
public:
    CCurve();
    CCurve( const std::string &name );
	virtual ~CCurve();

    typedef union Vertex {
        struct {
            Vec3 p, i, o;
        };
        Vec3 col[3];
        f32  e[9];
    } Vertex;

    enum class Type : u32
    {
        Bezier,
        Nurbs
    };
	// ------------------------------------------------------------------//
    void                                AddVertex(Vertex v);

    inline bool                         GetIsClosed();
    inline void                         SetIsClosed(bool closed);

    inline Type                         GetType();
    inline void                         SetType(Type type);

    virtual void						Update(f32 dt, void* userData);
	virtual void						Render( CDriver *driver );

    virtual bool	                    Load( io::CMemStream* ms );
    virtual bool	                    Save( io::CMemStream* ms );

	// ------------------------------------------------------------------//

protected:
	// ------------------------------------------------------------------//
    bool                                m_isClosed;
    Type                                m_type;
    sim::stl::CList<Vertex>             m_vertices;
    // ------------------------------------------------------------------//
};
inline bool CCurve::GetIsClosed()
{
    return m_isClosed;
}

inline void CCurve::SetIsClosed(bool closed)
{
    m_isClosed = closed;
}

inline CCurve::Type CCurve::GetType()
{
    return m_type;
}

inline void CCurve::SetType(CCurve::Type type)
{
    m_type = type;
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_CURVE_H
