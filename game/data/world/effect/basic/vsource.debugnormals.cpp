attribute vec4 a_PositionL;
attribute vec4 a_NormalL;

uniform mat4 u_Matrix_WorldViewProjection;
uniform vec4 u_Color;

varying vec4 v_Color;

void main()
{
	v_Color			= u_Color;
	
	gl_PointSize 	= 2.0;
	gl_Position		= a_PositionL * u_Matrix_WorldViewProjection + a_NormalL * 0.1;
}