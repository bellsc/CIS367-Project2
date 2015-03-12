#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Helicopter.h"

using glm::vec3;

Helicopter::~Helicopter() {

}

void Helicopter::build(int mainBlades, int tailBlades, glm::vec3 color) {

//    propBase.build(color, 1);
//    skid.build(color, 1);
//    skidConnect.build(color, 1);

    propBase.build(4,4,color, 1);
    skid.build(4,4,color, 1);

    tailPiece.build(4,4,color,1);


    mainProp.build(mainBlades, 8, color);
    tailProp.build(tailBlades, 3, color);

    body =  gluNewQuadric();
    tail = gluNewQuadric();
    skidConnect = gluNewQuadric();

   // mainProp_cf *= glm::rotate(glm::radians(90.0f), glm::vec3{1,0,0});
}

void Helicopter::render(bool outline) const {
    static float RUBBER_AMBIENT[] = {0.02, 0.02, 0.02, 1.0};
    static float RUBBER_DIFFUSE[] = {0.01, 0.01, 0.01, 1.0};
    static float RUBBER_SPECULAR[] = {0.4, 0.4, 0.4, 1.0};

    static float CHROME_AMBIENT[] = {0.250000, 0.250000, 0.250000, 1.0};
    static float CHROME_DIFFUSE[] = {0.400000, 0.400000, 0.400000, 1.0};
    static float CHROME_SPECULAR[] = {0.774597, 0.774597, 0.774597, 1.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT, CHROME_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, CHROME_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, CHROME_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 10);

   // glEnable(GL_COLOR_MATERIAL);

    gluQuadricOrientation( body, GLU_INSIDE);
    glPushMatrix();
    glScalef(4, 1.4, 1.8);
    gluSphere(body,1, 30, 30);
    glPopMatrix();

    gluQuadricOrientation( tail, GLU_INSIDE);
    glPushMatrix();
    glScalef(1, .6, .8);
    glTranslatef(-2.4,0,.8);
    glRotatef(-90, 0, 1, 0);
    gluCylinder(tail,.7, .15, 7.5, 30, 30);
    glTranslatef(0,0,7.5);
    glRotatef(180,1, 0, 0);
   // glScalef(5, 5, 5);
    gluDisk(gluNewQuadric(),0, .16, 8, 8);
    glPopMatrix();



    glPushMatrix();
    glScalef(3.0, 1.3, .5);
    glTranslatef(-.2, 0, 3.2);
    propBase.render(false);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 2.1);
    mainProp.render(false);
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

    //tail prop
    glPushMatrix();

    glTranslatef(-9.5, -.17, .6);
    glScalef(.25, .6, .25);
    glRotatef(90, 1,0,0);
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



    /*
     propBase.build(4,4,color, 1);
    skid.build(4,4,color, 1);
    skidConnect.build(4,4,color, 1);

    mainProp.build(numBlades, color);
    tailProp.build(numBlades, color);
     */




//    //Blades
//    for(int i = 0; i < blades; i++){
//        glPushMatrix();
//        glRotatef(i * 360/blades, 0, 0, 1);
//        glTranslatef(4, 0, 0);
//        glScalef(9, .6, .1);
//        blade.render(outline);
//        glPopMatrix();
//    }
//
//
//    gluQuadricOrientation( rotor, GLU_INSIDE);
//    //Center rotor
//    glPushMatrix();
//    glTranslatef(0,0,-.4);
//    gluCylinder(rotor, .5, .5, .7, 20, 5);
//
//    glTranslatef(0, 0, .7);
//    glRotatef(180,1,0,0);
//    gluDisk(gluNewQuadric(),0, .5, 20, 5);
//    glPopMatrix();

}