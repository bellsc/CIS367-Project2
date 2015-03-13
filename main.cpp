/**
* Example of 3D object animation
*
* Hans Dulimarta
* dulimar@cis.gvsu.edu
*/
#define GLEW_STATIC
#include <GL/glew.h>
#include <sys/time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/io.hpp>


#include "WheelTemp.h"
#include "Shapes/UnitCube.h"
#include "Helicopter/Propeller.h"
#include "Helicopter/Helicopter.h"



using namespace std;
void displayCallback(GLFWwindow*);

/* define global variables here */

UnitCube ground;
Helicopter heli;
GLUquadric *sun;


glm::mat4 heli_cf;
glm::mat4 camera_cf, light1_cf, light0_cf;
glm::mat4 *active;

float arc_ball_rad_square;
int screen_ctr_x, screen_ctr_y;

int propSpeedMode = 0;
const float PROP_SPEEDS[] = {0, 600, 1000, 1400};
float currentHeight = 0;


const float INIT_SWING_ANGLE = 35.0f; /* degrees */
const float GRAVITY = 9.8;   /* m/sec^2 */
bool is_anim_running = true;

/* light source setting */
GLfloat light0_color[] = {1.0, 1.0, 1.0, 1.0};   /* color */
GLfloat light1_color[] = {1.0, 1.0, 0.6, 1.0};  /* color */
GLfloat black_color[] = {0.0, 0.0, 0.0, 1.0};   /* color */

/*--------------------------------*
 * GLUT Reshape callback function *
 *--------------------------------*/
void reshapeCallback (GLFWwindow *win, int w, int h)
{
    glViewport (0, 0, w, h);

    /* switch to Projection matrix mode */
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    gluPerspective (60, (float) w / (float) h, 5.0, 100.0);

    /* switch back to Model View matrix mode */
    glMatrixMode (GL_MODELVIEW);
    camera_cf = glm::lookAt(glm::vec3(25,20,20), glm::vec3(0,0,10), glm::vec3(0,0,1));
}

/*================================================================*
 * Idle Callback function. This is the main engine for simulation *
 *================================================================*/
void updateCoordFrames()
{
    static double last_timestamp = 0;
    //static float swing_time = 0;

    static float prop_angle = 0;
    //static int deg = 0;


    float delta, current;

    current = glfwGetTime();
    if (is_anim_running) {
        delta = (current - last_timestamp);
        prop_angle = PROP_SPEEDS[propSpeedMode] * delta;


        heli_cf = glm::translate(glm::vec3{-.5 * heli.currXangle * delta, -.5 * heli.currYangle * delta, 0}) * heli_cf;

        //For helicopter movement, add together the z axis vector of the helicopter model
        //with the gravity vector (down in the world of course.  Natural movement without
        //"height modes" or differentiating x and y translation.

        //Move up if propeller speed can push up.
        if (currentHeight <= PROP_SPEEDS[propSpeedMode] / 25) {
            float liftSpeed = PROP_SPEEDS[propSpeedMode] - currentHeight;
            currentHeight += liftSpeed/100 * delta;
            heli_cf = glm::translate(glm::vec3{0, 0, liftSpeed/100 * delta}) * heli_cf;
        }

        //Move down if propeller speed can't preserve the current height.
        else if (currentHeight > PROP_SPEEDS[propSpeedMode] / 25) {
            float liftSpeed =  currentHeight - PROP_SPEEDS[propSpeedMode];
            currentHeight += liftSpeed/100 * delta;
            heli_cf = glm::translate(glm::vec3{0, 0, liftSpeed/100 * delta}) * heli_cf;
            /*
            if(propSpeedMode == 0 && liftSpeed/100 * delta > currentHeight){
                heli_cf = glm::translate(glm::vec3{0, 0, -currentHeight}) * heli_cf;
                currentHeight = 0;
            }
            */
        }



        //Helicopter propellers
        heli.mainProp_cf *= glm::rotate(glm::radians(prop_angle), glm::vec3{0.0f, 0.0f, 1.0f});
        heli.tailProp_cf *= glm::rotate(glm::radians(prop_angle), glm::vec3{0.0f, 0.0f, 1.0f});


        /* use the pendulum equation to calculate its angle */
       // swing_time += delta * 3;
        /*
        float angle = INIT_SWING_ANGLE *
                cos(swing_time * sqrt(GRAVITY / swingarm->length()));
        swing_arm_cf *= glm::rotate(glm::radians(angle - swing_angle), glm::vec3{0.0f, 1.0f, 0.0f});
        swing_angle = angle;
        */
    }
    last_timestamp = current;
}

