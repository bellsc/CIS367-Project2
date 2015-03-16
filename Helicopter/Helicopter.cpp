#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "Helicopter.h"

using glm::vec3;



void Helicopter::build(int mainBlades, int tailBlades, glm::vec3 color) {

    propBase.build(4,4,color);
    skid.build(4,4,color);
    tailPiece.build(4,4,color);

    mainProp.build(mainBlades, 8, color);
    tailProp.build(tailBlades, 3, color);

    body =  gluNewQuadric();
    tail = gluNewQuadric();
    skidConnect = gluNewQuadric();
    spotlight = gluNewQuadric();

    spotlight_cf = glm::translate(glm::vec3{3.1, 0, -1.2});
    spotlight_cf *= glm::rotate (glm::radians(145.0f), glm::vec3{0,1,0});

}

void Helicopter::render(bool outline) const {
    glLightfv (GL_LIGHT1, GL_POSITION, glm::value_ptr(glm::column(spotlight_cf, 3)));
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, glm::value_ptr(glm::column(spotlight_cf, 2)));

    static float CHROME_AMBIENT[] = {0.250000, 0.250000, 0.250000, 1.0};
    static float CHROME_DIFFUSE[] = {0.400000, 0.400000, 0.400000, 1.0};
    static float CHROME_SPECULAR[] = {0.774597, 0.774597, 0.774597, 1.0};

    static float RUBY_AMBIENT[] = {0.174500, 0.011750, 0.011750, 1.0};
    static float RUBY_DIFFUSE[] = {0.614240, 0.041360, 0.041360, 1.0};
    static float RUBY_SPECULAR[] = {0.727811, 0.626959, 0.626959, 1.0};


    glMaterialfv(GL_FRONT, GL_AMBIENT, RUBY_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, RUBY_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, RUBY_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);

    //Body
    glPushMatrix();
    glScalef(4, 1.4, 1.8);
    gluSphere(body,1, 30, 30);
    glPopMatrix();

    //Tail
    glPushMatrix();
    glScalef(1, .6, .8);
    glTranslatef(-2.4,0,.8);
    glRotatef(-90, 0, 1, 0);
    gluCylinder(tail,.7, .15, 7.5, 30, 30);
    glTranslatef(0,0,7.5);
    glRotatef(180,1, 0, 0);
    gluDisk(gluNewQuadric(),0, .16, 8, 8);
    glPopMatrix();

    //Propeller base
    glPushMatrix();
    glScalef(3.0, 1.3, .5);
    glTranslatef(-.2, 0, 3.2);
    propBase.render(false);
    glPopMatrix();

    //Tail pieces
    glPushMatrix();
    glTranslatef(-9.6, 0, .40);
    glRotatef(40, 0, 1, 0);
    glScalef(.3, .07, 1.0);
    glRotatef(-7, 0, 1, 0);
    tailPiece.render(false);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-9.8, 0, 1.10);
    glRotatef(-40, 0, 1, 0);
    glScalef(.3, .07, 1.4);
    glRotatef(8, 0, 1, 0);
    tailPiece.render(false);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, CHROME_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, CHROME_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, CHROME_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 10);

    //main prop
    glPushMatrix();
    glTranslatef(0, 0, 2.1);
    glMultMatrixf(glm::value_ptr(mainProp_cf));
    mainProp.render(false);
    glPopMatrix();


    //Tail prop
    glPushMatrix();
    glTranslatef(-9.5, -.17, .6);
    glScalef(.25, .6, .25);
    glRotatef(90, 1,0,0);
    glMultMatrixf(glm::value_ptr(tailProp_cf));
    tailProp.render(false);
    glPopMatrix();

    //Skids
    gluQuadricOrientation( skidConnect, GLU_INSIDE);
    glPushMatrix();
    glTranslatef(-1.5,-1.2,-2.0);
    glRotatef(-35, 1, 0, 0);
    gluCylinder(skidConnect,.1, .1, 1.2, 30, 30);
    glTranslatef(3,0,0);
    gluCylinder(skidConnect,.1, .1, 1.2, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.4,-1.2,-2.0);
    glScalef(5.2,.25,.1);
    skid.render(false);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-1.5,1.2,-2.0);
    glRotatef(35, 1, 0, 0);
    gluCylinder(skidConnect,.1, .1, 1.2, 30, 30);
    glTranslatef(3,0,0);
    gluCylinder(skidConnect,.1, .1, 1.2, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.4,1.2,-2.0);
    glScalef(5.2,.25,.1);
    skid.render(false);
    glPopMatrix();

    //Spotlight
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(spotlight_cf));
    gluSphere(spotlight, .148, 10, 10);
    gluCylinder(spotlight,.15, .3, .3, 16, 16);
    gluQuadricOrientation(spotlight, GLU_INSIDE);
    gluCylinder(spotlight,.149, .299, .3, 16, 16);
    gluQuadricOrientation(spotlight, GLU_OUTSIDE);

    glPopMatrix();
}

