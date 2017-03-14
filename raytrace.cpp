#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

#include "scene.h"
#include "sphere.h"
#include "triangle.h"
#include "directional_light.h"
#include "point_light.h"
#include "camera.h"
#include "plyModel.h"
#include "quadratic.h"

#define XSIZE 512
#define YSIZE 512

Colour frame_buffer[YSIZE][XSIZE];

float frand()
{
  int x;
  float f;

  x = rand();
  f = (float)(x & 0xffff);
  f = f/65536.0f;

  return f;
}

void write_framebuffer()
{
  int x, y;
  float r, g, b;

  ofstream fout;

  fout.open("output.ppm", ofstream::binary);

  fout << "P6" << std::endl << XSIZE << std::endl << YSIZE << std::endl << "255" << std::endl;

  for(y=YSIZE-1;y>=0;y-=1)
  {
    for(x=0;x<XSIZE;x+=1)
    {
      r = 255.0f * frame_buffer[y][x].getRed();
      g = 255.0f * frame_buffer[y][x].getGreen();
      b = 255.0f * frame_buffer[y][x].getBlue();
      if (r > 255.0) r = 255.0;
      if (g > 255.0) g = 255.0;
      if (b > 255.0) b = 255.0;
	  fout << (unsigned char)r << (unsigned char)g << (unsigned char)b;
    }
  }

  fout.close();
}

void clear_framebuffer()
{
  int x,y;

  for(y=0;y<YSIZE;y+=1)
  {
    for(x=0;x<XSIZE;x+=1)
    {
      frame_buffer[y][x].clear();
    }
  }
}

// The main raytacing entry point.

int main(int argc, const char *argv[])
{
  Scene *scene;
  Vertex ver;
  Vector vec;
  int x,y;
  int n;

  Colour cl;
  Vertex pp;
  float ca, cr, cg,cb;

  cout << "Starting ...\n";

  srand(1115);

  clear_framebuffer();

  // SETUP SCENE

  // Create a new scene to render
  scene = new Scene();
  PointLight *pl1;
  // Create and add a directional light to the scene
  Vertex ver1;
  Vector vec1;
  Colour cl1;
  ver1.set(0.5, 0.5, 0.0, 1.0);
  vec1.set(-0.5, -0.5, 1.0);
  cl1.set(1.0,0.0,0.0,1.0);

  pl1 = new PointLight(ver1, vec1, cl1);

  PointLight *pl2;
  Vertex ver2;
  Vector vec2;
  Colour cl2;
  // Create and add a directional light to the scene
  ver2.set(-0.5, -0.5, 0.0, 1.0);
  vec2.set(0.5, 0.5, 1.0);
  cl2.set(0.0,1.0,0.0,1.0);

  pl2 = new PointLight(ver2, vec2, cl2);

  scene->addLight(*pl2);
  scene->addLight(*pl1);

  Sphere *shape;
  Material *m;
  Vertex v1;
  Vertex v2;
  Vertex v3;

  // position

  v2.set(2.0, 0.0, 1.0, 1.0);
  v3.set(2.0, 2.0, 1.0, 1.0);

  int i;
  for (i = 0; i < 2; i++){
    Sphere *s;
    Material *m;
    Vertex p;

    // position
    p.set(0.0, 0.0, 3.0,1.0);

    // create with random radius
    s = new Sphere(p, 2.0);

    // create new material with shared random Ka and Kd
    m = new Material();

    m->ka.red = 0.1f;
    m->ka.green = 0.1f;
    m->ka.blue = 0.1f;
    m->kd.red = 0.5f;
    m->kd.green = 0.5f;
    m->kd.blue = 0.5f;
    m->kr.red =  0.0f;
    m->kr.green = 0.0f;
    m->kr.blue = 0.0f;
    m->ks.red = 0.5f;
    m->ks.green =  0.5f;
    m->ks.blue = 0.5;
    m->kt.red = 0.0;
    m->kt.green = 0.0;
    m->kt.blue = 0.0;
    m->n = 400.0;

    // set spheres material
    s->setMaterial(m);

    // as sphere to scene
    scene->addObject(*s);
  }


  // RAYTRACE SCENE

  cout << "Raytracing ...\n";
  Vertex position;
  position.set(0.0, 0.0, 0.0, 1.0);
  Vector lookat;
  lookat.set(0.0, 0.0, 1.0);
  Vector up;
  up.set(0.0, 1.0, 0.0);

  Camera *camera = new Camera(position, up, lookat);

  scene->setCamera(*camera);

  for(y=0;y<YSIZE;y+=1)
  {
    for(x=0;x<XSIZE;x+=1)
    {

      Colour col = camera->traceRay(XSIZE, YSIZE, x, y, scene);

      // Save result in frame buffer
      frame_buffer[y][x].red = col.red;
      frame_buffer[y][x].green = col.green;
      frame_buffer[y][x].blue = col.blue;
    }
  }

  // OUTPUT IMAGE

  cout << "Outputing ...\n";

  write_framebuffer();

  cout << "Done.\n";
}
