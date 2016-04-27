/*
 * Model.cpp
 *
 *  Created on: Sep 13, 2015
 *      Author: Saylor Berman
 */

#include "../inc/Model.h"
#include "../utils/LinearAlgebra.h"
#include <sstream>
#include <iostream>

Model::Model(std::vector<std::vector<float> > &vertices, std::vector<std::vector<float> > &face){

	this->vertices = vertices;
	tmpVertices = vertices;

	//turn polygons into triangles
	fcount = face[0].size();
	int tcount = (fcount - 2) * face.size(), i = 0, k = 0;
	faces.resize(tcount);
	while(i < tcount){
		for(unsigned int j = 0; j < face.size(); j++){
			for(int h = 1; h < fcount; h++){
				if(k == 0){
					faces[i].push_back(face[j][0]);
					h--;
				}
				else
					faces[i].push_back(face[j][h]);
				k++;
				if(k == 3){
					i++;
					k = 0;
					if(h != fcount - 1)
						h--;
				}
			}
		}
	}

	//Sphere* s = setSpheres(vertices);
	//spheres.push_back(*s);
	std::vector<float> centroid(3), tmp(3);
	std::vector<float> box;
	box = boundingBox(vertices);
	//create sphere around whole model
	centroid[0] = (box[0] + box[3])/2;
	centroid[1] = (box[1] + box[4])/2;
	centroid[2] = (box[2] + box[5])/2;
	tmp[0] = box[0]; tmp[1] = box[1]; tmp[2] = box[2];
	float radius = ::distance(tmp, centroid);
	modelSphere = new Sphere(centroid, radius);

	//create a sphere around each triangle
	for(unsigned int i = 0; i < faces.size(); i++){
			centroid[0] = (vertices[faces[i][0]][0] + vertices[faces[i][1]][0] + vertices[faces[i][2]][0])/3;
			centroid[1] = (vertices[faces[i][0]][1] + vertices[faces[i][1]][1] + vertices[faces[i][2]][1])/3;
			centroid[2] = (vertices[faces[i][0]][2] + vertices[faces[i][1]][2] + vertices[faces[i][2]][2])/3;
			float tmp = std::max(::distance(vertices[faces[i][0]], centroid), ::distance(vertices[faces[i][1]], centroid));
			float radius = std::max(::distance(vertices[faces[i][2]], centroid), tmp);
			Sphere s(centroid, radius);
			spheres.push_back(s);
	}

	//calculate surfaceNormal
	int f2count = faces[0].size();
	faceNormal.resize(faces.size());
	for(unsigned int i = 0; i < faces.size(); i++){
		for(int j = 0; j < f2count - 2; j++){
			faceNormal[i] = surfaceNormal(vertices[faces[i][j]], vertices[faces[i][j+1]], vertices[faces[i][j+2]]);
			if(faceNormal[i][0] == 0 && faceNormal[i][1] == 0 && faceNormal[i][2] == 0){
				continue;
			}
			else
				break;
		}
	}

	//create identity matrix
	int n = 4;
	identityMatrix = std::vector<std::vector<float> >(n, std::vector<float>(n, 0));

	for(int i = 0; i < n; i++)
		identityMatrix[i][i] = 1;
}

Model::~Model() {
	//keeping this here for future use, nothing to do with destructor
	/* 	for (unsigned int i = 0; i < rotationMatrix.size(); i++ ){
	   for (unsigned int j = 0; j < rotationMatrix[i].size(); j++ ){
	      std::cout << rotationMatrix[i][j] << ' ';
	   }
	   std::cout << std::endl;
	}*/
}

/*Sphere* Model::setSpheres(std::vector<std::vector<float> >& vertices){
	std::vector<float> centroid(3), tmp(3);
	std::vector<float> box;
	box = boundingBox(vertices);
	centroid[0] = (box[0] + box[3])/2;
	centroid[1] = (box[1] + box[4])/2;
	centroid[2] = (box[2] + box[5])/2;
	tmp[0] = box[0]; tmp[1] = box[1]; tmp[2] = box[2];
	//std::cout <<  "Box: " << box[0] << " " << box[1] << " " << box[2] << " " << box[3] << " " << box[4] << " " << box[5] << std::endl;
	float radius = ::distance(tmp, centroid);
	Sphere* sphere = new Sphere(centroid, radius);
	//std::cout << "Centroid: " << centroid[0] << " " << centroid[1] << " " << centroid[2] << " Radius: " << radius << std::endl;
	if(vertices.size() <= 3){
		return sphere;
	}
	std::vector<std::vector<float> > v1(vertices.begin(), vertices.begin() + vertices.size()/2);
	std::vector<std::vector<float> > v2(vertices.begin() + vertices.size()/2, vertices.end());
	sphere->parent = 1;
	sphere->first = setSpheres(v1);
	sphere->second = setSpheres(v2);
	return sphere;
}*/