void myGLInit ()
{
    glClearColor (0.0, 0.0, 0.0, 1.0); /* black background */

    /* fill front-facing polygon */
    glPolygonMode (GL_FRONT, GL_FILL);
    /* draw outline of back-facing polygon */
    glPolygonMode (GL_BACK, GL_LINE);

    /* Enable depth test for hidden surface removal */
    glEnable (GL_DEPTH_TEST);

    /* enable back-face culling */
    glEnable (GL_CULL_FACE);
    //glCullFace (GL_BACK);

    /* Enable shading */
    glEnable (GL_LIGHTING);
    glEnable (GL_NORMALIZE); /* Tell OpenGL to renormalize normal vector
                              after transformation */
    /* initialize two light sources */
    glEnable (GL_LIGHT0);
    glLightfv (GL_LIGHT0, GL_AMBIENT, light0_color);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, light0_color);
    glLightfv (GL_LIGHT0, GL_SPECULAR, light0_color);
    glEnable (GL_LIGHT1);
    glLightfv (GL_LIGHT1, GL_AMBIENT, light1_color);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light1_color);
    glLightfv (GL_LIGHT1, GL_SPECULAR, light1_color);
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, 40);

    glEnableClientState(GL_VERTEX_ARRAY);
}

/*--------------------------------*
 * GLUT display callback function *
 *--------------------------------*/
void displayCallback (GLFWwindow *win)
{
    /* clear the window */
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadMatrixf(glm::value_ptr(camera_cf));
/*
    glBegin (GL_LINES);
    glColor3ub (255, 0, 0);
    glVertex3i (0, 0, 0);
    glVertex3i (2, 0, 0);
    glColor3ub (0, 255, 0);
    glVertex3i (0, 0, 0);
    glVertex3i (0, 2, 0);
    glColor3ub (0, 0, 255);
    glVertex3i (0, 0, 0);
    glVertex3i (0, 0, 2);
    glEnd();
*/

    /* Specify the reflectance property of the ground using glColor
       (instead of glMaterial....)
     */



    glEnable (GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3ub (29, 100, 56);

    glBegin (GL_QUADS);
    const int GROUND_SIZE = 40;
    glNormal3f (0.0f, 0.0f, 1.0f); /* normal vector for the ground */
    glVertex2i (GROUND_SIZE, GROUND_SIZE);
    glNormal3f (0.0f, 0.0f, 1.0f); /* normal vector for the ground */
    glVertex2i (-GROUND_SIZE, GROUND_SIZE);
    glNormal3f (0.0f, 0.0f, 1.0f); /* normal vector for the ground */
    glVertex2i (-GROUND_SIZE, -GROUND_SIZE);
    glNormal3f (0.0f, 0.0f, 1.0f); /* normal vector for the ground */
    glVertex2i (GROUND_SIZE, -GROUND_SIZE);
    glEnd();
   glDisable (GL_COLOR_MATERIAL);



    /* place the light source in the scene. */
    glLightfv (GL_LIGHT0, GL_POSITION, glm::value_ptr(glm::column(light0_cf, 3)));

    /* recall that the last column of a CF is the origin of the CF */
    glLightfv (GL_LIGHT1, GL_POSITION, glm::value_ptr(glm::column(light1_cf, 3)));

    /* we use the Z-axis of the light CF as the spotlight direction */
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, glm::value_ptr(glm::column(light1_cf, 2)));

    /* the curly bracket pairs below are only for readability */
    glPushMatrix();
    {
        glMultMatrixf(glm::value_ptr(light0_cf));

        /* Render light-0 as an emmisive object */
        if (glIsEnabled(GL_LIGHT0))
            glMaterialfv(GL_FRONT, GL_EMISSION, light0_color);
        //sphere.render();
        glMaterialfv(GL_FRONT, GL_EMISSION, black_color);
    }
    glPopMatrix();






    /* render the spot light using its coordinate frame */
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(light1_cf));
   // spot->render();
    glPopMatrix();

    //Ground
    glPushMatrix();
    static float PEWTER_AMBIENT[] = {0.105882, 0.058824, 0.113725, 1.0};
    static float PEWTER_DIFFUSE[] = {0.427451, 0.470588, 0.541176, 1.0};
    static float PEWTER_SPECULAR[] = {0.333333, 0.333333, 0.521569, 1.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT, PEWTER_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, PEWTER_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, PEWTER_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);

    glTranslatef(0,0,-2.3);
    glScalef(100, 100, .5);
    //ground.render(false);
    glPopMatrix();

    glPushMatrix();
    // glTranslatef(1, 1, 10);
    glMultMatrixf(glm::value_ptr(light1_cf));
    gluSphere(sun,.2, 30, 30);
    glPopMatrix();


    glPushMatrix();
    //glTranslatef(0,0,10);
    glMultMatrixf(glm::value_ptr(heli_cf));
    heli.render(false);
    glPopMatrix();

    /* The following nesting of push-pop pairs create an easy
     * way to render different object w.r.t other coordinate
     * frame.
     *
     * The swingarm is rendered w.r.t the swing base frame
     * The wheel is rendered w.r.t the swing arm frame
     */

    glPopMatrix();
    /* to make smooth transition between frame */
    glfwSwapBuffers(win);
}

