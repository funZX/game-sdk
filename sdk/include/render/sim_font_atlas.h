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

#ifndef __SIM_FONT_ATLAS_H
#define __SIM_FONT_ATLAS_H

#include <core/sim_pool.h>
#include <core/sim_core.h>
#include <core/sim_interfaces.h>
#include <render/sim_render.h>

#include <imgui.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
class CFont;
class CMaterial;
class CTexture;
class CEffect;

class CFontAtlas : public IEngineItem
{
public:
    friend class CCanvas;

	CFontAtlas();
	CFontAtlas( const std::string &name );
	virtual ~CFontAtlas();
	// ------------------------------------------------------------------//
	CFont*								AddFont( const std::string name, io::CMemStream* ms, f32 pixelSize );
	void								Create();

	CMaterial*							GetMaterial()						{ return m_material; }
	
    virtual bool					    Load( io::CMemStream* ms );
    virtual bool					    Save( io::CMemStream* ms );
    // ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	void								InitEffect();
	void								InitMaterial();
	// ------------------------------------------------------------------//

    ImFontAtlas*                        m_imAtlas;

	CMaterial*							m_material;
	CEffect*							m_effect;
	CTexture*							m_texture;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_FONT_ATLAS_H
