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
	S.x += 0.05*cos(0.001*time + x * 87. + y * 37.);
	S.x += 0.07*sqrt(1.5+sin(0.001*time + y * 244. + x * 79.));
	S = normalize(S);
	vec3 water_color = reflect*textureCube(sky, S).rgb + (1.0 - reflect)*vec3(0.07, 0.04, 0.56); //ultramarine !
	
	gl_FragColor.rgb = water_color;
}

