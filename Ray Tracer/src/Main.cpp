/*
 * Main.cpp
 *
 *  Created on: Oct 3, 2015
 *      Author: Saylor Berman
 */

#include "../inc/Model.h"
#include "../inc/Camera.h"
#include "../inc/Image.h"
#include "../inc/Light.h"
#include "../inc/Material.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>

std::vector<std::vector<float> > vertices, faces;
std::vector<Light> lights;
std::vector<Material> materials;
int vertexCount, faceCount;

//reads PLY file, reads vertex/face count, populates vectors
void readPLYFile(std::string& filename){

	std::ifstream file(filename.c_str());
	if(!file.is_open()){
		std::cerr << "Failed to open file: " << filename << '\n';
		exit(-1);
	}

	std::string tmp;
	file >> tmp;
	if(tmp != "ply"){
		std::cerr << "Not a properly formatted PLY file.\n";
		exit(-1);
	}

	//clear vertices, faces
	vertices.clear();
	faces.clear();

	bool headerBool = true;
	std::string line, delimiter;
	int vcount = 0, fcount = 0;
	float x, y, z;
	while(std::getline(file, line)){
		//read and parse header
		if(headerBool == true){
			if(line.find("element vertex") != std::string::npos){
				delimiter = "element vertex";
				std::istringstream vertexFind(line.substr(line.find(delimiter) + delimiter.length(), line.length()));
				vertexFind >> vertexCount;
				vertices.resize(vertexCount);
			}

			if(line.find("element face") != std::string::npos){
				delimiter = "element face";
				std::istringstream faceFind(line.substr(line.find(delimiter) + delimiter.length(), line.length()));
				faceFind >> faceCount;
				faces.resize(faceCount);
			}

			if(line.find("end_header") != std::string::npos){
				headerBool = false;
			}
		}
		//populate vertices vector
		else if(vcount < vertexCount){
			std::istringstream stream(line);

			stream >> x >> y >> z;
			vertices[vcount].push_back(x);
			vertices[vcount].push_back(y);
			vertices[vcount].push_back(z);

			vcount++;
		}

		//populate faces vector
		else{
			int i, numSides;
			std::istringstream stream(line);
			stream >> numSides;
			while(stream >> i)
				faces[fcount].push_back(i);
			fcount++;
		}
	}
	file.close();
}

//reads material property file
void readPropertyFile(std::string& filename, std::vector<Model>& models){

	std::ifstream file(filename.c_str());
	if(!file.is_open()){
		std::cerr << "Failed to open file: " << filename << '\n';
		exit(-1);
	}

	float r, g, b, x, y, z, d1, d2, d3, k, a, o;
	int obj, fFace, lFace;
	std::string line;
	char tmp;
	while(std::getline(file, line)){
		std::istringstream stream(line);
		if(line[0] == 'L'){
			if(line.find('A') != std::string::npos){
				stream >> tmp >> r >> g >> b;
				Light ambient(r, g, b, 0, 0, 0, true);
				lights.push_back(ambient);
			}
			else{
				stream >> tmp >> r >> g >> b >> x >> y >> z;
				Light light(r, g, b, x, y, z, false);
				lights.push_back(light);
			}
		}
		else if(line[0] == 'M'){
			stream >> tmp >> obj >> fFace >> lFace >> d1 >> d2 >> d3 >> k >> a >> o;
			if(obj < 0 || fFace < 0 || lFace < 0){
				std::cerr << "Material file's object and/or faces should be positive integers.\n";
				exit(-1);
			}
			if(d1 < 0 || d1 > 1 || d2 < 0 || d2 > 1 || d3 < 0 || d3 > 1 || k < 0 || k > 1 || a < 0 || o < 0 || o > 1){
				std::cerr << "Material properties must be between 0 and 1 (alpha can be larger than 1).\n";
				exit(-1);
			}
			Material material(obj, fFace, lFace, d1, d2, d3, k, a, o, models);
			materials.push_back(material);
		}
		else{
			std::cerr << "Not a properly formatted property file.\n";
			exit(-1);
		}
	}
	file.close();
}

//writes output image file, ppm format
void writeFile(const Image& image, std::string& filename, int u, int v){
	std::ofstream file;
	file.open(filename.c_str());

	std::vector<std::vector<std::vector<float> > > pixels;
	pixels = image.getPixels();

	file << "P3\n";
	file << u << " " << v << " " << 255 << '\n';
	for(unsigned int i = 0; i < pixels.size(); i++){
		for(unsigned int j = 0; j < pixels[0].size(); j++){
				file << pixels[i][j][0] << " " << pixels[i][j][1] << " " << pixels[i][j][2] << '\n';
		}
	}

	file.close();
	std::cout << "File " << filename << " successfully written.\n";
}

void printUsage(){
	std::cout << "	Usage: ./raytracer <camera> <properties> <ply files...> <image>\n";
	std::cout << "	<camera> is a file containing the camera model\n";
	std::cout << "	<properties> is file containing the material properties of the model\n";
	std::cout << "	<ply files...> are the model files to use\n";
	std::cout << "	<image> is the name of the image to write\n";
	exit(-1);
}

int main(int argc, char *argv[]){

	if(argc < 5)
		printUsage();

	std::string cam = argv[1];
	std::string properties = argv[2];
	std::string output = argv[argc - 1];
	std::string plyFile;
	std::vector<Model> models;

	//read in all PLY files, create vector of models
	for(int i = 3; i < argc - 1; i++){
		plyFile = argv[i];
		readPLYFile(plyFile);
		Model model(vertices, faces);
		models.push_back(model);
	}
	readPropertyFile(properties, models);

	Camera camera(cam, models, lights, materials);

	int u = abs(camera.boundingBox[0]) + abs(camera.boundingBox[2]) + 1;
	int v = abs(camera.boundingBox[1]) + abs(camera.boundingBox[3]) + 1;

	std::cout << "Generating image..." << std::endl;
	clock_t t1 = clock();

	Image image(camera, u, v);
	image.generate();
	writeFile(image, output, u, v);

	clock_t t2 = clock();
	float diff((float)t2 - (float)t1);
	std::cout << "Time: " << diff/CLOCKS_PER_SEC << '\n';
	return 0;
}
