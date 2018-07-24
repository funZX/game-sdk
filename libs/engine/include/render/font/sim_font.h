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

#ifndef __SIM_FONT_H_
#define __SIM_FONT_H_

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

	s32							DrawString( CDriver* driver, s32 x, s32 y, const std::string &text );

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
