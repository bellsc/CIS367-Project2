/**
* Helicopter animation and lighting
*
* @author Scott Bell
* Simulates a helicopter flying with a spotlight.
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


#include "Shapes/UnitCube.h"
#include "Helicopter/Propeller.h"
#include "Helicopter/Helicopter.h"
#include "Tree.h"

using namespace std;
void displayCallback(GLFWwindow*);


UnitCube ground;
Helicopter heli;
vector<Tree> treeTypes;
vector<Tree> treeVec;
vector<glm::vec3> treeSpacings;


glm::mat4 heli_cf;
glm::mat4 camera_cf, light1_cf, light0_cf, spotlight_cf;
glm::mat4 *active;

float arc_ball_rad_square;
int screen_ctr_x, screen_ctr_y;

int propSpeedMode = 0;
const float PROP_SPEEDS[] = {0, 600, 1000, 1400};
const int NUM_TREES = 36;
const float TREE_SPACING = 40;

const float GRAVITY = 9.8;   /* m/sec^2 */
bool is_anim_running = true;

/* light source setting */
//GLfloat light0_color[] = {1.0, 1.0, 1.0, 1.0};   /* color */
GLfloat light0_color[] = {.5, .5, .5, 1.0};   /* color */

GLfloat light1_color[] = {1.0, 1.0, .6, 1.0};  /* color */
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

    gluPerspective (60, (float) w / (float) h, 2.0, 500.0);

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
    static float prop_angle = 0;
    float delta, current;

    current = glfwGetTime();
    if (is_anim_running) {
        delta = (current - last_timestamp);
        prop_angle = PROP_SPEEDS[propSpeedMode] * delta;

        glm::vec4 modelOrigin = heli_cf * glm::vec4{0, 0, 0, 1};
        if (modelOrigin.z >= 0) {

            glm::vec3 gravVec = {0, 0, -GRAVITY};
            float liftForce = 9.8;
            float multiplier = .598;
            if (modelOrigin.z == 0) {
                liftForce = PROP_SPEEDS[propSpeedMode] * multiplier;
            }
            else {
                liftForce = PROP_SPEEDS[propSpeedMode] * multiplier / modelOrigin.z;
            }

            glm::vec4 modelLiftVec = {0, 0, liftForce, 0};

            glm::vec3 worldLiftVec = glm::vec3(heli_cf * modelLiftVec);
            worldLiftVec += gravVec;


            if (modelOrigin.z > 0 || (modelOrigin.z == 0 && worldLiftVec.z > 0))
                heli_cf = glm::translate(glm::vec3{4*worldLiftVec.x, 4*worldLiftVec.y, worldLiftVec.z} * delta) * heli_cf;


        }
        if (modelOrigin.z < 0) {
            heli_cf *= glm::translate(glm::vec3{0, 0, -modelOrigin.z});
         }


        //Helicopter propellers
        heli.mainProp_cf *= glm::rotate(glm::radians(prop_angle), glm::vec3{0.0f, 0.0f, 1.0f});
        heli.tailProp_cf *= glm::rotate(glm::radians(prop_angle), glm::vec3{0.0f, 0.0f, 1.0f});


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
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, 45);


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



    /* place the light source in the scene. */
    glLightfv (GL_LIGHT0, GL_POSITION, glm::value_ptr(glm::column(light0_cf, 3)));

    /* the curly bracket pairs below are only for readability */
    glPushMatrix();
    {
        glMultMatrixf(glm::value_ptr(light0_cf));

        /* Render light-0 as an emmisive object */
        if (glIsEnabled(GL_LIGHT0))
            glMaterialfv(GL_FRONT, GL_EMISSION, light0_color);
        gluSphere(gluNewQuadric(), 3, 20, 20);
        glMaterialfv(GL_FRONT, GL_EMISSION, black_color);
    }
    glPopMatrix();

    //Place trees
    glPushMatrix();
    for(int i = 0; i < NUM_TREES; i++) {
        glPushMatrix();
        glm::vec3 trans = treeSpacings[i];
        glTranslatef(trans.x, trans.y, trans.z);
        treeVec[i].render(false);
        glPopMatrix();
    }
    glPopMatrix();

    //Ground
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glTranslatef(0,0,-2.3);
    glScalef(300, 300, .5);
    ground.render(false);
    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);

    glPushMatrix();
    glMultMatrixf(glm::value_ptr(heli_cf));
    heli.render(false);
    glPopMatrix();

    /* to make smooth transition between frame */
    glfwSwapBuffers(win);
}

void myModelInit ()
{
    ground.build(150,150,glm::vec3{0, .15, 0}, 2);
    heli.build(4, 2, glm::vec3{0,.5,.2});

    //Randomly generate 10 trees and spacings
    float avgHeight = 14;
    float variation = 8;

    for(int i = 0; i < 10; i++){
        Tree tree;
        treeTypes.push_back(tree);
        float height = avgHeight-variation/2.0f + rand() % (int)variation;
        int leafRows = 3 + rand() % 2;
        treeTypes[i].build(height, leafRows);

    }

    //make array of trees and generate array of spacings
    float rootTreeNum = sqrt(NUM_TREES);
    float startPos = rootTreeNum * TREE_SPACING/2;

    float startX = startPos;
    float startY = startPos;
    for(int i = 0; i < rootTreeNum; i++){
        for(int j = 0; j < rootTreeNum; j++){
            int randTree = rand() % 10;
            treeVec.push_back(treeTypes[randTree]);
            float spaceX = startX + (rand()%(int)(TREE_SPACING))*.7;
            float spaceY = startY + (rand()%(int)(TREE_SPACING))*.7;

            glm::vec3 vec{spaceX, spaceY,-2.3};
            cout << spaceX << " " << spaceY << endl;
            treeSpacings.push_back(vec);

            startY -= TREE_SPACING;
        }
        startY = startPos;
        startX -= TREE_SPACING;

    }

    active = &camera_cf;

    light0_cf = glm::translate(glm::vec3{-100, 8, 50});

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
            case GLFW_KEY_UP: /* tilt forward */
                heli_cf *= glm::rotate(glm::radians(3.0f), glm::vec3{0.0f, 1.0f, 0.0f});

                break;
            case GLFW_KEY_DOWN: /* tilt  backward*/
                heli_cf *= glm::rotate(glm::radians(-3.0f), glm::vec3{0.0f, 1.0f, 0.0f});

                break;
            case GLFW_KEY_LEFT: /* tilt left */
                heli_cf *= glm::rotate(glm::radians(-3.0f), glm::vec3{1.0f, 0.0f, 0.0f});


                break;
            case GLFW_KEY_RIGHT: /* tilt right */
                heli_cf *= glm::rotate(glm::radians(+3.0f), glm::vec3{1.0f, 0.0f, 0.0f});

                break;
            case GLFW_KEY_SLASH: /* rotate left */
                heli_cf *= glm::rotate(glm::radians(3.0f), glm::vec3{0.0f, 0.0f, 1.0f});
                break;
            case GLFW_KEY_APOSTROPHE: /* rotate right */
                heli_cf *= glm::rotate(glm::radians(-3.0f), glm::vec3{0.0f, 0.0f, 1.0f});
                break;
            case GLFW_KEY_F: /* Faster propeller speed */
                    if(propSpeedMode + 1 < sizeof(PROP_SPEEDS)/4) propSpeedMode++ ;
                break;
            case GLFW_KEY_S: /* Slower propeller speed */
                if(propSpeedMode - 1 > -1) propSpeedMode-- ;
                break;

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
    win = glfwCreateWindow(100, 50, "Helicopter", NULL, NULL);

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
