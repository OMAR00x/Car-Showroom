#pragma once
#include <GL/glew.h>
#include <string>


class Model {
private:
    // Pointer to private implementation (PIMPL Idiom)
    // ��� ������� ���� �� ������ ����� ������� �� ��� �����
    struct ModelImpl;
    ModelImpl* pimpl;

public:
    Model();
    ~Model(); // ���� ����� ������ �������

    // ����� ����� ���� ����� �������
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    bool load(const std::string& filename);
    void draw();
    void drawWithColor(float r, float g, float b);
};
