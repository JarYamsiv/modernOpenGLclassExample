CC = g++
GDB = -g
OBJS = objects/main.o objects/triangle.o objects/config.o
LIBS = -lGL -lGLU -lglut -lglfw -lGLEW

a.out : ${OBJS}
	${CC} ${GDB} ${OBJS} GLAD/glad.o ${LIBS} -o bin/main.out 

objects/main.o: main.cpp
	${CC} -c main.cpp -o objects/main.o

objects/triangle.o: src/triangle.cpp headers/triangle.h
	${CC} -c src/triangle.cpp -o objects/triangle.o

objects/config.o: src/config.cpp headers/config.h
	${CC} -c src/config.cpp -o objects/config.o


clean:
	\rm a.out *.o *.dot
