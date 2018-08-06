precision lowp float;

uniform sampler2D	u_Sampler_Tex_0;

varying vec2 v_Tex0;
varying vec4 v_Color;

#include "../library/functions.h"

void main()
{
	vec4 col = v_Color;
	
#if USE_TEXTURE_COLOR
	vec4 tex = texture2D( u_Sampler_Tex_0, v_Tex0 );	
	col *= tex;
#endif	

	gl_FragColor = col;
}