// ==================== main.cpp ====================

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glew.h> 
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <GL/glut.h>
#include "Camera.h"
#include "Showroom.h"
#include <iostream>

Camera camera;
Showroom showroom;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
bool keyStates[256] = { false };

void display();
void reshape(int, int);
void keyboard(unsigned char, int, int);
void keyboardUp(unsigned char, int, int);
void mouseMove(int, int);
void timer(int);
void init();
void processInput();

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("A Legend Car Showroom Project ");

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW failed to initialize!" << std::endl;
        return -1;
    }

    init();

    

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mouseMove);
    glutTimerFunc(0, timer, 0);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    glutMainLoop();
    return 0;
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    showroom.init();
    camera.showroom = &showroom;
    camera.x = 0;
    camera.y = 3.0f;
    camera.z = -60;
    camera.angleYaw = 90.0f;
    camera.updateVectors();
}

void display() {
    glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if(showroom.getDriverSeatMode()) {
        float carX, carY, carZ, carAngle;
        showroom.getDriverCarPos(carX, carY, carZ, carAngle);
        float rad = carAngle * 3.14159f / 180.0f;
        float eyeX = carX + sin(rad) * 0.5f;
        float eyeY = carY + 0.5f;
        float eyeZ = carZ + cos(rad) * 0.5f;
        float lookX = carX + sin(rad) * 10;
        float lookY = carY + 0.5f;
        float lookZ = carZ + cos(rad) * 10;
        gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, 0, 1, 0);
    } else {
        camera.applyView();
    }
    
    showroom.updateCurrentFloor(camera.y);
    showroom.updateCameraPos(camera.x, camera.z);
    showroom.draw();

    glutSwapBuffers();
}

void processInput() {
    camera.processKeys(keyStates);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y) {
    keyStates[key] = true;

    if (key == 27) exit(0);
    if (key == 'e' || key == 'E') {
        showroom.toggleDoor();
    }
    if (key == 'l' || key == 'L') {
        showroom.toggleLights();
    }
    if (key == 'n' || key == 'N') {
        showroom.toggleNightMode();
    }
    if (key == '1') showroom.callElevator(0);
    if (key == '2') showroom.callElevator(1);
    if (key == '3') showroom.callElevator(2);
    if (key == '4') showroom.callElevator(3);
    if (key == 'f' || key == 'F') {
        static bool fullscreen = false;
        if(fullscreen) {
            glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
            fullscreen = false;
        } else {
            glutFullScreen();
            fullscreen = true;
        }
    }
    if (key == 'c' || key == 'C') {
        showroom.interactWithCar();
    }
    if (key == 'v' || key == 'V') {
        showroom.toggleCarWheels();
    }
    if (key == 'r' || key == 'R') {
        showroom.enterCar();
    }
    if (key == 'x' || key == 'X') {
        showroom.toggleEngine();
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void mouseMove(int x, int y) {
    int cx = WINDOW_WIDTH / 2;
    int cy = WINDOW_HEIGHT / 2;
    if (x == cx && y == cy) return;
    camera.handleMouse(x, y, cx, cy);
    glutWarpPointer(cx, cy);
}

void timer(int value) {
    processInput();
    showroom.update();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}
