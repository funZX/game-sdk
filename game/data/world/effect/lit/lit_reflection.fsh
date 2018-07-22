precision highp float;

uniform sampler2D u_Sampler_Tex_0;
uniform samplerCube u_Sampler_Cube;
uniform float u_Material_Reflectivity;

varying vec2 v_Tex0;
varying vec3 v_NormalW;
varying vec3 v_ToEyeDirV;

void main()
{
	vec3 norW	= normalize( v_NormalW );
	vec3 toEyeV	= normalize( v_ToEyeDirV );

	vec4 tex0	= texture2D( u_Sampler_Tex_0, v_Tex0.xy );
	vec4 tex1	= textureCube( u_Sampler_Cube, reflect( toEyeV, norW ) );
	
	gl_FragColor = mix( tex0, tex1, u_Material_Reflectivity );
}