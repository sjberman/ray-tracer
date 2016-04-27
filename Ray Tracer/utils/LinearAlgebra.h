/*
 * LinearAlgebra.h
 *
 *  Created on: Sep 16, 2015
 *      Author: Saylor Berman
 */

#ifndef UTILS_LINEARALGEBRA_H_
#define UTILS_LINEARALGEBRA_H_

#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <numeric>

typedef std::vector<std::vector<float> > matrixvector;

matrixvector matrixMultiply(const matrixvector& A, const matrixvector& B);
std::vector<float> vectorMatrixMultiply(const matrixvector& A, const std::vector<float>& B);
std::vector<float> vectorCrossProduct(const std::vector<float>& A, const std::vector<float>& B);
float dotProduct(const std::vector<float>& A, const std::vector<float>& B);
std::vector<float> scalarVectorMultiply(const float s, const std::vector<float>& A);
std::vector<float> addVector(const std::vector<float>& A, const std::vector<float>& B);
std::vector<float> subtractVector(const std::vector<float>& A, const std::vector<float>& B);
float distance(const std::vector<float>& A, const std::vector<float>& B);
std::vector<float> normalize(const std::vector<float>& A);
std::vector<float> surfaceNormal(const std::vector<float>& A, const std::vector<float>& B, const std::vector<float>& C);
std::vector<float> gauss(std::vector<std::vector<float> >& A);
matrixvector transpose(const matrixvector& vector);
std::string vertexMean(matrixvector& vector);
std::vector<float> boundingBox(matrixvector& vector);

static std::vector<float> result;

#endif /* UTILS_LINEARALGEBRA_H_ */
