#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include "Car.h"
#include "BaseHall.h"

class SportsCarHall : public BaseHall {
private:
    GLuint floorTexture;
    GLuint wallTexture;
    float hallSize;
    Car sportCar;

    GLuint loadTexture(const char* filename);
    void drawFloor();
    void drawCeiling();
    void drawWalls();

public:
    SportsCarHall();
    void init() override;
    void update() override;
    void draw() override;
    void handleInteraction() override { sportCar.toggleDoor(); }
};
