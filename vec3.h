#pragma once

#ifndef _VEC3_H
#define _VEC3_H

#include <cmath>

class vec3 {

	public:
	float x, y, z;

	vec3(){x=0; y=0; z=0;}
	vec3(float X, float Y, float Z){
		x=X;
		y=Y;
		z=Z;
	}

	vec3 operator+ (vec3 b){return vec3(x+b.x, y+b.y, z+b.z);}
	vec3 operator- (vec3 b){return vec3(x-b.x, y-b.y, z-b.z);}
	vec3 operator* (float b){return vec3(x*b, y*b, z*b);}
	float operator* (vec3 b){return x*b.x + y*b.y + z*b.z;}
	vec3 operator/ (float b){return vec3(x/b, y/b, z/b);}

	vec3 cross ( vec3 b){return vec3(y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x);}
	vec3 normalize()
	{
		float c = 1/ length();
		return vec3(x*c, y*c, z*c);
	}
	float length(){return sqrt(x*x + y*y + z*z);}
	float length2(){return x*x + y*y + z*z;}

};

#endif
