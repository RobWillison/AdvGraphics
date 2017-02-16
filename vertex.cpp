#include "vertex.h"

// initialise vectors to zero.
Vertex::Vertex()
{
  x = 0.0;
  y = 0.0;
  z = 0.0;
  w = 0.0;
}

// set the elements of a vertex
void Vertex::set(double px, double py, double pz, double pw)
{
  x = px;
  y = py;
  z = pz;
  w = pw;
}