//displays info about the model
std::string Model::getInfo(){
	std::ostringstream ss;
	ss << "Number of vertices: " << vertices.size() << '\n';
	ss << "Number of faces: " << faces.size() << '\n';
	ss << "Mean vertex: " << vertexMean(vertices) << '\n';
	std::vector<float> box(6);
	box = boundingBox(vertices);
	ss << "Bounding box:\n";
	ss << "   Max: " << box[0] << ", " << box[1] << ", " << box[2] << '\n';
	ss << "   Min: " << box[3] << ", " << box[4] << ", " << box[5] << '\n';

	return ss.str();
}

std::vector <std::vector<float> > Model::getVertices(){
	return vertices;
}

std::vector <std::vector<float> > Model::getFaces(){
	return faces;
}

std::vector<Sphere> Model::getSpheres(){
	return spheres;
}

void Model::setTmpVertices(bool choice){

	if(choice == true){
		//adds 1 to each vertex for multiplication purposes
		for(unsigned int i = 0; i < tmpVertices.size(); i++)
			tmpVertices[i].push_back(1);
	}
	else{
		//removes 1 from end of each vertex
		for(unsigned int i = 0; i < tmpVertices.size(); i++)
			tmpVertices[i].pop_back();
	}
}

void Model::scale(float x, float y, float z){

	std::cout << "Scaling model by: {" << x << ", " << y << ", " << z << "}\n";
	std::vector <std::vector<float> > scaleMatrix = identityMatrix;
	scaleMatrix[0][0] = x;
	scaleMatrix[1][1] = y;
	scaleMatrix[2][2] = z;

	setTmpVertices(true);
	for(unsigned int i = 0; i < tmpVertices.size(); i++){
		tmpVertices[i] = vectorMatrixMultiply(scaleMatrix, tmpVertices[i]);
	}
	setTmpVertices(false);
	vertices = tmpVertices;
	std::cout << "Done scaling.\n";
}

void Model::translate(float x, float y, float z){

	std::cout << "Translating model by: {" << x << ", " << y << ", " << z << "}\n";
	std::vector <std::vector<float> > translateMatrix = identityMatrix;
	translateMatrix[0][3] = x;
	translateMatrix[1][3] = y;
	translateMatrix[2][3] = z;

	setTmpVertices(true);
	for(unsigned int i = 0; i < tmpVertices.size(); i++){
		tmpVertices[i] = vectorMatrixMultiply(translateMatrix, tmpVertices[i]);
	}
	setTmpVertices(false);
	vertices = tmpVertices;
	std::cout << "Done translating.\n";
}

void Model::rotate(float x, float y, float z, float theta){

	std::cout << "Rotating model by: " << theta << " degrees around axis: "
			"{" << x << ", " << y << ", " << z << "}\n";

	float angle = (theta * M_PI)/180;

	float L = (x*x + y*y + z*z);
	float u = x*x, v = y*y, w = z*z;
	std::vector<std::vector<float> > rotationMatrix = identityMatrix;

	//these are the axis angle rotation matrices all multiplied together
	rotationMatrix[0][0] = (u + (v + w) * cos(angle)) / L;
	rotationMatrix[0][1] = (x * y * (1 - cos(angle)) - z * sqrt(L) * sin(angle)) / L;
	rotationMatrix[0][2] = (x * z * (1 - cos(angle)) + y * sqrt(L) * sin(angle)) / L;

	rotationMatrix[1][0] = (x * y * (1 - cos(angle)) + z * sqrt(L) * sin(angle)) / L;
	rotationMatrix[1][1] = (v + (u + w) * cos(angle)) / L;
	rotationMatrix[1][2] = (y * z * (1 - cos(angle)) - x * sqrt(L) * sin(angle)) / L;

	rotationMatrix[2][0] = (x * z * (1 - cos(angle)) - y * sqrt(L) * sin(angle)) / L;
	rotationMatrix[2][1] = (y * z * (1 - cos(angle)) + x * sqrt(L) * sin(angle)) / L;
	rotationMatrix[2][2] = (w + (u + v) * cos(angle)) / L;

	setTmpVertices(true);
	tmpVertices = transpose(matrixMultiply(rotationMatrix, transpose(tmpVertices)));
	setTmpVertices(false);

	vertices = tmpVertices;

	std::cout << "Done rotating.\n";
}
