#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>       /* acos */
#include <pthread.h>
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
#define NUM_THREADS 16

Colour frame_buffer[YSIZE][XSIZE];

struct argsStruct{
  int startY;
  int endY;
  Camera *camera;
  Scene *scene;
};

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

void *rayTraceRows(void *args)
{
  argsStruct *arguments = (argsStruct *) args;
  int y,x;
  for(y = arguments->startY; y < arguments->endY; y += 1)
  {
    for(x=0;x<XSIZE;x+=1)
    {

      Colour col = arguments->camera->traceRay(XSIZE, YSIZE, x, y, arguments->scene);

      // Save result in frame buffer
      frame_buffer[y][x].red = col.red;
      frame_buffer[y][x].green = col.green;
      frame_buffer[y][x].blue = col.blue;
    }
  }

  return NULL;
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

  srand(1117);

  clear_framebuffer();

  // SETUP SCENE

  // Create a new scene to render
  scene = new Scene();
  PointLight *pl1;
  // Create and add a directional light to the scene
  Vertex ver1;
  Vector vec1;
  Colour cl1;
  ver1.set(40.0, 0.0, 30.0, 1.0);
  vec1.set(-0.2, 0.0, 1.0);
  cl1.set(1.0,1.0,1.0,1.0);

  pl1 = new PointLight(ver1, vec1, cl1);

  PointLight *pl2;
  Vertex ver2;
  Vector vec2;
  Colour cl2;

  scene->addLight(*pl1);
  //scene->addLight(*pl1);

  Sphere *shape;
  Material *m;
  Vertex v1;
  Vertex v2;
  Vertex v3;

  // position

  v2.set(2.0, 0.0, 1.0, 1.0);
  v3.set(2.0, 2.0, 1.0, 1.0);

  Triangle *background1;
  Triangle *background2;

  Vertex tri1;
  tri1.set(100.0, 100.0, 230.0, 1.0f);
  Vertex tri2;
  tri2.set(-100.0, -100.0, 230.0, 1.0f);
  Vertex tri3;
  tri3.set(100.0, -100.0, 230.0, 1.0f);
  Vertex tri4;
  tri4.set(100.0, 100.0, 230.0, 1.0f);
  Vertex tri5;
  tri5.set(-100.0, 100.0, 230.0, 1.0f);
  Vertex tri6;
  tri6.set(-100.0, -100.0, 230.0, 1.0f);


  cr = frand(); cg = frand(); cb = frand(); ca = frand();
  m = new Material();
  m->ka.red = cr * 1.0f;
  m->ka.green = cg * 0.1f;
  m->ka.blue = cb * 0.1f;
  m->kd.red = cr * 0.2f;
  m->kd.green = cg * 0.2f;
  m->kd.blue = cb * 0.2f;
  m->kr.red =  0.3f;
  m->kr.green = 0.3f;
  m->kr.blue = 0.3f;
  m->ks.red = 0.5f;
  m->ks.green =  0.5f;
  m->ks.blue = 0.5f;
  m->kt.red = 0.0f;
  m->kt.green = 0.0f;
  m->kt.blue = 0.0f;
  m->n = 400.0f;

  background1 = new Triangle(tri3, tri2, tri1);
  background2 = new Triangle(tri6, tri5, tri4);
  background1->setMaterial(m);
  background2->setMaterial(m);
  scene->addObject(*background1);
  scene->addObject(*background2);



  int i;
  // Add 10 random spheres to the scene
  for (n = 0; n < 1; n += 1)
  {
    Sphere *s;
    Material *m;
    Vertex p;

    // position
    p.set(5 - 10.0 * frand(),5 -  10.0 * frand(), 100 + 20.0 * frand(), 1.0);

    // create with random radius
    s = new Sphere(p, 30.0f);

    // create new material with shared random Ka and Kd
    m = new Material();

    cr = frand(); cg = frand(); cb = frand(); ca = frand();

    m->ka.red = 0.1f;
    m->ka.green = 0.1f;
    m->ka.blue = 0.1f;
    m->kd.red = 0.1f;
    m->kd.green = 0.1f;
    m->kd.blue = 0.1f;
    m->kr.red =  0.6f;
    m->kr.green = 0.6f;
    m->kr.blue = 0.6f;
    m->ks.red = 0.6f;
    m->ks.green =  0.6f;
    m->ks.blue = 0.6;
    m->kt.red = 0.8;
    m->kt.green = 0.8;
    m->kt.blue = 0.8;
    m->n = 2.0f;

    // set spheres material
    s->setMaterial(m);

    // as sphere to scene
    scene->addObject(*s);
  }

  // RAYTRACE SCENE

  cout << "Raytracing ...\n";
  Vertex position;
  position.set(0.0, 0.0, -10.0, 1.0);
  Vector lookat;
  lookat.set(0.0, 0.0, 1.0);
  Vector up;
  up.set(1.0, 0.0, 0.0);

  Camera *camera = new Camera(position, up, lookat);

  scene->setCamera(*camera);

  pthread_t threads[16];
  argsStruct args[16];
  int code;
  int thread;

  for( thread=0; thread < NUM_THREADS; thread++ ){
    cout << "Spawn Thread" << thread << "\n";

    args[thread].startY = (YSIZE / NUM_THREADS) * thread;
    args[thread].endY = (YSIZE / NUM_THREADS) * (thread + 1);
    args[thread].camera = camera;
    args[thread].scene = scene;
    code = pthread_create(&threads[thread], NULL, rayTraceRows, (void *)&args[thread]);

    if (code){
      cout << "Error\n";
      exit(-1);
    }
  }

  for( thread=0; thread < NUM_THREADS; thread++ ){
    cout << "Waiting On Thread" << thread << "\n";
    pthread_join(threads[thread], NULL);
  }

  // OUTPUT IMAGE

  cout << "Outputing ...\n";

  write_framebuffer();

  cout << "Done.\n";
}
