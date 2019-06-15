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

#ifndef __SIM_FONT_H_
#define __SIM_FONT_H_

#include <core/sim_core.h>
#include <core/sim_interfaces.h>

#include <render/sim_render.h>
#include <imgui.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CDriver;

class CFont : public IEngineItem
{
public:
    friend class CFontAtlas;

    CFont( CFontAtlas* atlas );
	CFont( const std::string& name, CFontAtlas* atlas);
	virtual ~CFont();
	// ------------------------------------------------------------------//
	void						DrawString( CDriver* driver, s32 x, s32 y, const std::string &text, Vec4 color);

    inline void                 SetPixelSize( f32 pixelSize );
    inline f32                  GetPixelSize();

    virtual bool				Load( io::CMemStream* ms );
    virtual bool				Save( io::CMemStream* ms );

	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
    f32                         m_pixelSize;
    ImFont*                     m_imFont;

    CFontAtlas*                 m_fontAtlas;


	// ------------------------------------------------------------------//
};

inline void CFont::SetPixelSize( f32 pixelSize)
{
    m_pixelSize = pixelSize;
}

inline f32 CFont::GetPixelSize()
{
    return m_pixelSize;
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_FONT_H_
