precision highp float;

uniform sampler2D	u_Sampler_Tex_0;
uniform sampler2D	u_Sampler_Tex_1;
uniform sampler2D	u_Sampler_Tex_2;

varying vec3		v_lightDir;
varying vec3		v_eyeDir;
varying vec2		v_Tex0;

void main (void)
{
	vec3 lightDir = normalize( v_lightDir );
	
	float depth = texture2D( u_Sampler_Tex_2, v_Tex0 ).x;
	
	vec2 texCoord = v_Tex0 + ( depth * v_eyeDir.xy );
	
	vec3 difuse = texture2D( u_Sampler_Tex_0, texCoord ).rgb;
	
	vec3 normal = ( texture2D( u_Sampler_Tex_1, texCoord ).rbg ) * 2.0 - 1.0;
		
	float intensity = max( dot( v_lightDir, normal ), 0.0 );	
	
	vec3 colour = vec3( intensity ) * difuse;

	gl_FragColor = vec4( colour, 1.0 );
}