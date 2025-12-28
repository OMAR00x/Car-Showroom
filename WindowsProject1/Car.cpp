#include "Car.h"
#include <GL/glut.h>

Car::Car() {
    x = 0.0f;
    y = 0.1f;
    z = 0.0f;
    angle = 90.0f;
    doorOpenAngle = 0.0f;
    isOpening = false;
    carModel = nullptr;
}

Car::~Car() {
    if(carModel) delete carModel;
}

void Car::init(const std::string& modelPath) {
    carModel = new Model();
    if (!carModel->load(modelPath)) {
        delete carModel;
        carModel = nullptr;
    }
}

void Car::update() {
    angle += 0.2f;
    if (angle > 360.0f) angle -= 360.0f;

    if (isOpening && doorOpenAngle < 60.0f) {
        doorOpenAngle += 2.0f;
    }
    else if (!isOpening && doorOpenAngle > 0.0f) {
        doorOpenAngle -= 2.0f;
    }
}

void Car::toggleDoor() {
    isOpening = !isOpening;
}

void Car::draw() {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glScalef(1.012f, 1.012f, 1.012f);

    if(carModel) carModel->draw();

    glPopMatrix();
}
