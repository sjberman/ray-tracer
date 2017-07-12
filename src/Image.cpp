/*
 * Image.cpp
 *
 *  Created on: Oct 3, 2015
 *      Author: Saylor Berman
 */

#include "../inc/Image.h"
#include "../utils/LinearAlgebra.h"
#include <ctime>
#include <iostream>
#include <omp.h>

Image::Image(const Camera &camera, int rows, int cols) : camera(camera) {

  // set pixel matrix size
  pixels.resize(rows);
  for (int i = 0; i < rows; i++)
    pixels[i].resize(cols);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      pixels[i][j].resize(3);
    }
  }
}

Image::~Image() {}

void Image::generate() {
  std::vector<float> L(3), color(3), tmp(3);
  int count = 0;
  srand(time(NULL));
#pragma omp parallel for ordered schedule(static)
  for (unsigned int i = 0; i < pixels.size(); i++) {
    for (unsigned int j = 0; j < pixels[0].size(); j++) {
      tmp[0] = 0;
      tmp[1] = 0;
      tmp[2] = 0;
      count++;
      // std::cout << count << std::endl;
      // adjust coordinates to put 0,0 in middle of plane
      int x = i + camera.boundingBox[0];
      int y = j + camera.boundingBox[1];

      // calculate pixel point L, throw 5 rays through each pixel for
      // anti-aliasing
      float random = 0;
      for (int k = 0; k < 5; k++) {
        random = ((float)rand()) / RAND_MAX - 0.5;
        float a = (float)(x + random);
        float b = (float)(y + random);
        L = addVector(addVector(addVector(camera.focalPoint,
                                          scalarVectorMultiply(
                                              camera.focalLength, camera.n)),
                                scalarVectorMultiply(a, camera.u)),
                      scalarVectorMultiply(b, camera.v));
        camera.reflectCount = 0;
        camera.refractCount = 0;
#pragma omp ordered
        tmp = addVector(
            camera.throwRay(L, normalize(subtractVector(L, camera.focalPoint))),
            tmp);
      }
      tmp[0] = tmp[0] / 5;
      tmp[1] = tmp[1] / 5;
      tmp[2] = tmp[2] / 5;
      color = tmp;

      /*L = addVector(addVector(addVector(camera.focalPoint,
      scalarVectorMultiply(camera.focalLength, camera.n)),
      scalarVectorMultiply(x, camera.u)), scalarVectorMultiply(y, camera.v));
      camera.reflectCount = 0;
      camera.refractCount = 0;
      #pragma omp ordered
      color = camera.throwRay(L, normalize(subtractVector(L,
      camera.focalPoint)));*/

      pixels[i][j][0] = color[0];
      pixels[i][j][1] = color[1];
      pixels[i][j][2] = color[2];
    }
  }
  scalePixels();
}

// scales the floating point pixels to whole integers
void Image::scalePixels() {
  float max = 0, min = 1;
  for (unsigned int i = 0; i < pixels.size(); i++) {
    for (unsigned int j = 0; j < pixels[0].size(); j++) {
      for (int k = 0; k < 3; k++) {
        if (pixels[i][j][k] > max) {
          max = pixels[i][j][k];
        }
        if (pixels[i][j][k] < min && pixels[i][j][k] > 0)
          min = pixels[i][j][k];
      }
    }
  }

  if (max == min)
    min = 0;
  for (unsigned int i = 0; i < pixels.size(); i++) {
    for (unsigned int j = 0; j < pixels[0].size(); j++) {
      for (int k = 0; k < 3; k++) {
        if (pixels[i][j][k] > 0)
          pixels[i][j][k] = int(255 * (pixels[i][j][k] - min) / (max - min));
      }
    }
  }
}

std::vector<std::vector<std::vector<float>>> Image::getPixels() const {
  return pixels;
}
