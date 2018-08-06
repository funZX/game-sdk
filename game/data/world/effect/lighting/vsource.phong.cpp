precision mediump float;

attribute vec4 a_PositionL;
attribute vec2 a_TexCoord_0;
attribute vec3 a_NormalL;

uniform mat4 u_Matrix_Projection;
uniform mat4 u_Matrix_WorldView;
uniform mat4 u_Matrix_View;
uniform mat3 u_Matrix_Normal;

uniform vec3 u_Light_Position_0;


varying vec2 v_Tex0;
varying vec3 v_ToEyeDirV;
varying vec3 v_ToLightDirV;
varying vec3 v_NormalW;

void main()
{
	vec4 posV, ligV;
	
	posV 			= a_PositionL * u_Matrix_WorldView;
	ligV 			= vec4( u_Light_Position_0, 1.0 ) * u_Matrix_View;
	
	v_NormalW 		= a_NormalL * u_Matrix_Normal;
	
	v_ToEyeDirV 	= 		   - posV.xyz;	
	v_ToLightDirV 	= ligV.xyz - posV.xyz;
	v_Tex0 			= a_TexCoord_0;

	
	gl_Position 	= posV * u_Matrix_Projection;
}
