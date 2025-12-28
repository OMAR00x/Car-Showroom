// ==================== SportsCarHall.cpp (النسخة الكاملة) ====================

#include "SportsCarHall.h"
#include "stb_image.h"
#include <stdio.h>
#include <GL/glut.h> // <-- نحتاج هذه المكتبة لدالة gluBuild2DMipmaps

// --- الدالة البانية ---
SportsCarHall::SportsCarHall() {
    hallSize = 30.0f;
    floorTexture = 0;
    wallTexture = 0;
}

// --- دالة التجهيز ---
void SportsCarHall::init() {
    // تأكد من أن هذه المسارات صحيحة في مشروعك
    floorTexture = loadTexture("textures/floor.jpg");
    wallTexture = loadTexture("textures/wall.jpg");
    sportCar.init("models/merc/uploads_files_2787791_Mercedes+Benz+GLS+580.obj");
}

// --- دالة التحديث ---
void SportsCarHall::update() {
    sportCar.update();
}

// --- دالة الرسم الرئيسية للقاعة ---
void SportsCarHall::draw() {
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);

    drawFloor();
    drawWalls();
    drawCeiling();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_COLOR_MATERIAL);

    sportCar.draw();
}
// --- الدوال الخاصة بالرسم (التي كانت ناقصة) ---

GLuint SportsCarHall::loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // 1. تحديد سلوك تكرار الخامة
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 2. تحديد فلاتر التصغير والتكبير (مع استخدام Mipmap)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;

        // 3. بناء الخامة مع Mipmaps لتحسين الجودة
        gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }
    else {
        // 4. طباعة رسالة في حال فشل تحميل الصورة
        printf("Failed to load texture: %s\n", filename);
    }

    return textureID;
}
void SportsCarHall::drawFloor() {
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-hallSize, 0.0f, -hallSize);
    glTexCoord2f(10.0f, 0.0f); glVertex3f(hallSize, 0.0f, -hallSize);
    glTexCoord2f(10.0f, 10.0f); glVertex3f(hallSize, 0.0f, hallSize);
    glTexCoord2f(0.0f, 10.0f); glVertex3f(-hallSize, 0.0f, hallSize);
    glEnd();
}

void SportsCarHall::drawWalls() {
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    float wallHeight = 15.0f;
    float texRepeat = 5.0f;
    glBegin(GL_QUADS);
    // ... (كود الجدران)
    // --- الجدار الخلفي ---
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-hallSize, 0.0f, -hallSize);
    glTexCoord2f(texRepeat, 0.0f); glVertex3f(hallSize, 0.0f, -hallSize);
    glTexCoord2f(texRepeat, 1.0f); glVertex3f(hallSize, wallHeight, -hallSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-hallSize, wallHeight, -hallSize);
    // --- الجدار الأيسر ---
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-hallSize, 0.0f, hallSize);
    glTexCoord2f(texRepeat, 0.0f); glVertex3f(-hallSize, 0.0f, -hallSize);
    glTexCoord2f(texRepeat, 1.0f); glVertex3f(-hallSize, wallHeight, -hallSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-hallSize, wallHeight, hallSize);
    // --- الجدار الأيمن ---
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(hallSize, 0.0f, -hallSize);
    glTexCoord2f(texRepeat, 0.0f); glVertex3f(hallSize, 0.0f, hallSize);
    glTexCoord2f(texRepeat, 1.0f); glVertex3f(hallSize, wallHeight, hallSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(hallSize, wallHeight, -hallSize);
    glEnd();
}

void SportsCarHall::drawCeiling() {
    // --- ارسم السقف الأساسي ---
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    float ceilingHeight = 15.0f;
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-hallSize, ceilingHeight, -hallSize);
    glTexCoord2f(10.0f, 0.0f); glVertex3f(hallSize, ceilingHeight, -hallSize);
    glTexCoord2f(10.0f, 10.0f); glVertex3f(hallSize, ceilingHeight, hallSize);
    glTexCoord2f(0.0f, 10.0f); glVertex3f(-hallSize, ceilingHeight, hallSize);
    glEnd();
    // --- ارسم الأضواء واجعلها "مضيئة" ---
    glDisable(GL_TEXTURE_2D);
    GLfloat light_emission[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, light_emission);
    glBegin(GL_QUADS);
    // الضوء الأول
    glVertex3f(-10, ceilingHeight - 0.1f, -5);
    glVertex3f(-2, ceilingHeight - 0.1f, -5);
    glVertex3f(-2, ceilingHeight - 0.1f, 5);
    glVertex3f(-10, ceilingHeight - 0.1f, 5);
    // الضوء الثاني
    glVertex3f(2, ceilingHeight - 0.1f, -5);
    glVertex3f(10, ceilingHeight - 0.1f, -5);
    glVertex3f(10, ceilingHeight - 0.1f, 5);
    glVertex3f(2, ceilingHeight - 0.1f, 5);
    glEnd();
    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    glEnable(GL_TEXTURE_2D);
}