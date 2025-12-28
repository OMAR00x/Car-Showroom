#pragma once
#include <GL/glew.h>
#include "BaseHall.h"
#include "Car.h"


class LuxuryCarHall : public BaseHall {
private:
    GLuint floorTexture;
    GLuint wallTexture;
    float hallSize;
    Car luxuryCar;
    Model pedestal; // „‰’… ⁄—÷ ··”Ì«—…

    GLuint loadTexture(const char* filename);
    void drawFloor();
    void drawCeiling();
    void drawWalls();
    void setupLights();

public:
    LuxuryCarHall();
    void init() override;
    void update() override;
    void draw() override;
    void handleInteraction() override {}
};