void myModelInit ()
{
    ground.build(10,10,glm::vec3{.7, .7, .7}, 2);
    heli.build(4, 2, glm::vec3{0,.5,.2});


    sun = gluNewQuadric();

    active = &camera_cf;


    light0_cf = glm::translate(glm::vec3{-25, 8, 26});

    light1_cf = glm::translate(glm::vec3{0, -10, 25});
    light1_cf = light1_cf * glm::rotate (glm::radians(-120.0f), glm::vec3{1,0,0});
}

void keyCallback (GLFWwindow *win, int key, int scan_code, int action, int mods) {
    if (action == GLFW_RELEASE) return; /* ignore key release action */

    if (mods == GLFW_MOD_SHIFT) {
        switch (key) {
            case GLFW_KEY_UP: /* tilt */
                *active *= glm::rotate(glm::radians(-3.0f), glm::vec3{1.0f, 0.0f, 0.0f});

                break;
            case GLFW_KEY_DOWN: /* tilt */
                *active *= glm::rotate(glm::radians(+3.0f), glm::vec3{1.0f, 0.0f, 0.0f});
                break;
            case GLFW_KEY_LEFT: /* pan left */
                *active *= glm::rotate(glm::radians(-3.0f), glm::vec3{0.0f, 1.0f, 0.0f});
                break;
            case GLFW_KEY_RIGHT: /* pan right */
                *active *= glm::rotate(glm::radians(+3.0f), glm::vec3{0.0f, 1.0f, 0.0f});
                break;
            case GLFW_KEY_X:
                *active *= glm::translate(glm::vec3{1, 0, 0});
                break;
            case GLFW_KEY_Y:
                *active *= glm::translate(glm::vec3{0, 1, 0});
                break;
            case GLFW_KEY_Z:
                *active *= glm::translate(glm::vec3{0, 0, 1});
                break;
            default:
                break;
        };

    }
        //Helicopter rotation control
    else if(mods == GLFW_MOD_CONTROL) {
        switch (key) {
            case GLFW_KEY_UP: /* tilt */
                if(heli.currXangle > heli.MIN_X_ANGLE) {

                    heli.currXangle -= 3;
                    heli_cf *= glm::rotate(glm::radians(3.0f), glm::vec3{0.0f, 1.0f, 0.0f});
                    cout << heli.currXangle << endl;
                }
                break;
            case GLFW_KEY_DOWN: /* tilt */
                if(heli.currXangle < heli.MAX_X_ANGLE) {
                    heli.currXangle += 3;
                    heli_cf *= glm::rotate(glm::radians(-3.0f), glm::vec3{0.0f, 1.0f, 0.0f});

                }
                break;
            case GLFW_KEY_LEFT: /* pan left */
                if(heli.currYangle > heli.MIN_Y_ANGLE) {
                    heli.currYangle -= 3;
                    heli_cf *= glm::rotate(glm::radians(-3.0f), glm::vec3{1.0f, 0.0f, 0.0f});
                    cout << heli.currYangle << endl;

                }
                break;
            case GLFW_KEY_RIGHT: /* pan right */
                if(heli.currYangle < heli.MAX_Y_ANGLE) {
                    heli.currYangle += 3;
                    heli_cf *= glm::rotate(glm::radians(+3.0f), glm::vec3{1.0f, 0.0f, 0.0f});
                }
                break;
            case GLFW_KEY_SLASH: /* pan right */
                heli_cf *= glm::rotate(glm::radians(-3.0f), glm::vec3{0.0f, 0.0f, 1.0f});
                break;
            case GLFW_KEY_APOSTROPHE: /* pan right */
                heli_cf *= glm::rotate(glm::radians(+3.0f), glm::vec3{0.0f, 0.0f, 1.0f});
                break;
            case GLFW_KEY_F: /* Faster propeller speed */
                cout << sizeof(PROP_SPEEDS)/4 << endl;
                    if(propSpeedMode + 1 < sizeof(PROP_SPEEDS)/4) propSpeedMode++ ;
                break;
            case GLFW_KEY_S: /* Slower propeller speed */
                if(propSpeedMode - 1 > -1) propSpeedMode-- ;
                break;

//            case GLFW_KEY_R: /* Reset propeller angle */
//
//                heli_cf *= glm::rotate(glm::radians((1.0f * heli.currXangle)), glm::vec3{1.0f, 0.0f, 0.0f});
//                heli_cf *= glm::rotate(glm::radians((1.0f * heli.currYangle)), glm::vec3{0.0f, 1.0f, 0.0f});
//                heli.currXangle = 0;
//                heli.currYangle = 0;
//                break;
        }
    }

    else {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                exit(0);
            case GLFW_KEY_0:
                active = &light0_cf;
                if (glIsEnabled(GL_LIGHT0))
                    glDisable(GL_LIGHT0);
                else
                    glEnable(GL_LIGHT0);
                break;
            case GLFW_KEY_1:
                active = &light1_cf;
                if (glIsEnabled(GL_LIGHT1))
                    glDisable(GL_LIGHT1);
                else
                    glEnable(GL_LIGHT1);
                break;

            case GLFW_KEY_SPACE: /* pause the animation */
                is_anim_running ^= true;
                break;

            case GLFW_KEY_UP: /* tilt */
                camera_cf *= glm::translate(glm::vec3{0, 0, -1});
                break;
            case GLFW_KEY_DOWN: /* tilt */
                camera_cf *= glm::translate(glm::vec3{0, 0, 1});
                break;
            case GLFW_KEY_LEFT: /* pan left */
                camera_cf *= glm::translate(glm::vec3{-1, 0, 0});
                break;
            case GLFW_KEY_RIGHT: /* pan right */
                camera_cf *= glm::translate(glm::vec3{1, 0, 0});
                break;


            case GLFW_KEY_C:
                active = &camera_cf;
                break;
            case GLFW_KEY_F:
                //active = &frame_cf;
                break;
            case GLFW_KEY_X:
                *active *= glm::translate(glm::vec3{-1, 0, 0});
                break;
            case GLFW_KEY_Y:
                *active *= glm::translate(glm::vec3{0, -1, 0});
                break;
            case GLFW_KEY_Z:
                *active *= glm::translate(glm::vec3{0, 0, -1});
                break;
        }
    }
}



