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
	typedef stl::CBalanceTree<u32, rnr::CScene*>					TSceneList;
	typedef stl::CBalanceTree<u32, vm::CScript*>					TScriptList;

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

	inline const std::string&	GetLoadMessage() { return m_loadMessage; }

	void						LinkEffect(const char* effect, const char* material);
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
	vm::CSquirrel*				m_vm;
    rnr::CFontAtlas*            m_fontAtlas;

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

	bool						m_isLoaded;
	std::string					m_loadMessage;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace io
}; // namespace sim

#endif // __SIM_FILESYSTEM_H
