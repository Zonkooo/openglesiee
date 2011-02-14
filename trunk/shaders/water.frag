varying vec3 N;
varying vec3 V;
varying vec3 S;
varying vec3 view;

uniform samplerCube sky;

void main(void)
{
	vec3 color;
	vec3 water_color = (textureCube(sky, S).rgb + 0.5*vec3(0.0, 0.0, 1.0)) / 1.5;
	
/*	// lumiere ambiante*/
/*	color = gl_LightSource[0].ambient * water_color;*/
/*	*/
/*	// diffus*/
/*	vec3 i = gl_LightSource[0].position.xyz - V;*/
/*	color += gl_LightSource[0].diffuse * water_color * max(0.0,dot(normalize(N), normalize(i)));*/
/*  */
/*	// speculaire*/
/*	vec3 r = reflect(-i, N);*/
/*	color += gl_LightSource[0].specular * water_color * pow(max(0.0,dot(normalize(-V), normalize(r))), gl_FrontMaterial.shininess); */

/*	gl_FragColor.rgb = color;*/
	gl_FragColor.rgb = water_color;
}