/*
    The virtual trackball technique implemented here is based on:
    https://www.opengl.org/wiki/Object_Mouse_Trackball
*/
void cursor_handler (GLFWwindow *win, double xpos, double ypos) {
    int state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT);
    static glm::vec3 first_click;
    static glm::mat4 current_cam;
    static bool is_tracking = false;
//rotate around xz axis
    glm::vec3 this_vec;
    if (state == GLFW_PRESS) {
        float x = (xpos - screen_ctr_x);
        float y = -(ypos - screen_ctr_y);
        float hypot_square = x * x + y * y;
        float z;

        /* determine whether the mouse is on the sphere or on the hyperbolic sheet */
        if (2 * hypot_square < arc_ball_rad_square)
            z = sqrt(arc_ball_rad_square - hypot_square);
        else
            z = arc_ball_rad_square / 2.0 / sqrt(hypot_square);
        if (!is_tracking) {
            /* store the mouse position when the button was pressed for the first time */
            first_click = glm::normalize(glm::vec3{x, y, z});
            current_cam = camera_cf;
            is_tracking = true;
        }
        else {
            /* compute the rotation w.r.t the initial click */
            this_vec = glm::normalize(glm::vec3{x, y, z});
            /* determine axis of rotation */
            glm::vec3 N = glm::cross(first_click, this_vec);

            /* determine the angle of rotation */
            float theta = glm::angle(first_click, this_vec);

            /* create a quaternion of the rotation */
            glm::quat q{cos(theta / 2), sin(theta / 2) * N};
            /* apply the rotation w.r.t to the current camera CF */
            camera_cf = current_cam * glm::toMat4(glm::normalize(q));
        }
        displayCallback(win);
    }
    else {
        is_tracking = false;
    }
}


