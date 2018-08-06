attribute vec4 a_PositionL;
attribute vec2 a_TexCoord_0;
attribute vec4 a_Color;

uniform mat4 u_Matrix_WorldViewProjection;
uniform vec4 u_Material_Diffuse;
uniform vec4 u_Color;

varying vec2 v_Tex0;
varying vec4 v_Color;

#include "../library/functions.h"

void main()
{
	v_Color			= u_Color;
	
#if USE_VERTEX_COLOR	
	v_Color			*= a_Color;
#endif	
#if USE_MATERIAL_DIFFUSE	
	v_Color			*= u_Material_Diffuse;
#endif	
#if USE_TEXTURE_COLOR
	v_Tex0			= a_TexCoord_0;
#endif
	
	gl_Position		= a_PositionL * u_Matrix_WorldViewProjection;
}	
