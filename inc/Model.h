/*
 * Model.h
 *
 *  Created on: Sep 13, 2015
 *      Author: Saylor Berman
 */

#ifndef MODEL_H_
#define MODEL_H_

#include "Sphere.h"
#include <string>
#include <vector>

class Model {
public:
  Model(std::vector<std::vector<float>> &vertices,
        std::vector<std::vector<float>> &faces);
  virtual ~Model();
  // Sphere* setSpheres(std::vector<std::vector<float> >& vertices);
  std::string getInfo();
  std::vector<std::vector<float>> getVertices();
  std::vector<std::vector<float>> getFaces();
  std::vector<Sphere> getSpheres();
  void scale(float x, float y, float z);
  void translate(float x, float y, float z);
  void rotate(float x, float y, float z, float theta);

  std::vector<std::vector<float>> faceNormal;
  int fcount;
  Sphere *modelSphere;

private:
  std::vector<std::vector<float>> vertices, faces, identityMatrix, tmpVertices;
  std::vector<Sphere> spheres;
  void setTmpVertices(bool choice);
};

#endif /* MODEL_H_ */
