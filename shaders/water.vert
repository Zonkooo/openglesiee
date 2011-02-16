varying vec3 N; //normale
varying vec3 V; //view (vecteur scene-œuil)
varying vec3 S; //reflexion parfaite
varying vec3 view;

uniform mat4 cam_to_scene;

mat3 GetLinearPart( mat4 m )
{
	mat3 result;
	
	result[0][0] = m[0][0]; 
	result[0][1] = m[0][1]; 
	result[0][2] = m[0][2]; 

	result[1][0] = m[1][0]; 
	result[1][1] = m[1][1]; 
	result[1][2] = m[1][2]; 
	
	result[2][0] = m[2][0]; 
	result[2][1] = m[2][1]; 
	result[2][2] = m[2][2]; 
	
	return result;
}		

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();
    
	vec3 N = normalize(gl_NormalMatrix * gl_Normal);
	vec4 P = gl_ModelViewMatrix * gl_Vertex;
	vec3 V = normalize(-P.xyz/P.w); 
	
	S = 2.0 * N * dot(V, N) - V;
	mat3 cam_to_scene3x3 = GetLinearPart(cam_to_scene);
	S = normalize(cam_to_scene3x3 * S); 
}

