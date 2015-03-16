#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Tree.h"

using glm::vec3;

void Tree::build(float trunkHeight, int numLeafRows) {

    height = trunkHeight;
    trunkRad = .07 * height;
    leafRows = numLeafRows;

    trunkBase = gluNewQuadric();
    trunk = gluNewQuadric();
    leafBunch = gluNewQuadric();

}

void Tree::render(bool outline) const {

    static float TREE_AMBIENT[] = {0.13, 0.22, 0.15, 1.0};
    static float TREE_DIFFUSE[] = {0.54, 0.89, 0.63, 1.0};
    static float TREE_SPECULAR[] = {0.32, 0.32, 0.32, 1.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT, TREE_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, TREE_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, TREE_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 120);

    glEnable(GL_COLOR_MATERIAL);
    glColor3ub (70, 35, 0);

    //TrunkBase
    glPushMatrix();
    gluCylinder(trunkBase, 1.4*trunkRad, trunkRad, .4*height, 10, 10);
    glPopMatrix();

    //Trunk
    glPushMatrix();
    glTranslatef(0,0,.2*height);
    gluCylinder(trunk, trunkRad, trunkRad, .6*height, 10, 10);
    glPopMatrix();


    //Leaf bunches
    //Top
    glColor3ub (0, 85, 0);
    glPushMatrix();
    glTranslatef(0, 0, height + trunkRad);
    gluSphere(leafBunch, trunkRad*2.5, 12, 12);
    glPopMatrix();

    //Lower layers
    glPushMatrix();
    glTranslatef(0, 0, height);
    float angle = 0;
    for(float i = 1; i < leafRows; i++) {
        int n = (i+1)*(1+1);
        for(int j = 0; j < n; j++){
            glPushMatrix();
            glRotatef(j*360/n, 0, 0, 1);
            glTranslatef(i * trunkRad * 1.5, 0, 0);
            gluSphere(leafBunch, trunkRad*2.5, 12, 12);
            glPopMatrix();
        }
        glTranslatef(0, 0, -height/(2*leafRows));
    }
    glPopMatrix();

    glDisable(GL_COLOR_MATERIAL);
}