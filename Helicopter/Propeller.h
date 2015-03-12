#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#include "../Shapes/UnitCube.h"

#pragma once
using namespace std;

class Propeller{

private:
    const float HEIGHT = 1.0f;
    const float BLADE_LENGTH = 3.0f;
    const float ROTOR_RAD = 0.5f;
    float blades, bladeLen;

    UnitCube blade;
    GLUquadric *rotor;


public:
    ~Propeller();
    void build(int numBlades, float bladeLength, glm::vec3 color);
    void render(bool) const;

};