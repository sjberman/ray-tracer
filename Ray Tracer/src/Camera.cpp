/*
 * Camera.cpp
 *
 *  Created on: Oct 3, 2015
 *      Author: Saylor Berman
 */

#include "../inc/Camera.h"
#include "../utils/LinearAlgebra.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

//read camera file, populate vectors
Camera::Camera(std::string& filename, std::vector<Model>& models, std::vector<Light>& lights, std::vector<Material>& materials){
	std::ifstream file(filename.c_str());
	if(!file.is_open()){
		std::cerr << "Failed to open file: " << filename << '\n';
		exit(-1);
	}

	std::string line;
	float w, x, y, z;

	std::getline(file, line);
	std::istringstream iss(line);
	iss >> x >> y >> z;
	focalPoint.push_back(x);
	focalPoint.push_back(y);
	focalPoint.push_back(z);
	iss.str("");
	iss.clear();

	std::getline(file, line);
	iss.str(line);
	iss >> x >> y >> z;
	lookAtPoint.push_back(x);
	lookAtPoint.push_back(y);
	lookAtPoint.push_back(z);
	iss.str("");
	iss.clear();

	std::getline(file, line);
	iss.str(line);
	iss >> x >> y >> z;
	VUP.push_back(x);
	VUP.push_back(y);
	VUP.push_back(z);
	iss.str("");
	iss.clear();

	std::getline(file, line);
	iss.str(line);
	iss >> x;
	focalLength = x;
	iss.str("");
	iss.clear();

	std::getline(file, line);
	iss.str(line);
	iss >> w >> x >> y >> z;
	boundingBox.push_back(w);
	boundingBox.push_back(x);
	boundingBox.push_back(y);
	boundingBox.push_back(z);

	file.close();

	if(focalLength <= 0){
		std::cerr << "Focal length must be greater than 0.\n";
		exit(-1);
	}

	if(boundingBox[0] > boundingBox[2] || boundingBox[1] > boundingBox[3]){
		std::cerr << "Minimum u and v coordinates must not be larger than maximum u and v coordinates.\n";
		exit(-1);
	}

	this->models = models;
	this->lights = lights;
	this->materials = materials;
	modelVerticesList.resize(models.size());
	modelFaceList.resize(models.size());
	modelSpheres.resize(models.size());
	for(unsigned int i = 0; i < models.size(); i++){
		modelVerticesList[i] = models[i].getVertices();
		modelFaceList[i] = models[i].getFaces();
		modelSpheres[i] = models[i].getSpheres();
	}

	n = normalize(subtractVector(lookAtPoint, focalPoint));
	u = normalize(vectorCrossProduct(VUP, n));
	v = vectorCrossProduct(n, u);

	equation.resize(3);
	for(int i = 0; i < 3; i++)
		equation[i].resize(3);

	reflectCount = 0;
	refractCount = 0;
}

Camera::~Camera(){

}

std::vector<float> Camera::solveLinearSystem(int j, std::vector<float>& L, std::vector<float>& U){
	Av = vertices[triangles[j][0]];
	Bv = vertices[triangles[j][1]];
	Cv = vertices[triangles[j][2]];

	equation[0][0] = Bv[0] - Av[0];
	equation[0][1] = Cv[0] - Av[0];
	equation[0][2] = -U[0];
	equation[0][3] = L[0] - Av[0];

	equation[1][0] = Bv[1] - Av[1];
	equation[1][1] = Cv[1] - Av[1];
	equation[1][2] = -U[1];
	equation[1][3] = L[1] - Av[1];

	equation[2][0] = Bv[2] - Av[2];
	equation[2][1] = Cv[2] - Av[2];
	equation[2][2] = -U[2];
	equation[2][3] = L[2] - Av[2];

	return gauss(equation);
}

//throw ray, check for intersection
std::vector<float> Camera::throwRay(std::vector<float>& L, std::vector<float> U){
	std::vector<float> color(3);
	std::fill(color.begin(), color.end(), 0);
	if(reflectCount >= 10){
		return color;
	}
	if(refractCount >= 10){
		return color;
	}

	float t = std::numeric_limits<float>::infinity();
	float faceIndex = -1, modelIndex = -1;
	bool found = false;

	//loop through triangles, try to find intersection
	for(unsigned int i = 0; i < models.size(); i++){
		if(!models[i].modelSphere->sphereIntersect(focalPoint, U))
			continue;

		triangles = modelFaceList[i];
		vertices = modelVerticesList[i];
		spheres = modelSpheres[i];

		//if(!spheres[0].sphereIntersect(focalPoint, U))
		//	return color;

		for(unsigned int j = 0; j < triangles.size(); j++){
			if(!spheres[j].sphereIntersect(focalPoint, U))
				continue;

			result = solveLinearSystem(j, L, U);

			//t
			if(result[2] <= 0)
				continue;
			//beta
			else if(result[0] < 0)
				continue;
			//gamma
			else if(result[1] < 0)
				continue;
			//beta + gamma
			else if(result[0] + result[1] > 1)
				continue;
			else{
				if(result[2] < t){
					t = result[2];
					faceIndex = j;
					modelIndex = i;
					found = true;
				}
			}
		}
	}
	if(found){
		color = lightIntersect(addVector(L, scalarVectorMultiply(t, U)), faceIndex, modelIndex, scalarVectorMultiply(-1, U));
	}
	return color;
}

