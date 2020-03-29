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
#include <core/io/sim_mem_stream.h>

#include <render/scene/sim_curve.h>
#include <render/sim_driver.h>

namespace sim
{
namespace ren
{
// ----------------------------------------------------------------------//

CCurve::CCurve()
	: CSceneNode()
{
    m_isClosed = false;
}

// ----------------------------------------------------------------------//

CCurve::CCurve( const std::string &name )
	: CCurve()
{
	m_name = name;
}

// ----------------------------------------------------------------------//

CCurve::~CCurve()
{

}

// ----------------------------------------------------------------------//

void CCurve::AddVertex(Vertex v)
{
    m_vertices.AddToEnd(v);
}

// ----------------------------------------------------------------------//

void CCurve::Update( f32 dt, void *userData )
{
    //zpl_vec2_lerp

    CSceneNode::Update(dt, userData);
}

// ----------------------------------------------------------------------//

void CCurve::Render( CDriver *driver )
{

}

// ----------------------------------------------------------------------//
bool CCurve::Load(io::CMemStream* ms)
{
    Vertex v;
    u32 count = 0;

    m_isClosed = 1 == ms->ReadU8();
    m_type  = (Type)(ms->ReadU8());
    count   = ms->ReadU16();

    for(u32 k = 0; k < count; k++)
    {
        v = *((Vertex*)ms->Read(sizeof(Vertex)));
        m_vertices.AddToEnd( v );
    }

    return true;
}
// ----------------------------------------------------------------------//
bool CCurve::Save(io::CMemStream* ms)
{
    Vertex* v = nullptr;

    ms->WriteU8(m_isClosed ? 1 : 0);
    ms->WriteU8((u8)Value(m_type));
    ms->WriteU16(m_vertices.Count());
    
    while (v = m_vertices.Next())
        ms->Write(v, sizeof(Vertex));

    return true;
}
// ----------------------------------------------------------------------//
}; // namespace ren
}; // namespace sim
