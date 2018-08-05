precision lowp float;

uniform sampler2D	u_Sampler_Tex_0;

varying vec2 v_Tex0;

void main()
{
	vec4 col = texture2D( u_Sampler_Tex_0, v_Tex0 );

	gl_FragColor = col;
}