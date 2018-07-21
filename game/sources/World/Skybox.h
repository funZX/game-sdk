#ifndef __SKY_H
#define __SKY_H

class CModel;
class CRenderer;
class CShader;

class CSky
{
friend class CSkyShader;

public:
	char				m_Name[ 64 ];

	CShader				*m_Shader;
	CModel				*m_Model;

	unsigned int		m_Texture;

	float				m_Scale;
	TVec3				m_Position;

public:
	CSky();
	~CSky();

	void				Setup( CModel	*mdl,
							const char *topTex, const char *bottomTex,
							const char *leftTex, const char *rightTex,
							const char *frontTex, const char *backTex );

	void				Render( CRenderer *ren );

	inline CModel*		GetModel() { return m_Model; }

	void				SetName( const char *name );
	inline const char*	GetName() { return &m_Name[ 0 ]; }

	inline void			SetPosition( TVec3 *pos ) { Vec3Copy( &m_Position, pos ); };
	inline void			SetShader( CShader *SH ) { m_Shader = SH; }
};

#endif
