#ifndef HANS_WHEEL_H
#define HANS_WHEEL_H

#include <GL/glew.h>

/* Wheel : torus  + spokes */
class WheelTemp {
public:
    /* default size is unit box */
    WheelTemp();
    void build();
    void render() const;
    GLfloat getRadius() const { return radius; }
    GLfloat getThickness() const { return tire_thickness; }
private:
    GLfloat radius;
    GLfloat tire_thickness;
    GLUquadric *spoke;
    GLint list_id;
};
#endif
