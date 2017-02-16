The files in this directory are a skeleton raytracing application written in C++.

There's VS15 project files in the Project1 directory.
Project1/example_output.ppm in an example of the image to expect.

Under linux compile and link all the cpp files with all the .h files available in the include path.


colour.cpp, colour.h		An RGBA colour class.

vector.cpp vector.h		A 3 element vector (x,y,z).
	
vertex.cpp vertex.h		A  4 element vertex (x,y,z,w).

ray.cpp ray.h			A ray consisting of position Vertex and direction Vector.

hit.cpp hit.h			Details of an object intersection.

material.cpp material.cpp	The material properties of a surface.

light.cpp light.h		The base class for lights

object.cpp object.h		The base class for objects

scene.cpp scene.h		A scene to be rendered consisting of objects and lights with top level tracing functions.

sphere.cpp sphere.h		A sphere object.

directional_light.cpp		A directional light.
directional_light.h

raytrace.cpp			The main entry point and framebuffer management.


By default the program generates a PPM image file called output.ppm.
The input scene is created procedurally and consists of one directional light and 10 random spheres.