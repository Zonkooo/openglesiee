varying vec3 N;
varying vec3 V;
varying vec3 S;

uniform samplerCube sky;
uniform int time;

float reflect = 0.6;

void main(void)
{
	float x = gl_TexCoord[0].x;
	float y = gl_TexCoord[0].y;
	//wrecking havoc in the reflexion vector
	S.x += 0.05*cos(0.001*(float)time + x * 87. + y * 37.);
	S.x += 0.07*sqrt(1.5+sin(0.001*(float)time + y * 244. + x * 79.));
	S = normalize(S);
	 		
	vec3 water_color = reflect*textureCube(sky, S).rgb + (1.0 - reflect)*vec3(0.07, 0.04, 0.56); //ultramarine !
		
	gl_FragColor.rgb = water_color;
	
	if(gl_FragColor.r > 0.55 && gl_FragColor.g > 0.55 && gl_FragColor.b > 0.75)
		gl_FragColor *= 1.3; //faux spéculaire sale
	
	//un peu de speculaire
	//vec3 r = reflect(gl_LightSource[0].position.xyz - P, N);
	//gl_FragColor.rgb += 0.7 * water_color * pow(max(0.0,dot(normalize(-V), S)), 50.);
	//non pas de spéculaire en fait
	
}

