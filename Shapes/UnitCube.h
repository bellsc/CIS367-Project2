#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#pragma once
using namespace std;

class UnitCube {

private:
    const float SIDE_LENGTH = 1.0f;
    short C_ADJUST = 10;

protected:
    GLuint vertex_buffer, color_buffer, index_buffer;
    std::vector <glm::vec3> all_points, all_colors;
    vector<GLushort> all_index;
public:
    ~UnitCube();
    void build(glm::vec3 color, short variation);
    void render(bool) const;

};