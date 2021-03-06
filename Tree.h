#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#include "Shapes/UnitCube.h"

#pragma once
using namespace std;

class Tree{

private:
    float height, trunkRad, leafRows;
    GLUquadric *trunkBase, *trunk, *leafBunch;

public:
    void build(float trunkHeight, int numLeafRows);
    void render(bool) const;

};