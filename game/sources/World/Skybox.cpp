#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "../Render/Model.h"
#include "../Render/Texture.h"
#include "../Render/Renderer.h"

#include "../Shaders/SkyShader.h"

#include "Sky.h"

CSky :: CSky()
{
	m_Name[ 0 ]			= '\0';

	m_Shader			= NULL;
	m_Model				= NULL;

	m_Texture			= 0;
	m_Scale				= 10.0f;

	Vec3ToZero( &m_Position );
}

CSky :: ~CSky()
{
	if( m_Texture )
		glDeleteTextures( 1, &m_Texture );
}

void CSky :: SetName( const char * name )
{
	ASSERT( strlen( name ) < 64 );

	strcpy( m_Name, name );
}

void CSky :: Setup(  CModel	*mdl,
				const char *topTex, const char *bottomTex,
				const char *leftTex, const char *rightTex,
				const char *frontTex, const char *backTex )
{
	unsigned char *buf = NULL;
	unsigned int width, height, bits;

	m_Model = mdl;
	glGenTextures( 1, &m_Texture );
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_Texture );

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	// TOP
	buf = CTexture :: LoadTGA( topTex, &width, &height, &bits );
	{
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB,
					  width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buf );
	}
	SAFE_DELETE_ARRAY( buf );

	// BOTTOM
	buf = CTexture :: LoadTGA( bottomTex, &width, &height, &bits );
	{
		glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB,
					  width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buf );
	}
	SAFE_DELETE_ARRAY( buf );

	// LEFT
	buf = CTexture :: LoadTGA( leftTex, &width, &height, &bits );
	{
		glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB,
					  width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buf );
	}
	SAFE_DELETE_ARRAY( buf );

	// RIGHT
	buf = CTexture :: LoadTGA( rightTex, &width, &height, &bits );
	{
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB,
					  width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buf );
	}
	SAFE_DELETE_ARRAY( buf );

	// FRONT
	buf = CTexture :: LoadTGA( frontTex, &width, &height, &bits );
	{
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB,
					  width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buf );
	}
	SAFE_DELETE_ARRAY( buf );

	// BACK
	buf = CTexture :: LoadTGA( backTex, &width, &height, &bits );
	{
		glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB,
					  width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buf );
	}
	SAFE_DELETE_ARRAY( buf );

	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
}

void CSky :: Render( CRenderer *ren )
{
	ren->ActiveTexture( e_RendererTextureSelect4 );
	ren->BindTexture( GL_TEXTURE_CUBE_MAP, m_Texture );

	glDepthRangef( 0.99f, 1.00f );
	ren->Push();
	{
		ren->Translate( &m_Position );

		m_Shader->Begin( ren, m_Model );
		m_Model->GetMesh( 0 )->Render( ren );
		m_Shader->End();
	}
	ren->Pop();
	glDepthRangef( 0.00f, 1.00f );
}