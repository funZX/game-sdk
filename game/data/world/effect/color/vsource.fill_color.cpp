attribute vec4 a_PositionL;

uniform mat4 u_Matrix_WorldViewProjection;
uniform vec4 u_Color;

varying vec4 v_Color;

void main()
{
	v_Color			= u_Color;

	gl_Position		= a_PositionL * u_Matrix_WorldViewProjection;
}