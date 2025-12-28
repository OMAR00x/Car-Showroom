#pragma once
#include "Model.h"
#include <string>

class Car {
public:
    float x, y, z;
    float angle;
    float doorOpenAngle;
    bool isOpening;
    Model* carModel;

    Car();
    ~Car();
    Car(const Car&) = delete;
    Car& operator=(const Car&) = delete;
    
    void init(const std::string& modelPath);
    void update();
    void draw();
    void toggleDoor();
};
