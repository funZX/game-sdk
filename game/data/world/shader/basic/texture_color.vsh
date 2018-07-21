attribute vec4 a_PositionL;
attribute vec2 a_TexCoord_0;

uniform mat4 u_Matrix_WorldViewProjection;
uniform vec4 u_Color;

varying vec2 v_Tex0;
varying vec4 v_Color;

void main()
{
	v_Tex0			= a_TexCoord_0;
	v_Color			= u_Color;

	gl_Position		= a_PositionL * u_Matrix_WorldViewProjection;
}