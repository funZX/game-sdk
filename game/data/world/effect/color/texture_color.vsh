attribute vec4 a_PositionL;
attribute vec2 a_TexCoord_0;

uniform mat4 u_Matrix_WorldViewProjection;

varying vec2 v_Tex0;

void main()
{
	v_Tex0			= a_TexCoord_0;

	gl_Position		= a_PositionL * u_Matrix_WorldViewProjection;
}	