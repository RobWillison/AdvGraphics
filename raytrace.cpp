#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

#include "scene.h"
#include "sphere.h"
#include "triangle.h"
#include "directional_light.h"
#include "camera.h"
#include "plyModel.h"

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
  Vector v;
  int x,y;
  int n;
  DirectionalLight *dl;
  Colour cl;
  Vertex pp;
  float ca, cr, cg,cb;

  cout << "Starting ...\n";
  char fileName[] = "Models/bunny.ply";
  PlyModel *plyModel = new PlyModel(fileName);

  srand(3115);

  clear_framebuffer();

  // SETUP SCENE

  // Create a new scene to render
  scene = new Scene();

  // Create and add a directional light to the scene
  v.set(-1.0,-1.0,1.0);
  cl.set(1.0,1.0,1.0,1.0);
  pp.set(-50.0, 50.0, -48.25, 1.0);

  dl = new DirectionalLight(v, cl);

  scene->addLight(*dl);

  Triangle *triangle;
  Material *m;
  Vertex v1;
  Vertex v2;
  Vertex v3;

  // position
  v1.set(0.0, 0.0, 1.0, 1.0);
  v2.set(2.0, 0.0, 1.0, 1.0);
  v3.set(2.0, 2.0, 1.0, 1.0);

  // create with random radius
  triangle = new Triangle(v1, v2, v3);

  // create new material with shared random Ka and Kd
  m = new Material();

  cr = frand(); cg = frand(); cb = frand(); ca = frand();

  m->ka.red = cr * 0.5f;
  m->ka.green = cg * 0.5f;
  m->ka.blue = cb * 0.5f;
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
  triangle->setMaterial(m);

  // as sphere to scene
  scene->addObject(*triangle);


  // RAYTRACE SCENE

  cout << "Raytracing ...\n";
  Vertex position;
  position.set(0.0, 0.0, -2.0, 1.0);
  Vertex windowBottomRight;
  windowBottomRight.set(2.0, -2.0, 0.0, 1.0);
  Vertex windowTopLeft;
  windowTopLeft.set(-2.0, 2.0, 0.0, 1.0);
  Vector lookat;
  lookat.set(0.0, 0.0, 1.0);
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
