#include "E512W3D.h"
Mesh cube;

void cubeInit () {
    cube.addVertex( 1.0f, -1.0f, -1.0f);
    cube.addVertex( 1.0f, -1.0f,  1.0f);
    cube.addVertex(-1.0f, -1.0f,  1.0f);
    cube.addVertex(-1.0f, -1.0f, -1.0f);
    cube.addVertex( 1.0f,  1.0f, -1.0f);
    cube.addVertex( 1.0f,  1.0f,  1.0f);
    cube.addVertex(-1.0f,  1.0f,  1.0f);
    cube.addVertex(-1.0f,  1.0f, -1.0f);
    cube.addFace(1, 3, 0);
    cube.addFace(7, 5, 4);
    cube.addFace(4, 1, 0);
    cube.addFace(5, 2, 1);
    cube.addFace(2, 7, 3);
    cube.addFace(0, 7, 4);
    cube.addFace(1, 2, 3);
    cube.addFace(7, 6, 5);
    cube.addFace(4, 5, 1);
    cube.addFace(5, 6, 2);
    cube.addFace(2, 6, 7);
    cube.addFace(0, 3, 7);
}
