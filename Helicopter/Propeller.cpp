#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Propeller.h"

using glm::vec3;

void Propeller::build(int numBlades, float bladeLength, glm::vec3 color) {
    blades = numBlades;
    bladeLen = bladeLength;
    blade.build(4,4,color);
    rotor =  gluNewQuadric();
}

void Propeller::render(bool outline) const {
    static float CHROME_AMBIENT[] = {0.250000, 0.250000, 0.250000, 1.0};
    static float CHROME_DIFFUSE[] = {0.400000, 0.400000, 0.400000, 1.0};
    static float CHROME_SPECULAR[] = {0.774597, 0.774597, 0.774597, 1.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT, CHROME_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, CHROME_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, CHROME_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 76.800003);

    //Blades
    for(int i = 0; i < blades; i++){
        glPushMatrix();
        glRotatef(i * 360/blades, 0, 0, 1);
        glTranslatef(.45*bladeLen, 0, 0);
        glScalef(bladeLen, .4, .05);
        blade.render(outline);
        glPopMatrix();
    }

    //Center rotor
    glPushMatrix();
    glTranslatef(0,0,-.3);
    gluCylinder(rotor, .2, .2, .4, 20, 5);

    glTranslatef(0, 0, .4);
    glRotatef(180,1,0,0);
    gluDisk(gluNewQuadric(),0, .2, 20, 5);
    glPopMatrix();

}