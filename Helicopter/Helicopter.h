#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#include "../Shapes/UnitCube.h"
#include "Propeller.h"

#pragma once
using namespace std;

class Helicopter{

private:

    UnitCube propBase, skid;
    UnitCube tailPiece ;
    Propeller mainProp, tailProp;
    GLUquadric *body, *tail, *skidConnect, *spotlight;

public:
    glm::mat4 spotlight_cf;
    glm::mat4 mainProp_cf, tailProp_cf;
    void build(int mainBlades, int tailBlades, glm::vec3 color);
    void render(bool) const;
};