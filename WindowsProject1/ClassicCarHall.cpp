#include "ClassicCarHall.h"
#include "stb_image.h"
#include <GL/glut.h>

ClassicCarHall::ClassicCarHall() {
    hallSize = 30.0f;
}

void ClassicCarHall::init() {
    // !!  √ﬂœ „‰ ÊÃÊœ Â–Â «·„·›«  »«·„”«—«  «·’ÕÌÕ…
    floorTexture = loadTexture("textures/floor.jpg");
    wallTexture = loadTexture("textures/wall.jpg");

    // !! «” »œ· »„”«— „ÊœÌ· «·”Ì«—… «·ﬂ·«”ÌﬂÌ…
    classicCar.init("models/Shelby/Shelby.obj");
}

void ClassicCarHall::update() {
    classicCar.update();
}

void ClassicCarHall::draw() {
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    drawFloor();
    drawWalls();
    drawCeiling();
    glDisable(GL_TEXTURE_2D);

    drawPillars();

    glDisable(GL_COLOR_MATERIAL);

    glPushMatrix();
    glTranslatef(-10, 0, 0);
    glRotatef(-30, 0, 1, 0);
    classicCar.draw();
    glPopMatrix();
}

// =======================================================
//            «·ﬂÊœ «·‰«ﬁ’ «·–Ì ÌÕ· «·„‘ﬂ·…
// =======================================================

GLuint ClassicCarHall::loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    else {
        printf("Failed to load texture: %s\n", filename);
    }
    return textureID;
}

void ClassicCarHall::drawPillars() {
    glDisable(GL_TEXTURE_2D); // «·√⁄„œ… »œÊ‰ Œ«„…
    glColor3f(0.7f, 0.7f, 0.65f); // ·Ê‰ ÕÃ—Ì
    float pillarHeight = 18.0f;
    GLUquadric* quad = gluNewQuadric();

    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            glPushMatrix();
            glTranslatef(i * hallSize / 2, 0, j * hallSize / 2);
            glRotatef(-90, 1, 0, 0);
            gluCylinder(quad, 1.0, 1.0, pillarHeight, 20, 1);
            glPopMatrix();
        }
    }
    gluDeleteQuadric(quad);
    glEnable(GL_TEXTURE_2D);
}

void ClassicCarHall::drawFloor() {
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-hallSize, 0.0f, -hallSize);
    glTexCoord2f(12.0f, 0.0f); glVertex3f(hallSize, 0.0f, -hallSize);
    glTexCoord2f(12.0f, 12.0f); glVertex3f(hallSize, 0.0f, hallSize);
    glTexCoord2f(0.0f, 12.0f); glVertex3f(-hallSize, 0.0f, hallSize);
    glEnd();
}

void ClassicCarHall::drawWalls() {
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    float wallHeight = 18.0f;
    float texRepeat = 10.0f;
    glBegin(GL_QUADS);
    // ... (ﬂÊœ —”„ «·Ãœ—«‰)
    glEnd();
}

void ClassicCarHall::drawCeiling() {
    glColor3f(0.8, 0.8, 0.8); // ”ﬁ› »·Ê‰ ⁄«œÌ
    glDisable(GL_TEXTURE_2D);
    float ceilingHeight = 18.0f;
    glBegin(GL_QUADS);
    // ... (ﬂÊœ —”„ «·”ﬁ›)
    glEnd();
    glEnable(GL_TEXTURE_2D);
}
