varying vec3 normal;
varying vec3 p;
varying vec3 view;

void main(void)
{
	vec4 color, water_color;
	water_color.rgb = vec3(0.0, 0.0, 1.0);
	// lumiere ambiante
	color = gl_LightSource[0].ambient * water_color;
	
	// diffus
	vec3 i = gl_LightSource[0].position.xyz - p;
	color += gl_LightSource[0].diffuse * water_color * max(0.0,dot(normalize(normal), normalize(i)));
  
	// speculaire
	vec3 r = reflect(-i, normal);
	color += gl_LightSource[0].specular * water_color * pow(max(0.0,dot(normalize(-p), normalize(r))), gl_FrontMaterial.shininess); 

	//  vecteur Reflechi
	vec3 reflect = normalize(reflect(view, normalize(normal)));

	gl_FragColor = color;
}

