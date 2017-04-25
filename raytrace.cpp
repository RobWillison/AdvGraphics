#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include <time.h>
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
#include "octree.h"

#define XSIZE 256
#define YSIZE 256
#define NUM_THREADS 1

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

    float amountDone = (y - arguments->startY) / (float) (arguments->endY - arguments->startY);
    //printf("%f\n", amountDone);
  }

  return NULL;
}

// The main raytacing entry point.

int main(int argc, const char *argv[])
{
  time_t timerStart;
  time(&timerStart);

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
  ver1.set(0.0, 30.0, 5.0, 1.0);
  vec1.set(0.0, 6.0, 1.0);
  cl1.set(1.0,1.0,1.0,1.0);

  pl1 = new PointLight(ver1, vec1, cl1);

  PointLight *pl2;
  Vertex ver2;
  Vector vec2;
  Colour cl2;

  ver2.set(00.0, 0.0, 5.0, 1.0);
  vec2.set(0.0, 0.0, 1.0);
  cl2.set(1.0,1.0,1.0,1.0);

  pl2 = new PointLight(ver2, vec2, cl2);

  //scene->addLight(*pl1);
  scene->addLight(*pl2);

  Sphere *shape;
  Material *m;
  Vertex v1;
  Vertex v2;
  Vertex v3;

  // position

  v2.set(2.0, 0.0, 1.0, 1.0);
  v3.set(2.0, 2.0, 1.0, 1.0);

  for (int j = -5; j < 5; j++){
    for (int i = -5; i < 5; i++){
      Triangle *background1;
      Triangle *background2;

      Vertex tri1;
      tri1.set(1.0 * i, 1.0 * j, 5, 1.0f);
      Vertex tri2;
      tri2.set(1.0 * i + 1.0, 1.0 * j, 5, 1.0f);
      Vertex tri3;
      tri3.set(1.0 * i, 1.0 * j + 1, 5, 1.0f);

      Vertex tri4;
      tri4.set(1.0 * i, 1.0 * j + 1, 5, 1.0f);
      Vertex tri5;
      tri5.set(1.0 * i + 1, 1.0 * j, 5, 1.0f);
      Vertex tri6;
      tri6.set(1.0 * i + 1, 1.0 * j + 1, 5, 1.0f);

      m = new Material();

      m->ka.red = 0.1f;
      m->ka.green = 0.1f;
      m->ka.blue = 0.2f;
      m->kd.red = 0.2f;
      m->kd.green = 0.2f;
      m->kd.blue = 0.4f;
      m->kr.red =  0.1f;
      m->kr.green = 0.1f;
      m->kr.blue = 0.3f;
      m->ks.red = 0.1f;
      m->ks.green =  0.1f;
      m->ks.blue =  0.1f;
      if ( (i + j) % 2 == 0)
      {
        m->ka.red = 0.2f;
        m->ka.green = 0.1f;
        m->ka.blue = 0.1f;
        m->kd.red = 0.4f;
        m->kd.green = 0.2f;
        m->kd.blue = 0.2f;
        m->kr.red =  0.0f;
        m->kr.green = 0.0f;
        m->kr.blue = 0.0f;
        m->ks.red = 0.1f;
        m->ks.green =  0.1f;
        m->ks.blue =  0.1f;
      }

      m->kt.red = 0.0;
      m->kt.green = 0.0;
      m->kt.blue = 0.0;
      m->n = 400.f;

      background1 = new Triangle(tri1, tri3, tri2);
      background2 = new Triangle(tri4, tri6, tri5);
      background1->setMaterial(m);
      background2->setMaterial(m);
      scene->addObject(*background1);
      scene->addObject(*background2);
    }
  }
  m = new Material();
  m->ka.red = 0.1f;
  m->ka.green = 0.1f;
  m->ka.blue = 0.1;
  m->kd.red = 0.1f;
  m->kd.green = 0.1f;
  m->kd.blue = 0.1f;
  m->kr.red =  0.1f;
  m->kr.green = 0.1f;
  m->kr.blue = 0.1f;
  m->ks.red = 0.2f;
  m->ks.green =  0.2f;
  m->ks.blue =  0.2f;
  m->kt.red = 0.8;
  m->kt.green = 0.8;
  m->kt.blue = 0.8;
  m->n = 2.0f;


  //Load Model Into Scene10
  // modelPosition.y = 0;
  // modelPosition.z = 200;
  //
  // PlyModel *model = new PlyModel(filename);
  // model->AddToScene(scene, m, modelPosition, 200

  Sphere *sphere;
  Vertex p;
  p.set(0, 0, 2, 3.0);
  sphere = new Sphere(p, 1);
  sphere->setMaterial(m);

  Vertex planeVer1;
  planeVer1.set(-1, -1, 0, 1);
  Vertex planeVer2;
  planeVer2.set(-1, 1, 0, 1);
  Vertex planeVer3;
  planeVer3.set(1, -1, 0, 1);
  Vertex planeVer4;
  planeVer4.set(1, 1, 0, 1);
  Vertex planeVer5;
  planeVer5.set(1, -1, 0, 1);
  Vertex planeVer6;
  planeVer6.set(-1, 1, 0, 1);

  // Triangle *plane = new Triangle(planeVer1, planeVer2, planeVer3);
  // Triangle *plane2 = new Triangle(planeVer4, planeVer5, planeVer6);
  // plane->setMaterial(m);
  // plane2->setMaterial(m);
  //scene->addObject(*plane);
  // scene->addObject(*plane2);


  double terms[] = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 1.0, 0.0, -1.0};
  Quadratic *quadratic = new Quadratic(terms);
  quadratic->setMaterial(m);

  //scene->addObject(*quadratic);
  scene->addObject(*sphere);
  int i;
  //Add 10 random spheres to the scene
  // for (n = 0; n < 20; n += 1)
  // {
  //   Sphere *s;
  //   Material *m;
  //   Vertex p;
  //
  //   // position
  //   p.set(10 - 20 * frand(), 10 - 20 * frand(), 200 + 30 * frand(), 1.0);
  //
  //   // create with random radius
  //   s = new Sphere(p, 3.0f * frand());
  //
  //   // create new material with shared random Ka and Kd
  //   m = new Material();
  //
  //   cr = frand(); cg = frand(); cb = frand(); ca = frand();
  //
  //   m->ka.red = 0.2f * cr;
  //   m->ka.green = 0.2f * cg;
  //   m->ka.blue = 0.2f * cb;
  //   m->kd.red = 0.3f * cr;
  //   m->kd.green = 0.3f * cg;
  //   m->kd.blue = 0.3f * cb;
  //   m->kr.red =  0.3f * cr;
  //   m->kr.green = 0.3f * cg;
  //   m->kr.blue = 0.3f * cb;
  //   m->ks.red = 0.3f * cr;
  //   m->ks.green =  0.3f * cg;
  //   m->ks.blue = 0.3 * cb;
  //   m->kt.red = 0.0 * cr;
  //   m->kt.green = 0.0 * cg;
  //   m->kt.blue = 0.0  * cb;
  //   m->n = 400.0f;
  //
  //   // set spheres material
  //   s->setMaterial(m);
  //
  //   // as sphere to scene
  //   scene->addObject(*s);
  // }

  // RAYTRACE SCENE

  cout << "Creating Octree ...\n";
  Vertex topCorner;
  topCorner.set(5, 5, 10, 1);
  Vertex bottomCorner;
  bottomCorner.set(-5, -5, 0, 1);
  //Add objects into octree leafs
  scene->createOctree(topCorner, bottomCorner);
  //Use octree in raytracing

  cout << "Raytracing ...\n";
  Vertex position;
  position.set(0.0, 0.0, -20.0, 1.0);
  Vector lookat;
  lookat.set(0.0, 0.0, 1.0);
  Vector up;
  up.set(0.0, 1.0, 0.0);

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

  time_t timerEnd;
  time(&timerEnd);
  double seconds = difftime(timerEnd,timerStart);
  printf("%f\n", seconds);
}
