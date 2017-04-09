// Sphere object

#include <stdio.h>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "plyModel.h"


struct VertexData {
  float x;
  float y;
  float z;
  float confidence;
  float intensity;
};

double * PlyModel::ParseLine(std::string line)
{
  int itemNum = std::count(line.begin(), line.end(), ' ') + 1;

  double * items = new double[itemNum];
  int index = 0;

  for (int i = 0; i < itemNum; i++)
  {
    index = line.find(" ");

    if (index == std::string::npos)
    {
      items[i] = atof(line.c_str());
      break;
    } else {
      items[i] = atof(line.substr(0, index).c_str());
    }

    line = line.substr(index + 1);
  }

  return items;
}

Model PlyModel::ParseFile(char fileName[])
{
  std::ifstream file(fileName);
  std::string str;

  int vertexNum;
  int faceNum;

  bool header = true;

  Model model;
  int index = 0;
  while (std::getline(file, str))
  {
    if (header) {
      if (!str.compare(0, 14, "element vertex"))
      {
        int vertexs = atoi(str.substr(15).c_str());
        model.vertexNum = vertexs;
        model.vertexs = std::vector<Vertex>(model.vertexNum);
      }
      if (!str.compare(0, 12, "element face"))
      {
        int faces = atoi(str.substr(13).c_str());
        model.faceNum = faces;
        model.faces = std::vector<Triangle>(model.faceNum);
      }
      if (str == "end_header")
      {
        header = false;
      }
    } else if (index < model.vertexNum) {
      double *items = this->ParseLine(str);
      model.vertexs[index].x = items[0];
      model.vertexs[index].y = items[1];
      model.vertexs[index].z = items[2];
      model.vertexs[index].w = 1;
      index++;
    } else {
      double *items = this->ParseLine(str);

      model.faces[index - model.vertexNum].set(
        model.vertexs[(int) items[1]],
        model.vertexs[(int) items[2]],
        model.vertexs[(int) items[3]]
      );

      index++;
    }
  }

  return model;
}

void PlyModel::AddToScene( Scene *scene, Material *material, Vertex position, int scale)
{

  for (int i = 0; i < model.faceNum; i++)
  {

    model.faces[i].setMaterial(material);
    model.faces[i].scale(scale);
    model.faces[i].PositionAt(position);

    scene->addObject(model.faces[i]);
  }
}

PlyModel::PlyModel(char fileName[])
{
  model = this->ParseFile(fileName);

}
