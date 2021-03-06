#define _USE_MATH_DEFINES
#include <cmath>
#include <time.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "UnitCube.h"

using glm::vec3;
UnitCube::~UnitCube() {
    if (glIsBuffer(vertex_buffer))
        glDeleteBuffers(1, &vertex_buffer);
    if (glIsBuffer(color_buffer))
        glDeleteBuffers(1, &color_buffer);
    if (glIsBuffer(index_buffer))
        glDeleteBuffers(1, &index_buffer);
    if (glIsBuffer(normal_buffer))
        glDeleteBuffers(1, &normal_buffer);
}

void UnitCube::build(int stacks, int slices, glm::vec3 color) {

    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &index_buffer);
    glGenBuffers(1, &color_buffer);
    glGenBuffers(1, &normal_buffer);

    //Make function to create 1 face.  Put 6 faces together.
    stcks = stacks;
    slces = slices;

    float x, y, z;
    x = y = z = SIDE_LENGTH / 2;

    y = -y;
    for(float i = 0; i < stcks+1;i++){
        for(float j = 0; j < slces+1; j++){

            vec3 vertex{x, y, z};
            all_points.push_back(vertex);
            all_colors.push_back(color);
            y += 1/(slces);
            all_normals.push_back(vec3{1, 0, 0});
        }
        y = -SIDE_LENGTH/2;
        z -=  1/(stcks) ;
    }

    //Indices
    for(int i = 0; i < stcks;i++){
        for(int j = 0; j < slces+1; j++){
            int temp1 = j+i*(slces+1);
            int temp2 = j+(i+1)*(slces+1);
            all_index.push_back(temp1);
            all_index.push_back(temp2);

        }
    }

    //Normals
    for(int i = 1; i < slces; i++){
        all_normals[i] = glm::normalize(vec3{1, 0, 1});
        all_normals[i+(stcks*(slces+1))]= glm::normalize(vec3{1, 0, -1});
    }
    for(int i = 1; i < stcks; i++){
        all_normals[i*(slces+1)] = glm::normalize(vec3{1, -1, 0});
        all_normals[i*(slces+1)+slces] = glm::normalize(vec3{1, 1, 0});
    }

    //Corners
    all_normals[0] = glm::normalize(vec3{1, -1, 1});
    all_normals[slces] = glm::normalize(vec3{1, 1, 1});
    all_normals[stcks*(slces+1)] = glm::normalize(vec3{1, -1, -1});
    all_normals[(stcks+1)*(slces+1)-1] = glm::normalize(vec3{1, 1, -1});


    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, all_points.size() * sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);
    float *vertex_ptr = (float *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    /* Initialize the vertices */
    float *ptr = vertex_ptr;
    for (auto v : all_points) {
        ptr[0] = v.x;
        ptr[1] = v.y;
        ptr[2] = v.z;
        ptr += 3;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //colors
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, all_colors.size() * sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);
    float *color_ptr = (float *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    /* Initialize the vertices */
    float *cptr = color_ptr;
    for (auto v : all_colors) {
        cptr[0] = v.x;
        cptr[1] = v.y;
        cptr[2] = v.z;
        cptr += 3;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Initialize the indices */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, all_index.size() * sizeof(GLushort), all_index.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //normals
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, all_normals.size() * sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);
    float *normal_ptr = (float *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    /* Initialize the vertices */
    float *nptr = normal_ptr;
    for (auto v : all_normals) {
        nptr[0] = v.x;
        nptr[1] = v.y;
        nptr[2] = v.z;
        nptr += 3;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    face_cf *= glm::rotate (glm::radians(90.0f), glm::vec3{0,0,1});
    bot_cf *= glm::rotate (glm::radians(90.0f), glm::vec3{0,1,0});
    top_cf *= glm::rotate (glm::radians(-180.0f), glm::vec3{0,1,0});
}

void UnitCube::render(bool outline) const {

    glPushMatrix();

    renderFace(outline);
    glMultMatrixf(glm::value_ptr(face_cf));
    renderFace(outline);
    glMultMatrixf(glm::value_ptr(face_cf));
    renderFace(outline);
    glMultMatrixf(glm::value_ptr(face_cf));
    renderFace(outline);

    glMultMatrixf(glm::value_ptr(bot_cf));
    renderFace(outline);

    glMultMatrixf(glm::value_ptr(top_cf));
    renderFace(outline);

    glPopMatrix();

}

void UnitCube::renderFace(bool outline) const{
    /* bind vertex buffer */
    glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    //glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);

    glColorPointer(3, GL_FLOAT, 0, 0);

    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

    /* render the polygon */
    if(outline)
        glPolygonMode(GL_FRONT, GL_LINE);
    else
        glPolygonMode (GL_FRONT, GL_FILL);
    glPolygonMode (GL_BACK, GL_LINE);


    int n = 0;
    for(int i = 0; i < stcks; i++) {
        glDrawElements(GL_QUAD_STRIP, 2 * (slces+1), GL_UNSIGNED_SHORT,(void *) (sizeof(GLushort) * (n)));
        n += 2 * (slces+1);
}

    /* unbind the buffers */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glPopAttrib();
}