attribute vec4 a_PositionL;
attribute vec2 a_TexCoord_0;
attribute vec3 a_NormalL;

uniform mat4 u_Matrix_Projection;
uniform mat4 u_Matrix_WorldView;
uniform mat3 u_Matrix_Normal;

varying vec2 v_Tex0;
varying vec3 v_NormalW;
varying vec3 v_ToEyeDirV;

void main()
{
	vec4 posV;
	
	posV 		= a_PositionL * u_Matrix_WorldView;
	
	v_NormalW 	= a_NormalL * u_Matrix_Normal;
	v_ToEyeDirV = -posV.xyz;
	v_Tex0 		= a_TexCoord_0;
	
	gl_Position = posV * u_Matrix_Projection;
}
