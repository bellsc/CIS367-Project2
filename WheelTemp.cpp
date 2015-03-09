#include "WheelTemp.h"

WheelTemp::WheelTemp()
{
    radius = 3.5;
    tire_thickness = 0.4;
}

void WheelTemp::build ()
{
    const int NUM_SPOKES = 6;
    static float CHROME_AMBIENT[] = {0.25, 0.25, 0.25, 1};
    static float CHROME_DIFFUSE[] = {0.4, 0.4, 0.4, 1.0};
    static float CHROME_SPECULAR[] = {0.774597, 0.774597, 0.774597, 1.000000};
    /* black rubber */
    spoke = gluNewQuadric();

    list_id = glGenLists(1);
    glNewList (list_id, GL_COMPILE);
    glPushMatrix();
    glRotatef (90, 1, 0, 0);

//    glColor3ub (70,40, 40);
    glMaterialfv(GL_FRONT, GL_AMBIENT, CHROME_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, CHROME_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, CHROME_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 76.800003);
    /* make the spokes */
    for (int k = 0; k < NUM_SPOKES; k++)
    {
        float angle = k * 360.0 / NUM_SPOKES;
        glPushMatrix();
        glRotatef (angle, 0, 1, 0);
        gluCylinder(spoke, 0.75 * tire_thickness, 0.75 * tire_thickness, radius, 10, 3);
        glPopMatrix();
    }
    /* make the hub */
    glPushMatrix();
    glTranslatef (0, 0, -tire_thickness/2);
    glScalef (0.3 * radius, 0.3 * radius, tire_thickness);
//    spoke->draw();
    glPopMatrix();
    glPopMatrix();
    glEndList();
};

void WheelTemp::render() const {
    static float RUBBER_AMBIENT[] = {0.02, 0.02, 0.02, 1.0};
    static float RUBBER_DIFFUSE[] = {0.01, 0.01, 0.01, 1.0};
    static float RUBBER_SPECULAR[] = {0.4, 0.4, 0.4, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, RUBBER_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, RUBBER_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, RUBBER_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 10);
    glCallList(list_id);
};
