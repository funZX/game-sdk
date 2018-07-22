attribute vec4 a_PositionL;
attribute vec4 a_Color;

uniform mat4 u_Matrix_WorldViewProjection;

varying vec4 v_Color;

void main()
{
	v_Color			= a_Color / 255.0;

	gl_Position		= a_PositionL * u_Matrix_WorldViewProjection;
}