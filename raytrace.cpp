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
  PointLight *pl;
  Colour cl;
  Vertex pp;
  float ca, cr, cg,cb;

  cout << "Starting ...\n";

  srand(1115);

  clear_framebuffer();

  // SETUP SCENE

  // Create a new scene to render
  scene = new Scene();

  // Create and add a directional light to the scene
  ver.set(0.0,0.0,-2.0, 1.0);
  vec.set(0.0, 0.0, 1.0);
  cl.set(1.0,1.0,1.0,1.0);
  pp.set(-50.0, 50.0, -48.25, 1.0);

  pl = new PointLight(ver, vec, cl);

  scene->addLight(*pl);

  Sphere *shape;
  Material *m;
  Vertex v1;
  Vertex v2;
  Vertex v3;

  // position

  v2.set(2.0, 0.0, 1.0, 1.0);
  v3.set(2.0, 2.0, 1.0, 1.0);

  // create with random radius
  double terms[] = {1.0, -1.0, +001.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  int i;
  for (i = 0; i < 10; i++){
    Sphere *s;
    Material *m;
    Vertex p;

    // position
    p.set(frand()-0.5,frand()-0.5,frand()+1.0,1.0);

    // create with random radius
    s = new Sphere(p, frand()/1.0f);

    // create new material with shared random Ka and Kd
    m = new Material();

    cr = frand(); cg = frand(); cb = frand(); ca = frand();

    m->ka.red = cr * 0.1f;
    m->ka.green = cg * 0.1f;
    m->ka.blue = cb * 0.1f;
    m->kd.red = cr * 0.5f;
    m->kd.green = cg * 0.5f;
    m->kd.blue = cb * 0.5f;
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
  position.set(0.0, 5.0, -1.0, 1.0);
  Vertex windowBottomRight;
  windowBottomRight.set(2.0, -2.0, 0.0, 1.0);
  Vertex windowTopLeft;
  windowTopLeft.set(-2.0, 2.0, 0.0, 1.0);
  Vector lookat;
  lookat.set(0.0, 1.0, 1.0);
  Vector up;
  up.set(0.0, 1.0, 0.0);

  Camera *camera = new Camera(position, up, lookat, windowTopLeft, windowBottomRight);

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