//see if ray to polygon bounces and hits a light; if so, return color
std::vector<float> Camera::lightIntersect(std::vector<float> S, int faceIndex, int modelIndex, std::vector<float> V){
	std::vector<float> tmp, I, L, N, Q, B, R;
	std::vector<std::vector<float> > K;
	I.resize(3); std::fill(I.begin(), I.end(), 0);
	Q.resize(3);
	B.resize(3);
	N = models[modelIndex].faceNormal[faceIndex];
	if(dotProduct(N, V) < 0)
		N = scalarVectorMultiply(-1, N);
	float k, alpha, opacity;
	bool intersect = false, mats = false;

	//find material property for this polygon
	for(unsigned int i = 0; i < materials.size(); i++){
		if(materials[i].objNum == modelIndex && faceIndex >= materials[i].firstFace && faceIndex <= materials[i].lastFace){
			K = materials[i].diffuseMatrix;
			k = materials[i].k;
			alpha = materials[i].alpha;
			opacity = materials[i].opacity;
			mats = true;
			break;
		}
	}
	if(mats == false){
		K = materials[0].diffuseMatrix;
		K[0][0] = 0.5;
		K[1][1] = 0.5;
		K[2][2] = 0.5;
		k = 0;
		alpha = 1;
		opacity = 0;
	}

	//set ambient light
	for(unsigned int i = 0; i < lights.size(); i++){
		if(lights[i].ambient == true){
			B[0] = lights[i].red;
			B[1] = lights[i].green;
			B[2] = lights[i].blue;
		}
	}
	I = addVector(vectorMatrixMultiply(K, B), I);

	//loop through lights, find which light ray intersects with, find color
	for(unsigned int i = 0; i < lights.size(); i++){
		if(lights[i].ambient == true)
			continue;
		intersect = false;
		Q[0] = lights[i].x;
		Q[1] = lights[i].y;
		Q[2] = lights[i].z;
		L = normalize(subtractVector(Q, S));
		for(unsigned int j = 0; j < models.size(); j++){
			//if(!models[i].modelSphere->sphereIntersect(S, L))
			//	continue;

			triangles = modelFaceList[j];
			vertices = modelVerticesList[j];
			for(unsigned int k = 0; k < triangles.size(); k++){
				if(!spheres[j].sphereIntersect(S, L))
					continue;

				result = solveLinearSystem(k, Q, L);

				//t
				if(result[2] <= 0)
					continue;
				//beta
				else if(result[0] < 0)
					continue;
				//gamma
				else if(result[1] < 0)
					continue;
				//beta + gamma
				else if(result[0] + result[1] > 1)
					continue;
				else{
					intersect = true;
					break;
				}
			}
			if(intersect == true)
				break;
		}
		if(intersect == true)
			continue;

		float LN = dotProduct(L, N);
		if(LN < 0)
			LN = 0;
		R = subtractVector(scalarVectorMultiply(2 * LN, N), L);
		float VR = dotProduct(V, R);
		if(VR < 0)
			VR = 0;
		B[0] = lights[i].red;
		B[1] = lights[i].green;
		B[2] = lights[i].blue;
		tmp = addVector(scalarVectorMultiply(LN, vectorMatrixMultiply(K, B)), scalarVectorMultiply(pow(VR, alpha), scalarVectorMultiply(k, B)));
		I = addVector(tmp, I);
	}
	//recursive specular reflection
	/*std::vector<float> U = subtractVector(scalarVectorMultiply(2 * dotProduct(V, N), N), V);
	reflectCount++;
	I = addVector(scalarVectorMultiply(k, throwRay(S, U)), I);*/

	//refraction for translucent objects
	/*if(opacity != 0){
		refractCount++;
		I = addVector(scalarVectorMultiply(opacity, throwRay(S, V)), I);
	}*/

	return I;
}
