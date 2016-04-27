/*
 * LinearAlgebra.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: Saylor Berman
 */

#include "LinearAlgebra.h"

//multiplies two matrices (vector of vectors) together and returns a matrix (vector of vectors)
matrixvector matrixMultiply(const matrixvector& A, const matrixvector& B){
	int vrows, vcols;
	vrows = A.size();
	vcols = B[0].size();
	matrixvector res;
	res.resize(vrows);
	for(int i = 0; i < vrows; i++)
		res[i].resize(vcols);

	for(int i = 0; i < vrows; i++){
		for(int j = 0; j < vcols; j++){
			for(int k = 0; k < vrows; k++){
				res[i][j] += (A[i][k] * B[k][j]);
			}
		}
	}
	return res;
}

//multiplies a vector and matrix
std::vector<float> vectorMatrixMultiply(const matrixvector& A, const std::vector<float>& B){

	int vrows, vcols;
	vrows = A.size();
	vcols = A[0].size();
	result.resize(B.size());
	std::fill(result.begin(), result.end(), 0);

	for(int i = 0; i < vrows; i++){
		for(int j = 0; j < vcols; j++)
			result[i] += (A[i][j] * B[j]);
	}
	return result;
}

//multiplies two vectors
std::vector<float> vectorCrossProduct(const std::vector<float>& A, const std::vector<float>& B){

	result.resize(A.size());

	result[0] = (A[1] * B[2]) - (A[2] * B[1]);
	result[1] = (A[2] * B[0]) - (A[0] * B[2]);
	result[2] = (A[0] * B[1]) - (A[1] * B[0]);

	return result;
}

//dot product of two vectors
float dotProduct(const std::vector<float>& A, const std::vector<float>& B){
	return std::inner_product(A.begin(), A.end(), B.begin(), 0.0);
}

//multiplies a scalar by a vector
std::vector<float> scalarVectorMultiply(const float s, const std::vector<float>& A){

	result.resize(A.size());
	result[0] = s * A[0];
	result[1] = s * A[1];
	result[2] = s * A[2];

	return result;
}

//adds two vectors
std::vector<float> addVector(const std::vector<float>& A, const std::vector<float>& B){

	result.resize(A.size());
	result[0] = A[0] + B[0];
	result[1] = A[1] + B[1];
	result[2] = A[2] + B[2];

	return result;
}

//subtracts two vectors
std::vector<float> subtractVector(const std::vector<float>& A, const std::vector<float>& B){

	result.resize(A.size());
	result[0] = A[0] - B[0];
	result[1] = A[1] - B[1];
	result[2] = A[2] - B[2];

	return result;
}

//computes the distance between 2 points
float distance(const std::vector<float>& A, const std::vector<float>& B){
	return sqrt(pow(A[0] - B[0], 2) + pow(A[1] - B[1], 2) + pow(A[2] - B[2], 2));
}

//normalizes a vector
std::vector<float> normalize(const std::vector<float>& A){

	result.resize(A.size());
	float length = sqrt(pow(A[0], 2) + pow(A[1], 2) + pow(A[2], 2));
	result[0] = A[0]/length;
	result[1] = A[1]/length;
	result[2] = A[2]/length;

	if(std::isnan(result[0]))
		result[0] = 0;
	if(std::isnan(result[1]))
		result[1] = 0;
	if(std::isnan(result[2]))
		result[2] = 0;

	return result;
}

//calculate the surface normal for a face
std::vector<float> surfaceNormal(const std::vector<float>& A, const std::vector<float>& B, const std::vector<float>& C){

	std::vector<float> result, tmp1, tmp2;
	result.resize(A.size());
	tmp1 = subtractVector(B, A);
	tmp2 = subtractVector(C, B);

	result = normalize(vectorCrossProduct(tmp1, tmp2));

	return result;
}

//solve system of linear equations
std::vector<float> gauss(std::vector<std::vector<float> >& A){
	int n = A.size();
	for(int i = 0; i < n; i++){
		//search for max in this column
		float max = abs(A[i][i]);
		int maxRow = i;
		for(int j = i+1; j < n; j++){
			if(abs(A[j][i]) > max){
				max = abs(A[j][i]);
				maxRow = j;
			}
		}
		//swap max row with current row
		for(int j = i; j < n+1; j++){
			float tmp = A[maxRow][j];
			A[maxRow][j] = A[i][j];
			A[i][j] = tmp;
		}
		//make all rows below this 0 for current column
		for(int j = i+1; j < n; j++){
			float c = -A[j][i]/A[i][i];
			for(int k = i; k < n+1; k++){
				if(i == k)
					A[j][k] = 0;
				else
					A[j][k] += c * A[i][k];
			}
		}
	}
	//solve Ax = b for an upper triangular matrix A
	result.resize(n);
	for(int i = n-1; i >= 0; i--){
		result[i] = A[i][n]/A[i][i];
		for(int j = i-1; j >= 0; j--)
			A[j][n] -= A[j][i] * result[i];
	}
	return result;
}

//transposes a matrix
matrixvector transpose(const matrixvector& vector){

	int cols = vector.size();
	int rows = vector[0].size();
	matrixvector res(rows);

	for(int i = 0; i < cols; i++){
		for(int j = 0; j < rows; j++){
			res[j].push_back(vector[i][j]);
		}
	}

	return res;
}

//returns the mean vertex (as a string) from a vector of vertices
std::string vertexMean(matrixvector& vector){

	int vrows = vector.size();
	float x = 0, y = 0, z = 0;

	for(int i = 0; i < vrows; i++){
		x = x + vector[i][0];
		y = y + vector[i][1];
		z = z + vector[i][2];
	}
	x = x/vrows;
	y = y/vrows;
	z = z/vrows;

	std::ostringstream oss;
	oss << "{" << x << ", " << y << ", " << z << "}";

	return oss.str();
}

//returns the bounding box(as a string) for the model
std::vector<float> boundingBox(matrixvector& vector){

	std::vector<float> box(6);
	int vrows = vector.size();
	float maxX = 0, maxY = 0, maxZ = 0;
	float minX = vector[0][0], minY = vector[0][0], minZ = vector[0][0];

	for(int i = 0; i < vrows; i++){
		if(maxX < vector[i][0])
			maxX = vector[i][0];
		if(maxY < vector[i][1])
			maxY = vector[i][1];
		if(maxZ < vector[i][2])
			maxZ = vector[i][2];

		if(minX > vector[i][0])
			minX = vector[i][0];
		if(minY > vector[i][1])
			minY = vector[i][1];
		if(minZ > vector[i][2])
			minZ = vector[i][2];
	}

	box[0] = maxX; box[1] = maxY; box[2] = maxZ;
	box[3] = minX; box[4] = minY; box[5] = minZ;

	return box;
}
