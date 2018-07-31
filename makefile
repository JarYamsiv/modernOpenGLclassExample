CC = g++
GDB = -g
CXXFLAGS += -std=c++11
OBJS = objects/main.o objects/triangle.o objects/config.o objects/shaders.o objects/mesh.o
LIBS = -lGL -lGLU -lglut -lglfw -lGLEW

a.out : ${OBJS}
	${CC} ${GDB} ${CXXFLAGS} ${OBJS} GLAD/glad.o ${LIBS} -o bin/main.out 

objects/main.o: main.cpp
	${CC} -c main.cpp -o objects/main.o

objects/triangle.o: src/triangle.cpp headers/triangle.h
	${CC} ${CXXFLAGS} -c src/triangle.cpp -o objects/triangle.o

objects/config.o: src/config.cpp headers/config.h
	${CC} -c src/config.cpp -o objects/config.o

objects/shaders.o: src/shaders.cpp headers/shaders.h
	${CC} -c src/shaders.cpp -o objects/shaders.o

objects/mesh.o: src/mesh.cpp headers/mesh.h
	${CC} -c src/mesh.cpp -o objects/mesh.o


clean:
	\rm a.out *.o *.dot
