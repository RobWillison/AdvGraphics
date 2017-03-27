#include <math.h>

#include "scene.h"
#include <stdio.h>
#include "camera.h"

Scene::Scene()
{
  scache = 0;
  obj_list = (Object *)0;
  light_list = (Light *)0;
}

void Scene::setCamera(Camera &cam)
{
  camera = &cam;
}

void Scene::addObject(Object &obj)
{
	// add object to list of objects in scene
  obj.link(obj_list);
  obj_list = &obj;
}

void Scene::addLight(Light &lt)
{
	// add light to list of objects in scene
  lt.link(light_list);
  light_list = &lt;
}

Colour Scene::raytrace(Ray &ray, int level)
{
  float ta,t;
  Colour col;
  Object *obj;
  Object *closest;
  Light *lt;
  Hit   hit;
  Vertex position;
  Vector normal;


  if (level == 0)
  {
    col.clear();
    return col; // stop if recursed deep enough
  }

  // TEST EACH OBJECT AGAINST RAY, FIND THE CLOSEST

  t = 1000000000.0; // a long way aways
  closest = (Object *)0;
  obj = obj_list;

  while (obj != (Object *)0)
  {
    if(obj->intersect(ray, &hit) == true)
    {
      if (hit.t < t)
      {
      	closest = hit.obj;
      	t = hit.t;
      	normal = hit.n;
      	position = hit.p;
      }
    }

    obj = obj->next();
  }

  col.clear();

  if (closest != (Object *)0)
  {
    lt = light_list;

    Colour ocol;

    Colour ka = closest->obj_mat->ka;
    Colour kd = closest->obj_mat->kd;
    Colour ks = closest->obj_mat->ks;
    Colour kr = closest->obj_mat->kr;
    Colour kt = closest->obj_mat->kt;
    Ray sray;


    while (lt != (Light *)0)
    {
      Vector ldir;
      Vector xldir;
      Colour lcol;

      // add shadow test here

      // calculate diffuse component

      lt->getLightProperties(position, &xldir, &lcol);

      xldir.normalise();

      float dlc = xldir.dot(normal);

      if (dlc < 0.0)
      {
	       dlc = 0.0;
      }

      // calculate specular component here
      Vector reflection;

      reflection.x = xldir.x - 2.0 * (xldir.dot(normal)) * normal.x;
      reflection.y = xldir.y - 2.0 * (xldir.dot(normal)) * normal.y;
      reflection.z = xldir.z - 2.0 * (xldir.dot(normal)) * normal.z;

      reflection.normalise();

      Vector view;
      Vertex cameraPos = camera->getPosition();
      view.x = cameraPos.x - position.x;
      view.y = cameraPos.y - position.y;
      view.z = cameraPos.z - position.z;

      view.normalise();
      double reflectionDiff = reflection.dot(view);

      float slc = 0.0;

      if (reflectionDiff <= -0.0)
      {
        slc = pow(reflectionDiff, 2);
      }


      // combine components

      col.red += ka.red + lcol.red*(dlc * kd.red + slc * ks.red);
      col.green += ka.green + lcol.green*(dlc * kd.green + slc * ks.green);
      col.blue += ka.blue + lcol.blue*(dlc * kd.blue + slc * ks.blue);

      lt = lt->next(); // next light
    }

    // add reflected rays here

    // add refracted rays here
  }

  return col;
}


bool Scene::shadowtrace(Ray &ray, double tlimit)
{
  Object *obj;
  Hit   hit;

  // check for shadow intersections

  return false;
}
