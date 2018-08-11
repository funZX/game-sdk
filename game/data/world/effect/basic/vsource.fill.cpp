attribute vec4 a_PositionL;
attribute vec2 a_TexCoord_0;
attribute vec4 a_Color;

uniform mat4 u_Matrix_WorldViewProjection;
uniform vec4 u_Material_Diffuse;
uniform vec4 u_Color;

varying vec2 v_Tex0;
varying vec4 v_Color;

#include "../include/func.h"

void main()
{
	vec4 col = UNDEFINED_COLOR;
#if USE_CLEAR_COLOR	
	col = u_Color;
#endif	
#if USE_VERTEX_COLOR	
#if !(USE_CLEAR_COLOR)
	col = a_Color;
#else
	col *= a_Color;
#endif	
#endif	
#if USE_MATERIAL_DIFFUSE	
#if !(USE_VERTEX_COLOR || USE_CLEAR_COLOR)
	col = u_Material_Diffuse;
#else	
	col *= u_Material_Diffuse;
#endif	
#endif
#if USE_TEXTURE_COLOR
	v_Tex0 = a_TexCoord_0;
#endif
	v_Color 		= col;	
	gl_Position		= a_PositionL * u_Matrix_WorldViewProjection;
}	