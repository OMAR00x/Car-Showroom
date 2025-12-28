#pragma once
#include <GL/glew.h>
#include "BaseHall.h"
#include "Car.h"


class ClassicCarHall : public BaseHall {
private:
    GLuint floorTexture;
    GLuint wallTexture;
    float hallSize;
    Car classicCar;

    GLuint loadTexture(const char* filename);
    void drawFloor();
    void drawCeiling();
    void drawWalls();
    void drawPillars();

public:
    ClassicCarHall();
    void init() override;
    void update() override;
    void draw() override;
    void handleInteraction() override {}
};
