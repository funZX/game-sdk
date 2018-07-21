precision lowp float;

uniform sampler2D	u_Sampler_Tex_0;

varying vec2 v_Tex0;
varying vec4 v_Color;

void main()
{
	vec4 tex = texture2D( u_Sampler_Tex_0, v_Tex0 );
	vec4 col = tex * v_Color;

	gl_FragColor = col;
}