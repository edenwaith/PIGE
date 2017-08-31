/*
 *  Vector3D.h
 *  PIGE
 *
 *  Created by Administrator on Sun Jul 06 2003.
 *  Copyright (c) 2003 Edenwaith. All rights reserved.
 *
 */

#include <math.h>

class Vector3D
{
    public:
    
        float x, y, z;
        
        // constructors and destructors
        Vector3D() {}
        Vector3D(const float fx,const float fy,const float fz) { x=fx; y=fy; z=fz; }
        ~Vector3D() {}
        
        // set and get methods
        void setVector(float, float, float);
        Vector3D vector();
        
        // vector operations
        Vector3D operator+(const Vector3D & v) { return Vector3D(x+v.x,y+v.y,z+v.z); }
        Vector3D operator-(const Vector3D & v) { return Vector3D(x-v.x,y-v.y,z-v.z); }
        Vector3D operator*(const Vector3D & v) { return Vector3D(x*v.x,y*v.y,z*v.z); }
        Vector3D operator/(const Vector3D & v) { return Vector3D(x/v.x,y/v.y,z/v.z); }
        Vector3D operator*(const float f) { return Vector3D(x*f, y*f, z*f);	}
        Vector3D operator/(const float f) { return Vector3D(x/f, y/f, z/f); }
        
        Vector3D operator-() { return Vector3D(-x,-y,-z); }
        
        Vector3D operator+=(const Vector3D & v) { *this = *this + v; return *this; }
        
        // vector mathematical functions
        void crossProduct(Vector3D, Vector3D);
        float dotProduct(Vector3D &, Vector3D &);
        float dotProduct(Vector3D &);
        void normalize();
        
	float vectorMagnitude() { return (float)sqrt(x*x + y*y + z*z); }
        
        void printComponents();

};

void Vector3D :: setVector(float new_x, float new_y, float new_z)
{
    x = new_x;
    y = new_y;
    z = new_z;
}

Vector3D Vector3D :: vector()
{
    return (Vector3D(x, y, z));
}


void Vector3D :: crossProduct(Vector3D v1, Vector3D v2)
{
	x = ( v1.y * v2.z ) - ( v1.z * v2.y );
	y = ( v1.z * v2.x ) - ( v1.x * v2.z );
	z = ( v1.x * v2.y ) - ( v1.y * v2.x );
}

float Vector3D :: dotProduct(Vector3D & v1, Vector3D & v2)
{
	return ( v1.x*v2.x  +  v1.y*v2.y  +  v1.z*v2.z );
}

float Vector3D :: dotProduct(Vector3D & v2) 
{	
    return ( x*v2.x + y*v2.y + z*v2.z );
}

void Vector3D :: normalize()
{
	float len = vectorMagnitude();
        
        // if the vector has no magnitude (0), then
        // return since normalizing a 0 vector will
        // result in an error by trying to divide by 0.
	if (len == 0) return;
        
	len = 1.0f/len;
	
	x *= len;
	y *= len;
	z *= len;
}

void Vector3D :: printComponents()
{
    cout << "x: " << x << " y: " << y << " z: " << z << endl;
}