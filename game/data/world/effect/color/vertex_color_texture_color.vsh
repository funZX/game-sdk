attribute vec4 a_PositionL;
attribute vec2 a_TexCoord_0;
attribute vec4 a_Color;

uniform mat4 u_Matrix_WorldViewProjection;

varying vec2 v_Tex0;
varying vec4 v_Color;

void main()
{
	v_Tex0			= a_TexCoord_0;
	v_Color			= a_Color;

	gl_Position		= a_PositionL * u_Matrix_WorldViewProjection;
}	