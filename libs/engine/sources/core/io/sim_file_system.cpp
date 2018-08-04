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

#include <core/io/sim_file_system.h>

#include <core/io/sim_lzma_stream.h>
#include <core/io/sim_json_stream.h>
#include <core/io/sim_mem_stream.h>

#include <render/scene/sim_camera.h>
#include <render/scene/sim_sky_box.h>
#include <render/scene/sim_light.h>
#include <render/scene/sim_actor.h>
#include <render/scene/sim_scene.h>

#include <render/sim_driver.h>
#include <render/sim_shader.h>
#include <render/sim_effect.h>
#include <render/sim_mesh.h>
#include <render/sim_texture.h>
#include <render/sim_cube_texture.h>
#include <render/sim_material.h>
#include <render/sim_sprite_texture.h>
#include <render/font/sim_font.h>
#include <render/font/sim_font_atlas.h>

#include <sound/sim_sound_data.h>
#include <vm/sim_script.h>

namespace sim
{
namespace io
{
// ----------------------------------------------------------------------//

CFileSystem::CFileSystem( const std::string &filename )
{
	CEngine* engine = CEngine::GetSingletonPtr();

	m_filename		= filename;
	m_driver		= engine->GetDriver();
	m_squirrel		= engine->GetVM();

	m_lzmaStream	= SIM_NEW io::CLzmaStream(filename);
	m_fontAtlas		= SIM_NEW CFontAtlas(m_filename);

	m_buffer		= nullptr;
	m_bufferSize	= 0;

	m_steps			= nullptr;
	m_crtStep		= nullptr;
}

// ----------------------------------------------------------------------//

CFileSystem::~CFileSystem()
{
	UnloadScenes();
	UnloadScripts();
	UnloadSounds();
	UnloadSprites();
	UnloadCameras();
	UnloadLights();
	UnloadActors();
	UnloadMeshes();
	UnloadMaterials();
	UnloadEffects();
	UnloadShaders();
	UnloadSkyboxes();
	UnloadTextures();
	UnloadFonts();

	SIM_SAFE_DELETE( m_fontAtlas );
	SIM_SAFE_DELETE( m_lzmaStream );
}

// ----------------------------------------------------------------------//

u8* CFileSystem::LoadFile( const std::string& filename )
{
	u32 offset = 0;
	m_lzmaStream->OpenFile( filename, &m_buffer, &offset, &m_bufferSize );

	u8* data = SIM_NEW u8[ m_bufferSize + 1 ];
	SIM_MEMCPY( data, &m_buffer[ offset ], m_bufferSize );
	data[ m_bufferSize ] = 0;

	m_lzmaStream->CloseFile( &m_buffer );

	return data;
}

// ----------------------------------------------------------------------//

void CFileSystem::UnloadFile( u8* memfile )
{
	SIM_SAFE_DELETE_ARRAY(memfile);
}

// ----------------------------------------------------------------------//

void CFileSystem::Open()
{
	m_steps		= SIM_NEW TLoadStep[ 15];

	m_steps[  0 ].Init("init",		&CFileSystem::LoadInit);
	m_steps[  1 ].Init("font",		&CFileSystem::LoadFont);
	m_steps[  2 ].Init("texture",	&CFileSystem::LoadTexture);
	m_steps[  3 ].Init("skybox",	&CFileSystem::LoadSkybox);
	m_steps[  4 ].Init("shader",	&CFileSystem::LoadShader);
	m_steps[  5 ].Init("effect",	&CFileSystem::LoadEffect);
	m_steps[  6 ].Init("material",	&CFileSystem::LoadMaterial);
	m_steps[  7 ].Init("mesh",		&CFileSystem::LoadMesh);
	m_steps[  8 ].Init("actor",		&CFileSystem::LoadActor);
	m_steps[  9 ].Init("light",		&CFileSystem::LoadLight);
	m_steps[ 10 ].Init("camera",	&CFileSystem::LoadCamera);
	m_steps[ 11 ].Init("sprite",	&CFileSystem::LoadSprite);
	m_steps[ 12 ].Init("sound",		&CFileSystem::LoadSound);
	m_steps[ 13 ].Init("script",	&CFileSystem::LoadScript);
	m_steps[ 14 ].Init("scene",		&CFileSystem::LoadScene);

	m_crtStep	= m_steps;
	m_lastStep  = m_steps + 15;

	m_lzmaStream->Open();

	u8* memfile			= LoadFile("content.json");
	m_crtStep->json		= SIM_NEW CJsonStream(memfile);

	UnloadFile(memfile);
}

// ----------------------------------------------------------------------//

void CFileSystem::Close()
{
	m_lzmaStream->Close();

	m_crtStep = m_steps;

	for ( s32 k = 0; k < ( m_lastStep - m_steps ); k++ )
	{
		SIM_SAFE_DELETE( m_crtStep->json );
		++m_crtStep;
	}

	SIM_SAFE_DELETE_ARRAY( m_steps );

	u32 tex =
	m_driver->BindTexture(0);
	m_fontAtlas->Create();
	m_driver->BindTexture(tex);
}


// ----------------------------------------------------------------------//

bool CFileSystem::NextStep(const json_t* jsonRoot)
{
	return ( ++m_crtStep->index >= (s32)json_array_size( jsonRoot ) );
}

// ----------------------------------------------------------------------//

bool CFileSystem::LoadNext( void )
{
	bool stepDone = true;

	if ( m_crtStep->json )
	{
		json_t* jsonRoot = m_crtStep->json->GetRoot();

		SIM_ASSERT( json_is_array( jsonRoot ) );

		if ( json_array_size( jsonRoot ) > 0 )
			stepDone = (this->*m_crtStep->LoadFn)( jsonRoot, m_crtStep->index );
	}

	if ( stepDone )
		++m_crtStep;

	return ( m_crtStep != m_lastStep );
}

// ----------------------------------------------------------------------//

bool CFileSystem::LoadInit(const json_t* jsonRoot, s32 index)
{
	SIM_ASSERT( json_is_array( jsonRoot ) );

	for ( s32 k = 0; k < (s32)json_array_size( jsonRoot ); k++ )
	{
		json_t* jsonValue = json_array_get( jsonRoot, k );

		for ( s32 l = 0; l < (m_lastStep - m_steps) ; l++)
		{
			TLoadStep* ls		= &m_steps[l];
			json_t* jsonName	= json_object_get( jsonValue, "name" );

			SIM_ASSERT( json_is_string( jsonName ) );

			if ( ls->name == json_string_value( jsonName ) )
			{
				json_t* jsonFile = json_object_get( jsonValue, "file" );

				SIM_ASSERT( json_is_string( jsonFile ) );

				std::string file = json_string_value( jsonFile );
				u8* memfile	= LoadFile( file );

				ls->json = SIM_NEW CJsonStream( memfile );

				SIM_ASSERT( ls->json->GetRoot() );

				UnloadFile( memfile );

				m_loadMessage.clear();
				m_loadMessage = "Loading init ";
				m_loadMessage.append( "\"" );
				m_loadMessage.append( file );
				m_loadMessage.append( "\"" );
				m_loadMessage.append( "..." );
			}
		}
	}

	return true;
}

// ----------------------------------------------------------------------//

bool CFileSystem::LoadFont(const json_t* jsonRoot, s32 index)
{
	const char* szLetters = 
		" ~`!@#$%^&*()-_=+0123456789:;'\"\\|<>?,./?{}[]@ABCDEFGHI"
		"JKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzÁáÉéÍíÑñÓóÚú";

	json_t* jsonValue	= json_array_get( jsonRoot, index );

	std::string name	= json_string_value( json_object_get( jsonValue, "name" ) );
	std::string file	= json_string_value( json_object_get( jsonValue, "file" ) );
	s32	size			= json_integer_value( json_object_get( jsonValue, "size" ) );

	u32 offset = 0;
	m_lzmaStream->OpenFile( file, &m_buffer, &offset, &m_bufferSize );
	
	CFont* font =
	m_fontAtlas->AddFont( name, &m_buffer[ offset ], m_bufferSize, size, szLetters );

	m_lzmaStream->CloseCurrent( &m_buffer );
	m_buffer = nullptr;

	m_fontList[ hash::Get( name ) ]	= font;

	m_loadMessage.clear();
	m_loadMessage = "Loading font ";
	m_loadMessage.append( "\"" );
	m_loadMessage.append( file );
	m_loadMessage.append( "\"" );
	m_loadMessage.append( "..." );

	return NextStep(jsonRoot);
}

// ----------------------------------------------------------------------//

bool CFileSystem::LoadTexture(const json_t* jsonRoot, s32 index)
{
	json_t* jsonValue	= json_array_get( jsonRoot, index );

	std::string name	= json_string_value( json_object_get( jsonValue, "name" ) );
	std::string file	= json_string_value( json_object_get( jsonValue, "file" ) );
	std::string wrap	= json_string_value( json_object_get( jsonValue, "wrap" ) );
	std::string filter	= json_string_value( json_object_get( jsonValue, "filter" ) );

	CTexture::K_TYPE	t	= CTexture::k_Type_Default;
	CTexture::K_WRAP	w	= CTexture::k_Wrap_Default;
	CTexture::K_FILTER	f	= CTexture::k_Filter_Default;

	std::string ext(file.c_str() + (file.length() - 4));

	if ( ext == ".tga" )
		t = CTexture::k_Type_TGA;
	if ( ext == ".pvr" )
		t = CTexture::k_Type_PVR;
	if ( ext == ".mip" )
		t = CTexture::k_Type_MIP;

	if ( wrap == "clamp" )
		w = CTexture::k_Wrap_Clamp;
	if ( wrap == "repeat" )
		w = CTexture::k_Wrap_Repeat;

	if ( filter == "nearest" )
		f = CTexture::k_Filter_Nearest;
	if ( filter == "linear" )
		f = CTexture::k_Filter_Linear;
	if ( filter == "bilinear" )
		f = CTexture::k_Filter_Bilinear;
	if ( filter == "trilinear" )
		f = CTexture::k_Filter_Trilinear;
	if ( filter == "quadlinear" )
		f = CTexture::k_Filter_Quadlinear;

	u32 offset = 0;
	m_lzmaStream->OpenFile( file, &m_buffer, &offset, &m_bufferSize );

	CMemStream ms(&m_buffer[offset], m_bufferSize);
	CTexture* texture	= SIM_NEW CTexture( name );

	u32 tex = 
	m_driver->BindTexture( 0 );
	texture->Generate( &ms, t, w, f ); 
	m_driver->BindTexture( tex );

	m_lzmaStream->CloseCurrent(&m_buffer);
	m_buffer = nullptr;

	m_textureList[ hash::Get( name ) ] = texture;

	m_loadMessage.clear();
	m_loadMessage = "Loading texture ";
	m_loadMessage.append( "\"" );
	m_loadMessage.append( file );
	m_loadMessage.append( "\"" );
	m_loadMessage.append( "..." );

	return NextStep(jsonRoot);
}

// ----------------------------------------------------------------------//

bool CFileSystem::LoadSkybox(const json_t* jsonRoot, s32 index)
{
	json_t* jsonValue	= json_array_get( jsonRoot, index );

	std::string name	= json_string_value( json_object_get( jsonValue, "name" ) );

	std::string front	= json_string_value( json_object_get( jsonValue, "front" ) );
	std::string back	= json_string_value( json_object_get( jsonValue, "back" ) );
	std::string right	= json_string_value( json_object_get( jsonValue, "right" ) );
	std::string left	= json_string_value( json_object_get( jsonValue, "left" ) );
	std::string top		= json_string_value( json_object_get( jsonValue, "top" ) );
	std::string bottom	= json_string_value( json_object_get( jsonValue, "bottom" ) );

	CSkyBox* skybox		= SIM_NEW CSkyBox( name );

	u32 offset = 0;

	m_lzmaStream->OpenFile( front, &m_buffer, &offset, &m_bufferSize );
	CMemStream msfront(m_bufferSize);
	msfront.Write( &m_buffer[offset], m_bufferSize );
	msfront.Rewind();
	m_lzmaStream->CloseCurrent( &m_buffer );
	m_buffer = nullptr;

	m_lzmaStream->OpenFile( back, &m_buffer, &offset, &m_bufferSize );
	CMemStream msback(m_bufferSize);
	msback.Write( &m_buffer[offset], m_bufferSize );
	msback.Rewind();
	m_lzmaStream->CloseCurrent( &m_buffer );
	m_buffer = nullptr;

	m_lzmaStream->OpenFile( right, &m_buffer, &offset, &m_bufferSize );
	CMemStream msright(m_bufferSize);
	msright.Write( &m_buffer[offset], m_bufferSize );
	msright.Rewind();
	m_lzmaStream->CloseCurrent( &m_buffer );
	m_buffer = nullptr;

	m_lzmaStream->OpenFile( left, &m_buffer, &offset, &m_bufferSize );
	CMemStream msleft(m_bufferSize);
	msleft.Write( &m_buffer[offset], m_bufferSize );
	msleft.Rewind();
	m_lzmaStream->CloseCurrent( &m_buffer );
	m_buffer = nullptr;

	m_lzmaStream->OpenFile( top, &m_buffer, &offset, &m_bufferSize );
	CMemStream mstop(m_bufferSize);
	mstop.Write( &m_buffer[offset], m_bufferSize );
	mstop.Rewind();
	m_lzmaStream->CloseCurrent( &m_buffer );
	m_buffer = nullptr;

	m_lzmaStream->OpenFile( bottom, &m_buffer, &offset, &m_bufferSize );
	CMemStream msbot(m_bufferSize);
	msbot.Write( &m_buffer[offset], m_bufferSize );
	msbot.Rewind();
	m_lzmaStream->CloseCurrent( &m_buffer );
	m_buffer = nullptr;

	skybox->Generate( 1.0f, &msfront, &msback, &msleft, &msright, &mstop, &msbot );

	m_skyboxList[ hash::Get( name ) ] = skybox;

	m_loadMessage.clear();
	m_loadMessage = "Loading skybox ";
	m_loadMessage.append( "\"" );
	m_loadMessage.append( name );
	m_loadMessage.append( "\"" );
	m_loadMessage.append( "..." );
	
	return NextStep(jsonRoot);
}

// ----------------------------------------------------------------------//

bool CFileSystem::LoadShader(const json_t* jsonRoot, s32 index)
{
	json_t* jsonValue	= json_array_get( jsonRoot, index );

	std::string name	= json_string_value( json_object_get( jsonValue, "name" ) );
	std::string file	= json_string_value( json_object_get( jsonValue, "file" ) );

	u32 offset = 0;
	m_lzmaStream->OpenFile( file, &m_buffer, &offset, &m_bufferSize );

	s8* data = SIM_NEW s8[ m_bufferSize + 1 ];
	SIM_MEMCPY( data, &m_buffer[ offset ], m_bufferSize );
	data[ m_bufferSize ] = 0;

	m_lzmaStream->CloseFile( &m_buffer );

	CShader::K_TYPE type = CShader::k_Type_None;

	std::string ext(file.c_str() + (file.length() - 4));

	if ( ext == ".vsh" )
		type = CShader::k_Type_Vertex;
	if ( ext == ".fsh" )
		type = CShader::k_Type_Fragment;

	CShader* shader = SIM_NEW CShader( name, type );
	shader->Load( data );

	SIM_SAFE_DELETE_ARRAY(data);

	m_shaderList[ hash::Get( name ) ] = shader;

	m_loadMessage.clear();
	m_loadMessage = "Loading shader ";
	m_loadMessage.append( "\"" );
	m_loadMessage.append( name );
	m_loadMessage.append( "\"" );
	m_loadMessage.append( "..." );

	return NextStep(jsonRoot);
}

// ----------------------------------------------------------------------//

bool CFileSystem::LoadEffect(const json_t* jsonRoot, s32 index)
{
	json_t* jsonValue	= json_array_get( jsonRoot, index );

	std::string name	= json_string_value( json_object_get( jsonValue, "name" ) );
	std::string vsh		= json_string_value( json_object_get( jsonValue, "vsh" ) );
	std::string fsh		= json_string_value( json_object_get( jsonValue, "fsh" ) );

	CShader* vShader = GetShader( vsh );
	SIM_ASSERT( vShader != nullptr );

	CShader* fShader = GetShader( fsh );
	SIM_ASSERT( fShader != nullptr );

	CEffect* effect = SIM_NEW CEffect( name );
	
	json_t*  jsonAttributes = json_object_get(jsonValue, "attributes");
	SIM_ASSERT(json_is_array(jsonAttributes));

	u32 nAttrib = json_array_size(jsonAttributes);
	effect->InitAttributes( nAttrib );
	for (s32 k = 0; k < (s32)nAttrib; k++)
	{
		std::string attribute = json_string_value( json_array_get(jsonAttributes, k));

		effect->AddAttribute( attribute, k );
	}

	json_t*  jsonUniforms = json_object_get(jsonValue, "uniforms");
	SIM_ASSERT(json_is_array(jsonUniforms));

	u32 nUniforms = json_array_size(jsonUniforms);
	effect->InitUniforms(nUniforms);
	for (s32 k = 0; k < (s32)nUniforms; k++)
	{
		std::string uniform = json_string_value(json_array_get(jsonUniforms, k));

		effect->AddUniform(uniform, k);
	}


	effect->Load( vShader, fShader );

	CEffect::TTechnique technique;
	json_t*  jsonTechnique = json_object_get( jsonValue, "technique" );

	technique.depthtest = json_is_true( json_object_get( jsonTechnique, "depthtest" ) );
	technique.depthmask = json_is_true( json_object_get( jsonTechnique, "depthmask" ) );
	technique.cullface = json_is_true( json_object_get( jsonTechnique, "cullface" ) );
	technique.alphatest = json_is_true( json_object_get( jsonTechnique, "alphatest" ) );

	technique.blending = json_is_true( json_object_get( jsonTechnique, "blending" ) );

	json_t*  jsonBlendfunc = json_object_get( jsonTechnique, "blendfunc" );
	std::string bleq = json_string_value( json_object_get( jsonBlendfunc, "equation" ) );
	std::string src = json_string_value( json_object_get( jsonBlendfunc, "src" ) );
	std::string dst = json_string_value( json_object_get( jsonBlendfunc, "dst" ) );

	if (bleq == "FUNC_ADD" )
		technique.blendfunc.equation = GL_FUNC_ADD;
	if (bleq == "FUNC_SUBTRACT" )
		technique.blendfunc.equation = GL_FUNC_SUBTRACT;
	if (bleq == "FUNC_REVERSE_SUBTRACT" )
		technique.blendfunc.equation = GL_FUNC_REVERSE_SUBTRACT;

	static const u32 to[11] = 
	{ 
		GL_ZERO, 
		GL_ONE, 
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA, 
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA_SATURATE,
	};

	static const char* from[11] = 
	{ 
		"ZERO", 
		"ONE", 
		"SRC_COLOR",
		"ONE_MINUS_SRC_COLOR",
		"SRC_ALPHA", 
		"ONE_MINUS_SRC_ALPHA",
		"DST_ALPHA",
		"ONE_MINUS_DST_ALPHA",
		"DST_COLOR",
		"ONE_MINUS_DST_COLOR",
		"SRC_ALPHA_SATURATE",
	};

	for ( u32 k = 0; k < 11; k++ )
		if ( src == from[ k ] )
			technique.blendfunc.src = to[ k ];

	for ( u32 k = 0; k < 11; k++ )
		if ( dst == from[ k ] )
			technique.blendfunc.dst = to[ k ];

	std::string dpeq = json_string_value(json_object_get(jsonTechnique, "depthfunc"));

	if (dpeq == "GL_NEVER")
		technique.depthfunc.equation = GL_NEVER;
	if (dpeq == "GL_LESS")
		technique.depthfunc.equation = GL_LESS;
	if (dpeq == "GL_EQUAL")
		technique.depthfunc.equation = GL_EQUAL;
	if (dpeq == "GL_LEQUAL")
		technique.depthfunc.equation = GL_LEQUAL;
	if (dpeq == "GL_GREATER")
		technique.depthfunc.equation = GL_GREATER;
	if (dpeq == "GL_NOTEQUAL")
		technique.depthfunc.equation = GL_NOTEQUAL;
	if (dpeq == "GL_GEQUAL")
		technique.depthfunc.equation = GL_GEQUAL;
	if (dpeq == "GL_ALWAYS")
		technique.depthfunc.equation = GL_ALWAYS;

	effect->SetTechnique( &technique );

	json_t* jsonTex =  json_object_get( jsonValue, "textures" );
	SIM_ASSERT( json_is_array( jsonTex ) );

	for ( s32 k = 0; k < (s32)json_array_size( jsonTex ); k++ )
	{
		json_t* jsonInfo = json_array_get( jsonTex, k );

		std::string texName		= json_string_value ( json_object_get( jsonInfo, "name" ) );
		s32 channel				= json_integer_value ( json_object_get( jsonInfo, "channel" ) );

		CTexture* tex = GetTexture( texName );

		effect->SetTexture( tex, channel );

		SIM_ASSERT( tex != nullptr );
	}

	m_effectList[ hash::Get( name ) ] = effect;

	m_loadMessage.clear();
	m_loadMessage = "Loading effect ";
	m_loadMessage.append( "\"" );
	m_loadMessage.append( name );
	m_loadMessage.append( "\"" );
	m_loadMessage.append( "..." );

	return NextStep(jsonRoot);
}

// ----------------------------------------------------------------------//

bool CFileSystem::LoadMaterial(const json_t* jsonRoot, s32 index)
{
	json_t* jsonValue	= json_array_get( jsonRoot, index );

	std::string name	= json_string_value( json_object_get( jsonValue, "name" ) );
	std::string file	= json_string_value( json_object_get( jsonValue, "file" ) );

	u32 offset = 0;
	m_lzmaStream->OpenFile( file, &m_buffer, &offset, &m_bufferSize );

	u8* data = SIM_NEW u8[ m_bufferSize + 1 ];
	SIM_MEMCPY( data, &m_buffer[ offset ], m_bufferSize );
	data[ m_bufferSize ] = 0;

	m_lzmaStream->CloseFile( &m_buffer );

	CJsonStream jm( data );
	json_t* jmr = jm.GetRoot();

	SIM_SAFE_DELETE_ARRAY( data );

	CMaterial* material = SIM_NEW CMaterial(name);

	json_t*  ambient	= json_object_get( jmr, "ambient" );
	SIM_ASSERT( json_is_array(ambient) && 4 == json_array_size(ambient) );

	TVec4 a;
	a.x = (f32)json_real_value( json_array_get( ambient, 0 ) );
	a.y = (f32)json_real_value( json_array_get( ambient, 1 ) );
	a.z = (f32)json_real_value( json_array_get( ambient, 2 ) );
	a.w = (f32)json_real_value( json_array_get( ambient, 3 ) );
	material->SetAmbient( &a );

	json_t*  diffuse	= json_object_get( jmr, "diffuse" );
	SIM_ASSERT( json_is_array(diffuse) && 4 == json_array_size(diffuse) );

	TVec4 d;
	d.x = (f32)json_real_value( json_array_get( diffuse, 0 ) );
	d.y = (f32)json_real_value( json_array_get( diffuse, 1 ) );
	d.z = (f32)json_real_value( json_array_get( diffuse, 2 ) );
	d.w = (f32)json_real_value( json_array_get( diffuse, 3 ) );
	material->SetDiffuse( &a );

	json_t*  emissive	= json_object_get( jmr, "emissive" );
	SIM_ASSERT( json_is_array(emissive) && 4 == json_array_size(emissive) );

	TVec4 e;
	e.x = (f32)json_real_value( json_array_get( emissive, 0 ) );
	e.y = (f32)json_real_value( json_array_get( emissive, 1 ) );
	e.z = (f32)json_real_value( json_array_get( emissive, 2 ) );
	e.w = (f32)json_real_value( json_array_get( emissive, 3 ) );
	material->SetEmissive( &a );

	json_t*  specular	= json_object_get( jmr, "specular" );
	SIM_ASSERT( json_is_array(specular) && 4 == json_array_size(specular) );

	TVec4 s;
	s.x = (f32)json_real_value( json_array_get( specular, 0 ) );
	s.y = (f32)json_real_value( json_array_get( specular, 1 ) );
	s.z = (f32)json_real_value( json_array_get( specular, 2 ) );
	s.w = (f32)json_real_value( json_array_get( specular, 3 ) );
	material->SetSpecular( &a );

	f32	r;
	r = (f32)json_real_value( json_object_get( jmr, "reflectivity" ) );
	material->SetReflectivity( r );


	f32	h;
	h = (f32)json_real_value( json_object_get( jmr, "shininess" ) );
	material->SetShininess( h );

	json_t*  texture = json_object_get( jmr, "texture" );
	SIM_ASSERT( json_is_array(texture) && CDriver::k_Select_Texture_Count > json_array_size(texture) );

	for (s32 k = 0; k < (s32)json_array_size(texture); k++ )
	{
		json_t*  t		= json_array_get( texture, k );

		std::string n	= json_string_value( json_object_get( t, "name" ) );
		s32 c			= json_integer_value( json_object_get( t, "channel" ) );

		material->SetTexture( GetTexture( n ) , c);
	}

	std::string  effect = json_string_value( json_object_get( jmr, "effect" ) );
	material->SetEffect( GetEffect( effect ) );

	m_materialList[ hash::Get( name ) ] = material;

	m_loadMessage.clear();
	m_loadMessage = "Loading material ";
	m_loadMessage.append( "\"" );
	m_loadMessage.append( name );
	m_loadMessage.append( "\"" );
	m_loadMessage.append( "..." );

	return NextStep(jsonRoot);
}
// ----------------------------------------------------------------------//

bool CFileSystem::LoadMesh(const json_t* jsonRoot, s32 index)
{
	json_t* jsonValue	= json_array_get( jsonRoot, index );

	std::string name	= json_string_value( json_object_get( jsonValue, "name" ) );
	std::string file	= json_string_value( json_object_get( jsonValue, "file" ) );

	CMesh* mesh			= SIM_NEW CMesh( name, this );

	u32 offset = 0;
	m_lzmaStream->OpenFile( file, &m_buffer, &offset, &m_bufferSize );
	
	CMemStream ms( &m_buffer[ offset ], m_bufferSize );
	
	mesh->Load( &ms );
	mesh->BindMaterial();

	m_lzmaStream->CloseFile( &m_buffer );

	m_meshList[ hash::Get( name ) ] = mesh;

	m_loadMessage.clear();
	m_loadMessage = "Loading mesh ";
	m_loadMessage.append( "\"" );
	m_loadMessage.append( name );
	m_loadMessage.append( "\"" );
	m_loadMessage.append( "..." );

	return NextStep(jsonRoot);
}

// ----------------------------------------------------------------------//

bool CFileSystem::LoadActor(const json_t* jsonRoot, s32 index)
{

	return NextStep(jsonRoot);
}

// ----------------------------------------------------------------------//

bool CFileSystem::LoadLight(const json_t* jsonRoot, s32 index)
{
	return NextStep(jsonRoot);
}
// ----------------------------------------------------------------------//

bool CFileSystem::LoadCamera(const json_t* jsonRoot, s32 index)
{
	return NextStep(jsonRoot);
}

// ----------------------------------------------------------------------//

bool CFileSystem::LoadSprite(const json_t* jsonRoot, s32 index)
{
	return NextStep(jsonRoot);
}

// ----------------------------------------------------------------------//

bool CFileSystem::LoadSound(const json_t* jsonRoot, s32 index)
{
	return NextStep(jsonRoot);
}
// ----------------------------------------------------------------------//

bool CFileSystem::LoadScript(const json_t* jsonRoot, s32 index)
{
	json_t* jsonValue	= json_array_get( jsonRoot, index );

	std::string name	= json_string_value( json_object_get( jsonValue, "name" ) );
	std::string file	= json_string_value( json_object_get( jsonValue, "file" ) );

	CScript* script = SIM_NEW CScript( name, m_squirrel );

	u32 offset = 0;
	m_lzmaStream->OpenFile( file, &m_buffer, &offset, &m_bufferSize );
	
	Sqrat::string source((s8*)m_buffer);
	Sqrat::string errorMsg;
	
	CMemStream ms( &m_buffer[ offset ], m_bufferSize );
	SQRESULT result = script->Load( &ms );

	SIM_ASSERT( SQ_SUCCEEDED( result ) );

	m_lzmaStream->CloseFile( &m_buffer );

	m_scriptList[ hash::Get( name ) ] = script;

	m_loadMessage.clear();
	m_loadMessage = "Loading script ";
	m_loadMessage.append( "\"" );
	m_loadMessage.append( name );
	m_loadMessage.append( "\"" );
	m_loadMessage.append( "..." );

	return NextStep(jsonRoot);
}

// ----------------------------------------------------------------------//

bool CFileSystem::LoadScene(const json_t* jsonRoot, s32 index)
{
	return NextStep(jsonRoot);
}

// ----------------------------------------------------------------------//

void CFileSystem::UnloadFonts()
{
	auto it = m_fontList.begin();

	while( it != m_fontList.end())
	{
		SIM_SAFE_DELETE(it->second);
		++it;
	}

	m_fontList.clear();
}

// ----------------------------------------------------------------------//

rnr::CFont* CFileSystem::GetFont( const std::string &name )
{
	auto it = m_fontList.find( hash::Get( name ) );

	return it != m_fontList.end() ? it->second : nullptr;
}

// ----------------------------------------------------------------------//

void CFileSystem::UnloadTextures()
{
	auto it = m_textureList.begin();

	while( it != m_textureList.end())
	{
		SIM_SAFE_DELETE(it->second);
		++it;
	}

	m_textureList.clear();
}

// ----------------------------------------------------------------------//

rnr::CTexture* CFileSystem::GetTexture( const std::string &name )
{
	auto it = m_textureList.find( hash::Get( name ) );

	return it != m_textureList.end() ? it->second : nullptr;
}

// ----------------------------------------------------------------------//

void CFileSystem::UnloadSkyboxes()
{
	auto it = m_skyboxList.begin();

	while( it != m_skyboxList.end())
	{
		SIM_SAFE_DELETE(it->second);
		++it;
	}

	m_skyboxList.clear();
}

// ----------------------------------------------------------------------//

rnr::CSkyBox* CFileSystem::GetSkybox( const std::string &name )
{
	auto it = m_skyboxList.find( hash::Get( name ) );

	return it != m_skyboxList.end() ? it->second : nullptr;
}

// ----------------------------------------------------------------------//

void CFileSystem::UnloadShaders()
{
	auto it = m_shaderList.begin();

	while( it != m_shaderList.end())
	{
		SIM_SAFE_DELETE(it->second);
		++it;
	}

	m_shaderList.clear();
}

// ----------------------------------------------------------------------//

rnr::CShader* CFileSystem::GetShader( const std::string &name )
{
	auto it = m_shaderList.find( hash::Get( name ) );

	return it != m_shaderList.end() ? it->second : nullptr;
}

// ----------------------------------------------------------------------//

void CFileSystem::UnloadEffects()
{
	auto it = m_effectList.begin();

	while( it != m_effectList.end())
	{
		SIM_SAFE_DELETE(it->second);
		++it;
	}

	m_effectList.clear();
}

// ----------------------------------------------------------------------//

rnr::CEffect* CFileSystem::GetEffect( const std::string &name )
{
	auto it = m_effectList.find( hash::Get( name ) );

	return it != m_effectList.end() ? it->second : nullptr;
}


// ----------------------------------------------------------------------//

void CFileSystem::UnloadMaterials()
{
	auto it = m_materialList.begin();

	while( it != m_materialList.end())
	{
		SIM_SAFE_DELETE(it->second);
		++it;
	}

	m_materialList.clear();
}

// ----------------------------------------------------------------------//

rnr::CMaterial* CFileSystem::GetMaterial( const std::string &name )
{
	auto it = m_materialList.find( hash::Get( name ) );

	return it != m_materialList.end() ? it->second : nullptr;
}

// ----------------------------------------------------------------------//

void CFileSystem::UnloadMeshes()
{
	auto it = m_meshList.begin();

	while( it != m_meshList.end())
	{
		SIM_SAFE_DELETE(it->second);
		++it;
	}

	m_meshList.clear();
}

// ----------------------------------------------------------------------//

rnr::CMesh* CFileSystem::GetMesh( const std::string &name )
{
	auto it = m_meshList.find( hash::Get( name ) );

	return it != m_meshList.end() ? it->second : nullptr;
}

// ----------------------------------------------------------------------//

void CFileSystem::UnloadActors()
{
	auto it = m_actorList.begin();

	while( it != m_actorList.end())
	{
		SIM_SAFE_DELETE(it->second);
		++it;
	}

	m_actorList.clear();
}

// ----------------------------------------------------------------------//

rnr::CActor* CFileSystem::GetActor( const std::string &name )
{
	auto it = m_actorList.find( hash::Get( name ) );

	return it != m_actorList.end() ? it->second : nullptr;
}

// ----------------------------------------------------------------------//

void CFileSystem::UnloadLights()
{
	auto it = m_lightList.begin();

	while( it != m_lightList.end())
	{
		SIM_SAFE_DELETE(it->second);
		++it;
	}

	m_lightList.clear();
}

// ----------------------------------------------------------------------//

rnr::CLight* CFileSystem::GetLight( const std::string &name )
{
	auto it = m_lightList.find( hash::Get( name ) );

	return it != m_lightList.end() ? it->second : nullptr;
}

// ----------------------------------------------------------------------//

void CFileSystem::UnloadCameras()
{
	auto it = m_cameraList.begin();

	while( it != m_cameraList.end())
	{
		SIM_SAFE_DELETE(it->second);
		++it;
	}

	m_cameraList.clear();
}

// ----------------------------------------------------------------------//

rnr::CCamera* CFileSystem::GetCamera( const std::string &name )
{
	auto it = m_cameraList.find( hash::Get( name ) );

	return it != m_cameraList.end() ? it->second : nullptr;
}

// ----------------------------------------------------------------------//

void CFileSystem::UnloadSprites()
{
	auto it = m_spriteList.begin();

	while( it != m_spriteList.end())
	{
		SIM_SAFE_DELETE(it->second);
		++it;
	}

	m_spriteList.clear();
}

// ----------------------------------------------------------------------//

rnr::CSpriteTexture* CFileSystem::GetSprite( const std::string &name )
{
	auto it = m_spriteList.find( hash::Get( name ) );

	return it != m_spriteList.end() ? it->second : nullptr;
}

// ----------------------------------------------------------------------//

void CFileSystem::UnloadSounds()
{
	auto it = m_soundList.begin();

	while( it != m_soundList.end())
	{
		SIM_SAFE_DELETE(it->second);
		++it;
	}

	m_soundList.clear();
}

// ----------------------------------------------------------------------//

snd::CSoundData* CFileSystem::GetSound( const std::string &name )
{
	auto it = m_soundList.find( hash::Get( name ) );

	return it != m_soundList.end() ? it->second : nullptr;
}

// ----------------------------------------------------------------------//

void CFileSystem::UnloadScripts()
{
	auto it = m_scriptList.begin();

	while( it != m_scriptList.end())
	{
		SIM_SAFE_DELETE(it->second);
		++it;
	}

	m_scriptList.clear();
}

// ----------------------------------------------------------------------//

vm::CScript* CFileSystem::GetScript( const std::string &name )
{
	auto it = m_scriptList.find( hash::Get( name ) );

	return it != m_scriptList.end() ? it->second : nullptr;
}

// ----------------------------------------------------------------------//

void CFileSystem::UnloadScenes()
{
	auto it = m_sceneList.begin();

	while( it != m_sceneList.end())
	{
		SIM_SAFE_DELETE(it->second);
		++it;
	}

	m_sceneList.clear();
}

// ----------------------------------------------------------------------//

rnr::CScene* CFileSystem::GetScene( const std::string &name )
{
	auto it = m_sceneList.find( hash::Get( name ) );

	return it != m_sceneList.end() ? it->second : nullptr;
}

// ----------------------------------------------------------------------//
}; // namespace io
}; // namespace sim