//OLD ONE
//void cursor_handler (GLFWwindow *win, double xpos, double ypos) {
//    int state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT);
//    static glm::vec3 first_click;
//    static glm::mat4 current_cam;
//    static bool is_tracking = false;
////rotate around xz axis
//    glm::vec3 this_vec;
//    if (state == GLFW_PRESS) {
//        float x = (xpos - screen_ctr_x);
//        float y = -(ypos - screen_ctr_y);
//        float hypot_square = x * x + y * y;
//        float z;
//
//        /* determine whether the mouse is on the sphere or on the hyperbolic sheet */
//        if (2 * hypot_square < arc_ball_rad_square)
//            z = sqrt(arc_ball_rad_square - hypot_square);
//        else
//            z = arc_ball_rad_square / 2.0 / sqrt(hypot_square);
//        if (!is_tracking) {
//            /* store the mouse position when the button was pressed for the first time */
//            first_click = glm::normalize(glm::vec3{x, y, z});
//            current_cam = camera_cf;
//            is_tracking = true;
//        }
//        else {
//            /* compute the rotation w.r.t the initial click */
//            this_vec = glm::normalize(glm::vec3{x, y, z});
//            /* determine axis of rotation */
//            glm::vec3 N = glm::cross(first_click, this_vec);
//
//            /* determine the angle of rotation */
//            float theta = glm::angle(first_click, this_vec);
//
//            /* create a quaternion of the rotation */
//            glm::quat q{cos(theta / 2), sin(theta / 2) * N};
//            /* apply the rotation w.r.t to the current camera CF */
//            camera_cf = current_cam * glm::toMat4(glm::normalize(q));
//        }
//        displayCallback(win);
//    }
//    else {
//        is_tracking = false;
//    }
//}

void scroll_handler (GLFWwindow *win, double xscroll, double yscroll) {
    /* translate along the camera Z-axis */
    glm::mat4 z_translate = glm::translate((float)yscroll * glm::vec3{0, 0, 1});
    camera_cf =  z_translate * camera_cf;
    displayCallback(win);

}




int main (int argc, char **argv)
{

    glfwInit();
    GLFWwindow *win;
    win = glfwCreateWindow(100, 50, "Animation", NULL, NULL);

    glfwMakeContextCurrent(win);
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf (stderr, "GLEW init error %s\n",
                glewGetErrorString(err));
        exit (EXIT_FAILURE);
    }

    srand (time(0));

    myGLInit ();
    myModelInit ();

    /* setup display callback function */
    glfwSetFramebufferSizeCallback(win, reshapeCallback);
    glfwSetWindowRefreshCallback(win, displayCallback);
    glfwSetKeyCallback(win, keyCallback);
    glfwSetCursorPosCallback(win, cursor_handler);
    glfwSetScrollCallback(win, scroll_handler);
    glfwSetWindowSize(win, 800, 600);

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        updateCoordFrames();
        displayCallback(win);
    }
    glfwDestroyWindow(win);
    glfwTerminate();

}
