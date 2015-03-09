#define _USE_MATH_DEFINES
#include <cmath>
#include <time.h>
#include "Ring.h"

using glm::vec3;

Ring::~Ring() {
    if (glIsBuffer(vertex_buffer))
        glDeleteBuffers(1, &vertex_buffer);
    if (glIsBuffer(color_buffer))
        glDeleteBuffers(1, &color_buffer);
    if (glIsBuffer(index_buffer))
        glDeleteBuffers(1, &index_buffer);
}

void Ring::build(float innerRad, short circ_pts, glm::vec3 color, short variation) {

    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &index_buffer);
    glGenBuffers(1, &color_buffer);

    circ_points = circ_pts;

    srand (time(NULL));

    float angle = 0;
    float z = HEIGHT/2;

    //Upper, inner circle
    for (int i=0; i < circ_points; i++){
        vec3 v1{innerRad * cos(angle), innerRad * sin(angle), z};
        all_points.push_back(v1);
        vec3 darkColor{color.x-(rand()%variation+ C_ADJUST)/100.0, color.y-(rand()%variation+ C_ADJUST)/100.0, color.z-(rand()%variation+ C_ADJUST)/100.0};
        all_colors.push_back(darkColor);
        angle += 2 * M_PI/circ_points;
    }

    //Upper, outer circle
    for (int i=0; i < circ_points; i++){
        vec3 v1{OUTER_RAD * cos(angle), OUTER_RAD * sin(angle), z};
        all_points.push_back(v1);
        vec3 lightColor{color.x+(rand()%variation+ C_ADJUST)/100.0, color.y+(rand()%variation+ C_ADJUST)/100.0, color.z+(rand()%variation+ C_ADJUST)/100.0};
        all_colors.push_back(lightColor);
        angle += 2 * M_PI/circ_points;
    }

    z = -HEIGHT/2;
    //Lower, inner circle
    for (int i=0; i < circ_points; i++){
        vec3 v1{innerRad * cos(angle), innerRad * sin(angle), z};
        all_points.push_back(v1);
        vec3 darkColor{color.x-(rand()%variation+ C_ADJUST)/100.0, color.y-(rand()%variation+ C_ADJUST)/100.0, color.z-(rand()%variation+ C_ADJUST)/100.0};
        all_colors.push_back(darkColor);
        angle += 2 * M_PI/circ_points;
    }

    //Lower, outer circle
    for (int i=0; i < circ_points; i++){
        vec3 v1{OUTER_RAD * cos(angle), OUTER_RAD * sin(angle), z};
        all_points.push_back(v1);
        vec3 normColor{color.x-variation/200.0+(rand()%variation)/100.0,
                color.y-variation/200.0-(rand()%variation)/100.0,
                color.z-variation/200.0-(rand()%variation)/100.0};
        all_colors.push_back(normColor);
        angle += 2 * M_PI/circ_points;
    }


    //Top indices
    for(int i = 0; i < circ_points; i++){
        all_index.push_back(i);
        all_index.push_back(i+circ_points);
    }
    all_index.push_back(0);
    all_index.push_back(circ_points);


    //Bottom indices
    for(int i = 2*circ_points; i < 3 * circ_points; i++){
        all_index.push_back(i);
        all_index.push_back(i+circ_points);
    }
    all_index.push_back(2*circ_points);
    all_index.push_back(3*circ_points);

    //Inner Quads
    for(int i = 0; i < circ_points; i++){
        all_index.push_back(i);
        all_index.push_back(i+2*circ_points);
    }
    all_index.push_back(0);
    all_index.push_back(2*circ_points);

    //Outer Quads
    for(int i = circ_points; i < 2 * circ_points; i++){
        all_index.push_back(i);
        all_index.push_back(i+2*circ_points);
    }
    all_index.push_back(circ_points);
    all_index.push_back(3*circ_points);


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
}

void Ring::render(bool outline) const {
    /* bind vertex buffer */
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glEnableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

    /* render the polygon */
    if(outline)
        glPolygonMode(GL_FRONT, GL_LINE);
    else
        glPolygonMode (GL_FRONT, GL_FILL);

    glPolygonMode (GL_BACK, GL_LINE);

    int n =  circ_points * 2 + 2;
    glFrontFace(GL_CCW);
    glDrawElements(GL_QUAD_STRIP, n, GL_UNSIGNED_SHORT, 0);

    glFrontFace(GL_CW);
    glDrawElements(GL_QUAD_STRIP, n, GL_UNSIGNED_SHORT,
            (void *) ((sizeof(GLushort) * (n))));

    glDrawElements(GL_QUAD_STRIP, n, GL_UNSIGNED_SHORT,
            (void *) ((sizeof(GLushort) * (2*n))));

    glFrontFace(GL_CCW);
    glDrawElements(GL_QUAD_STRIP, n, GL_UNSIGNED_SHORT,
            (void *) ((sizeof(GLushort) * (3*n))));

    /* unbind the buffers */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnableClientState(GL_COLOR_ARRAY);
}