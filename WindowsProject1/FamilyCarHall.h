#pragma once
#include <GL/glew.h>
#include "BaseHall.h"
#include "Car.h"


class FamilyCarHall : public BaseHall {
private:
    GLuint floorTexture;
    GLuint wallTexture;
    GLuint windowTexture; // ‰”ÌÃ ··‰«›–…
    float hallSize;
    Car familyCar;

    GLuint loadTexture(const char* filename);
    void drawFloor();
    void drawCeiling();
    void drawWalls();

public:
    FamilyCarHall();
    void init() override;
    void update() override;
    void draw() override;
    void handleInteraction() override {
        familyCar.toggleDoor();
    }
};
