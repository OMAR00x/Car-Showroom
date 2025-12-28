#pragma once
#include <GL/glew.h>
#include <GL/glut.h>

class BaseHall {
public:
    virtual ~BaseHall() {}
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    // ��� ��� ����� ���
    virtual void handleInteraction() {}
};
