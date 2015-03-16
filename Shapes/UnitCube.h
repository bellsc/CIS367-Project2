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
    float stcks = 1;
    float slces = 1;
    glm::mat4 face_cf, top_cf, bot_cf;
    void renderFace(bool) const;

protected:
    GLuint vertex_buffer, color_buffer, index_buffer, normal_buffer;
    std::vector <glm::vec3> all_points, all_colors, all_normals;
    vector<GLushort> all_index;
public:
    ~UnitCube();
    void build(int stacks, int slices, glm::vec3 color);
    void render(bool) const;


};