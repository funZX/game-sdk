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

#ifndef __SIM_FILESYSTEM_H
#define __SIM_FILESYSTEM_H

#include <jansson.h>
#include <sim_engine.h>
#include <core/sim_balance_tree.h>

namespace sim
{
// ----------------------------------------------------------------------//
namespace rnr 
{
	class CDriver;
	class CFont;
	class CFontAtlas;
	class CTexture;
	class CSkyBox;
	class CEffect;
	class CMaterial;
	class CMesh;
	class CActor;
	class CLight;
	class CCamera;
	class CSpriteTexture;
};
// ----------------------------------------------------------------------//
namespace snd { class CSoundData; };
// ----------------------------------------------------------------------//
namespace vm { class CScript; class CSquirrel; };
// ----------------------------------------------------------------------//
namespace io
{ 
class CLzmaStream; 
class CJsonStream;
// ----------------------------------------------------------------------//

class CFileSystem
{
	friend class rnr::CScene;

	typedef stl::CBalanceTree<u32, rnr::CFont*>						TFontList;
	typedef stl::CBalanceTree<u32, rnr::CTexture*>					TTextureList;
	typedef stl::CBalanceTree<u32, rnr::CSkyBox*>					TSkyboxList;
	typedef stl::CBalanceTree<u32, rnr::CEffect*>					TEffectList;
	typedef stl::CBalanceTree<u32, rnr::CMaterial*>					TMaterialList;
	typedef stl::CBalanceTree<u32, rnr::CMesh*>						TMeshList;
	typedef stl::CBalanceTree<u32, rnr::CActor*>					TActorList;
	typedef stl::CBalanceTree<u32, rnr::CLight*>					TLightList;
	typedef stl::CBalanceTree<u32, rnr::CCamera*>					TCameraList;
	typedef stl::CBalanceTree<u32, rnr::CSpriteTexture*>			TSpriteList;
	typedef stl::CBalanceTree<u32, snd::CSoundData*>				TSoundList;
	typedef stl::CBalanceTree<u32, vm::CScript*>					TScriptList;
	typedef stl::CBalanceTree<u32, rnr::CScene*>					TSceneList;

public:

	CFileSystem( const std::string &filename );
	virtual ~CFileSystem();

	// ------------------------------------------------------------------//
	void						Open();
	bool						LoadNext();
	void						Close();

	rnr::CFont*					GetFont( const std::string &name );
	rnr::CTexture*				GetTexture( const std::string &name );
	rnr::CSkyBox*				GetSkybox( const std::string &name );
	rnr::CEffect*				GetEffect( const std::string &name );
	rnr::CMaterial*				GetMaterial( const std::string &name );
	rnr::CMesh*					GetMesh( const std::string &name );
	rnr::CActor*				GetActor( const std::string &name );
	rnr::CLight*				GetLight( const std::string &name );
	rnr::CCamera*				GetCamera( const std::string &name );
	rnr::CSpriteTexture*		GetSprite( const std::string &name );
	snd::CSoundData*			GetSound( const std::string &name );
	vm::CScript*				GetScript( const std::string &name );
	rnr::CScene*				GetScene( const std::string &name );

	inline rnr::CFontAtlas*		GetFontAtlas() { return m_fontAtlas; }
	inline const std::string&	GetLoadMessage() { return m_loadMessage; }
	// ------------------------------------------------------------------//

protected:
	u8*							LoadFile( const std::string& filename );
	void						UnloadFile(u8* memfile);

	typedef bool (CFileSystem::*TLoadFunction)(const json_t* jsonRoot, s32 index);

	typedef struct
	{
		std::string				name;
		s32						index;
		CJsonStream*			json;

		TLoadFunction			LoadFn;

		void Init( const std::string& n, TLoadFunction loadFn)
		{
			name	= n;
			index	= 0;
			json	= nullptr;
			LoadFn	= loadFn;
		}

	} TLoadStep;

	// ------------------------------------------------------------------//
	bool						LoadInit(const json_t* jsonRoot, s32 index);

	bool						LoadFont(const json_t* jsonRoot, s32 index);
	bool						LoadTexture(const json_t* jsonRoot, s32 index);
	bool						LoadSkybox(const json_t* jsonRoot, s32 index);
	bool						LoadEffect(const json_t* jsonRoot, s32 index);
	bool						LoadMaterial(const json_t* jsonRoot, s32 index);
	bool						LoadMesh(const json_t* jsonRoot, s32 index);
	bool						LoadActor(const json_t* jsonRoot, s32 index);
	bool						LoadLight(const json_t* jsonRoot, s32 index);
	bool						LoadCamera(const json_t* jsonRoot, s32 index);
	bool						LoadSprite(const json_t* jsonRoot, s32 index);
	bool						LoadSound(const json_t* jsonRoot, s32 index);
	bool						LoadScript(const json_t* jsonRoot, s32 index);
	bool						LoadScene(const json_t* jsonRoot, s32 index);

	bool						NextStep(const json_t* jsonRoot);

	void						UnloadFonts();
	void						UnloadTextures();
	void						UnloadSkyboxes();
	void						UnloadEffects();
	void						UnloadMaterials();
	void						UnloadMeshes();
	void						UnloadActors();
	void						UnloadLights();
	void						UnloadCameras();
	void						UnloadSprites();
	void						UnloadSounds();
	void						UnloadScripts();
	void						UnloadScenes();

	// ------------------------------------------------------------------//
protected:
	std::string					m_filename;

	io::CLzmaStream*			m_lzmaStream;

	u8*							m_buffer;
	s32							m_bufferSize;

	rnr::CDriver*				m_driver;
	rnr::CFontAtlas*			m_fontAtlas;

	vm::CSquirrel*				m_squirrel;

	TFontList					m_fontList;
	TTextureList				m_textureList;
	TSkyboxList					m_skyboxList;
	TEffectList					m_effectList;
	TMaterialList				m_materialList;
	TMeshList					m_meshList;
	TActorList					m_actorList;
	TLightList					m_lightList;
	TCameraList					m_cameraList;
	TSpriteList					m_spriteList;
	TSoundList					m_soundList;
	TScriptList					m_scriptList;
	TSceneList					m_sceneList;

	TLoadStep*					m_steps;
	TLoadStep*					m_crtStep;
	TLoadStep*					m_lastStep;

	std::string					m_loadMessage;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace io
}; // namespace sim

#endif // __SIM_FILESYSTEM_H
