attribute vec4		a_PositionL;
attribute vec2		a_TexCoord_0;
attribute vec3		a_NormalL;
attribute vec3		a_TangentL;
attribute vec3		a_BinormalL;

uniform mat4		u_Matrix_WorldViewProjection;
uniform mat3		u_Matrix_Normal;
uniform vec3		u_Light_Position_0;

varying vec3		v_lightDir;
varying vec3		v_eyeDir;
varying vec2		v_Tex0;

const float k_Parallax_Scale = 0.065;

void main(void)
{	
	vec3 n		= normalize( a_NormalL * u_Matrix_Normal );
	vec3 t		= normalize( a_TangentL * u_Matrix_Normal );
	vec3 b		= cross( n, t );

	mat3 eyeToTangent = mat3( t.x, b.x, n.x, t.y, b.y, n.y, t.z, b.z, n.z);
	
	vec4 pos	= u_Matrix_WorldViewProjection * a_PositionL;

	v_eyeDir	= pos.xyz;
	v_eyeDir	= normalize( v_eyeDir * eyeToTangent ) * k_Parallax_Scale;
	
	v_lightDir	= normalize( u_Light_Position_0 - pos.xyz );
	
	v_Tex0		= a_TexCoord_0;

	gl_Position = a_PositionL * u_Matrix_WorldViewProjection;
}