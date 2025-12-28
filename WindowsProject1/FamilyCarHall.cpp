#include "FamilyCarHall.h"
#include "stb_image.h"
#include <GL/glut.h>

FamilyCarHall::FamilyCarHall() {
    hallSize = 40.0f; // ﬁ«⁄… √Ê”⁄
}

void FamilyCarHall::init() {
    // !! «” »œ· »„”«—«  «·’Ê— «·ÕﬁÌﬁÌ… ·œÌﬂ
    floorTexture = loadTexture("textures/floor.jpg");
    wallTexture = loadTexture("textures/wall.jpg");
    windowTexture = loadTexture("textures/floor.jpg"); // „‰Ÿ— ÿ»Ì⁄Ì ··‰«›–…

    // !! «” »œ· »„”«— «·„ÊœÌ· «·ÕﬁÌﬁÌ
    familyCar.init("models/Shelby/Shelby.obj");
    familyCar.y = 0.1;
}

void FamilyCarHall::update() {
    familyCar.update();
}

void FamilyCarHall::draw() {
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);

    drawFloor();
    drawWalls();
    drawCeiling();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_COLOR_MATERIAL);

    glPushMatrix();
    glTranslatef(5, 0, -10);
    familyCar.draw();
    glPopMatrix();
}

GLuint FamilyCarHall::loadTexture(const char* filename) {
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
    return textureID;
}

void FamilyCarHall::drawFloor() {
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-hallSize, 0.0f, -hallSize);
    glTexCoord2f(5.0f, 0.0f); glVertex3f(hallSize, 0.0f, -hallSize);
    glTexCoord2f(5.0f, 5.0f); glVertex3f(hallSize, 0.0f, hallSize);
    glTexCoord2f(0.0f, 5.0f); glVertex3f(-hallSize, 0.0f, hallSize);
    glEnd();
}

void FamilyCarHall::drawWalls() {
    glBindTexture(GL_TEXTURE_2D, wallTexture);

    //  ⁄—Ì› «·„ €Ì— „—… Ê«Õœ… ›ﬁÿ
    float wallHeight = 20.0f;
    float texRepeat = 8.0f; // Ì„ﬂ‰ “Ì«œ… «· ﬂ—«— ·√‰ «·ﬁ«⁄… √Ê”⁄

    glBegin(GL_QUADS);

    // --- «·Ãœ«— «·Œ·›Ì (”‰Ã⁄· Ã“¡« „‰Â ‰«›–…) ---
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-hallSize, 0.0f, -hallSize);
    glTexCoord2f(texRepeat, 0.0f); glVertex3f(hallSize, 0.0f, -hallSize);
    glTexCoord2f(texRepeat, 1.0f); glVertex3f(hallSize, wallHeight, -hallSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-hallSize, wallHeight, -hallSize);

    // --- «·Ãœ«— «·√Ì”— ---
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-hallSize, 0.0f, hallSize);
    glTexCoord2f(texRepeat, 0.0f); glVertex3f(-hallSize, 0.0f, -hallSize);
    glTexCoord2f(texRepeat, 1.0f); glVertex3f(-hallSize, wallHeight, -hallSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-hallSize, wallHeight, hallSize);

    // --- «·Ãœ«— «·√Ì„‰ (Â‰« ”‰÷⁄ «·‰«›–… «·ﬂ»Ì—…) ---
    // ·« ‰—”„Â Â‰«° √Ê ‰—”„ √Ã“«¡ „‰Â ›ﬁÿ

    glEnd();

    // --- —”„ «·‰«›–… «·ﬂ»Ì—… «·‘›«›… ---
    glEnable(GL_BLEND); //  ›⁄Ì· «·‘›«›Ì…
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, windowTexture);
    glColor4f(1.0f, 1.0f, 1.0f, 0.7f); // ·Ê‰ “Ã«ÃÌ „⁄ ‘›«›Ì… 70%

    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(hallSize, wallHeight - 2.0f, -hallSize + 5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(hallSize, wallHeight - 2.0f, hallSize - 5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(hallSize, 2.0f, hallSize - 5.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(hallSize, 2.0f, -hallSize + 5.0f);
    glEnd();

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // ≈⁄«œ… «··Ê‰ ·Ê÷⁄Â «·ÿ»Ì⁄Ì
    glDisable(GL_BLEND); // ≈Ìﬁ«› «·‘›«›Ì…
}
void FamilyCarHall::drawCeiling() {
    glBindTexture(GL_TEXTURE_2D, wallTexture); // Ì„ﬂ‰ «” Œœ«„ Œ«„… «·”ﬁ› ‰›”Â« ··Ãœ—«‰

    //  ⁄—Ì› «·„ €Ì— „—… Ê«Õœ… ›ﬁÿ
    float ceilingHeight = 20.0f;

    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-hallSize, ceilingHeight, -hallSize);
    glTexCoord2f(10.0f, 0.0f); glVertex3f(hallSize, ceilingHeight, -hallSize);
    glTexCoord2f(10.0f, 10.0f); glVertex3f(hallSize, ceilingHeight, hallSize);
    glTexCoord2f(0.0f, 10.0f); glVertex3f(-hallSize, ceilingHeight, hallSize);
    glEnd();
}