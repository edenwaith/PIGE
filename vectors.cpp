// To compile: c++ -Wall -o vectors vectors.cpp
// To run: ./vectors
// Written by Chad Armstrong on 6. July 2003 16:32

#include <iostream.h>
#include "Vector3D.h"

float dotProduct(Vector3D v1, Vector3D v2);

int main()
{

  float foo = 0.0;
  Vector3D vec1(0.0, -0.5, 1.0);
  Vector3D vec2(0.0, 1.0, 0.0);
  Vector3D vec3(0.0, 0.0, 0.0);
  Vector3D coord(0.5, 0.25, 1.0);
  Vector3D velocity(0.05, 0.0, -0.25);
  Vector3D normal(0.0, 0.0, -1.0);
 
  foo = coord.dotProduct(normal); // vec1.dotProduct(vec2);

  vec3 = coord + velocity;
  vec3 += velocity;
  //vec3 = vec1;
  //vec3 = -vec3;
  
  // cout << vec3.x << " " << vec3.y << " " << vec3.z << " " << endl;
  cout << "Foo: " << foo << endl;

  cout << "coord + velocity = ";
  vec3.printComponents();
 
  return 0;
}

float dotProduct(Vector3D v1, Vector3D v2)
{
	return ( v1.x*v2.x  +  v1.y*v2.y  +  v1.z*v2.z );
}