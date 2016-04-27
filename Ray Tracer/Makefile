CXXFLAGS=-Wall -Wextra -ansi -pedantic -Wfatal-errors -g -fopenmp

raytracer: Main.o Model.o Image.o Camera.o Sphere.o Material.o Light.o LinearAlgebra.o
	g++ $(CXXFLAGS) Main.o Model.o Image.o Camera.o Sphere.o Material.o Light.o LinearAlgebra.o -o raytracer

Main.o: src/Main.cpp
	g++ $(CXXFLAGS) -c src/Main.cpp

Model.o: src/Model.cpp inc/Model.h
	g++ $(CXXFLAGS) -c src/Model.cpp

Image.o: src/Image.cpp inc/Image.h
	g++ $(CXXFLAGS) -c src/Image.cpp

Camera.o: src/Camera.cpp inc/Camera.h
	g++ $(CXXFLAGS) -c src/Camera.cpp

Sphere.o: src/Sphere.cpp inc/Sphere.h
	g++ $(CXXFLAGS) -c src/Sphere.cpp

Material.o: src/Material.cpp inc/Material.h
	g++ $(CXXFLAGS) -c src/Material.cpp

Light.o: src/Light.cpp inc/Light.h
	g++ $(CXXFLAGS) -c src/Light.cpp

LinearAlgebra.o: utils/LinearAlgebra.cpp utils/LinearAlgebra.h
	g++ $(CXXFLAGS) -c utils/LinearAlgebra.cpp

clean:
	rm -f raytracer *.o