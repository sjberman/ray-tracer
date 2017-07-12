/*
 * Material.h
 *
 *  Created on: Oct 25, 2015
 *      Author: Saylor Berman
 */

#ifndef SRC_MATERIAL_H_
#define SRC_MATERIAL_H_

#include "Model.h"
#include <vector>

class Material {
public:
  Material(int objNum, int firstFace, int lastFace, float diff1, float diff2,
           float diff3, float k, float alpha, float opacity,
           std::vector<Model> &models);
  virtual ~Material();

  int objNum, firstFace, lastFace;
  float k, alpha, opacity;
  std::vector<std::vector<float>> diffuseMatrix;

private:
};

#endif /* SRC_MATERIAL_H_ */
