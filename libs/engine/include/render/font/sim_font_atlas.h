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

#ifndef __SIM_FONT_ATLAS_H
#define __SIM_FONT_ATLAS_H

#include <render/sim_render.h>

struct FT_LibraryRec_;
struct FT_FaceRec_;

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CFontChar;
class CFont;
class CMaterial;
class CTexture;
class CEffect;
class CBatch2D;

class CFontAtlas
{
public:
	CFontAtlas( const std::string &name );
	virtual ~CFontAtlas();
	// ------------------------------------------------------------------//
	CFont*								AddFont( const std::string &name, u8* memfile, s32 memsize, s32 size, const char* szLetters );
	void								Create();

	CMaterial*							GetMaterial()						{ return m_material; }
	CFont*								GetFont(s32 index)					{ return m_listBitmapFont[index]; }
	bool								GetUseKerning() const				{ return m_useKerning; }
	void								SetUseKerning( bool useKerning )	{ m_useKerning = useKerning; }
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	void								InitEffect();
	void								InitMaterial();
	bool								BinPack( s32 width, s32 height );
	void								ReleaseLibrary();
	// ------------------------------------------------------------------//

	std::string							m_name;

	CMaterial*							m_material;
	CEffect*							m_effect;
	CTexture*							m_texture;

	struct FT_LibraryRec_*				m_library;
	std::vector<CFontChar*>				m_listBitmapChar;
	std::vector<CFont*>					m_listBitmapFont;
	std::vector<struct FT_FaceRec_*> 	m_listFace;
	bool								m_useKerning;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_FONT_ATLAS_H
