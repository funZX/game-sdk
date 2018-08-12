precision highp float;

uniform sampler2D  u_Sampler_Tex_0;
uniform sampler2D  u_Sampler_Tex_1;
		
varying vec3  v_LightPos;
varying vec2  v_Tex0;

void main()
{
	vec3 normal = texture2D( u_Sampler_Tex_1, v_Tex0 ).rgb * 2.0 - 1.0;

	float lightIntensity = dot( v_LightPos, normal );

	vec3 tex = texture2D( u_Sampler_Tex_0, v_Tex0 ).rgb;	

	gl_FragColor = vec4( tex * lightIntensity, 1.0);
}


