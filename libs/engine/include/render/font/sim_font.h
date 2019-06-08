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

struct FT_FaceRec_;

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CFontChar;
class CFontAtlas;
class CEffect;
class CDriver;

class CFont : public IEngineItem
{
public:
	CFont( CFontAtlas* pFontAtlas );
	CFont( const std::string& name, CFontAtlas* pFontAtlas );
	virtual ~CFont();
	// ------------------------------------------------------------------//
	void						Load(const std::string& fileName);

	s32							DrawString( CDriver* driver, s32 x, s32 y, const std::string &text, Vec4 color);

	s32							GetWidth( const char* text );

	s32							GetHeight() const { return m_height; }
	void						SetHeight( s32 height ) { m_height = height; }

	void						Release();
	void						Init();

	s32							GetCharWidth( s32 charCode );

	void						AddChar( s32 charCode, CFontChar* fontChar ) { m_mapBitmapChar[charCode] = fontChar; }
	void						SetFTFace( struct FT_FaceRec_* ftFace ) { m_ftFace = ftFace; }
	void						FinishCreating();
	CFontChar*					GetChar(s32 charCode);
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	void						ReleaseFace();

	s32							m_height;
	std::map<s32, CFontChar*>	m_mapBitmapChar;
	CFontAtlas*					m_fontAtlas;
	struct FT_FaceRec_*			m_ftFace;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_FONT_H_
