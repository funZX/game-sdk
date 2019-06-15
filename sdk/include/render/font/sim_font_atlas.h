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

struct FT_LibraryRec_;
struct FT_FaceRec_;

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CFontChar;
class CFont;
class CFontNode;
class CMaterial;
class CTexture;
class CEffect;
class CBatch2D;

class CFontAtlas : public IEngineItem
{
public:
	CFontAtlas();
	CFontAtlas( const std::string &name );
	virtual ~CFontAtlas();
	// ------------------------------------------------------------------//
	CFont*								AddFont( const std::string &name, u8* memfile, s32 memsize, s32 size, const char* szLetters );
	void								Create();

	CMaterial*							GetMaterial()						{ return m_material; }
	CFont*								GetFont(s32 index)					{ return m_listBitmapFont[index]; }
	bool								GetUseKerning() const				{ return m_useKerning; }
	void								SetUseKerning( bool useKerning )	{ m_useKerning = useKerning; }
	
    virtual bool					    Load(io::CMemStream* ms);
    virtual bool					    Save(io::CMemStream* ms);
    // ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	void								InitEffect();
	void								InitMaterial();
	bool								BinPack(stl::CPool<CFontNode>* pool, s32 width, s32 height );
	void								ReleaseLibrary();
	// ------------------------------------------------------------------//

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
