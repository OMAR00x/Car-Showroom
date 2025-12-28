#include "LuxuryCarHall.h"
#include "stb_image.h"
#include <GL/glut.h>

LuxuryCarHall::LuxuryCarHall() {
    hallSize = 35.0f;
}

void LuxuryCarHall::init() {
    // !!  √ﬂœ „‰ ÊÃÊœ Â–Â «·„·›«  »«·„”«—«  «·’ÕÌÕ…
    floorTexture = loadTexture("textures/floor.jpg");
    wallTexture = loadTexture("textures/wall.jpg");

    // !! «” »œ· »„”«— „ÊœÌ· «·”Ì«—… «·›«Œ—…
    luxuryCar.init("models/Shelby/Shelby.obj");

    pedestal.load("models/Shelby/Shelby.obj");
    luxuryCar.y = 1.0f; // ·—›⁄ «·”Ì«—… ›Êﬁ «·„‰’…
}

void LuxuryCarHall::update() {
    luxuryCar.update();
}

void LuxuryCarHall::draw() {
    setupLights(); // ≈⁄œ«œ «·≈÷«¡… «·Œ«’… »Â–Â «·ﬁ«⁄…

    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    drawFloor();
    drawWalls();
    drawCeiling();
    glDisable(GL_TEXTURE_2D);

    glDisable(GL_COLOR_MATERIAL);

    // —”„ «·„‰’…
    glPushMatrix();
    glTranslatef(0, 0, -5);
    glScalef(5, 1, 5);
    pedestal.draw();
    glPopMatrix();

    // —”„ «·”Ì«—… ›Êﬁ «·„‰’…
    glPushMatrix();
    glTranslatef(0, 0, -5);
    luxuryCar.draw();
    glPopMatrix();

    // —”„ «‰⁄ﬂ«” «·”Ì«—…
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
    glTranslatef(0, 0, -5);
    glScalef(1.0, -1.0, 1.0); // ﬁ·» «·„ÊœÌ· —√”Ì«
    glColor4f(1.0, 1.0, 1.0, 0.2); // ‘›«›Ì… 20%
    glDisable(GL_LIGHTING); // ·« ‰—Ìœ ≈÷«¡… ⁄·Ï «·«‰⁄ﬂ«”
    luxuryCar.draw();
    glEnable(GL_LIGHTING);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glPopMatrix();
    glDisable(GL_BLEND);

    glDisable(GL_LIGHT1); // ≈ÿ›«¡ «·÷Ê¡ «·Œ«’
    glEnable(GL_LIGHT0);  // ≈⁄«œ… «·÷Ê¡ «·⁄«„
}

// =======================================================
//            «·ﬂÊœ «·‰«ﬁ’ «·–Ì ÌÕ· «·„‘ﬂ·…
// =======================================================

GLuint LuxuryCarHall::loadTexture(const char* filename) {
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

void LuxuryCarHall::setupLights() {
    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    GLfloat light_pos[] = { 0.0f, 10.0f, -5.0f, 1.0f };
    GLfloat spot_dir[] = { 0.0f, -1.0f, 0.0f };
    GLfloat diffuse[] = { 1.0f, 1.0f, 0.9f, 1.0f };

    glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0f);
}

void LuxuryCarHall::drawFloor() {
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-hallSize, 0.0f, -hallSize);
    glTexCoord2f(4.0f, 0.0f); glVertex3f(hallSize, 0.0f, -hallSize);
    glTexCoord2f(4.0f, 4.0f); glVertex3f(hallSize, 0.0f, hallSize);
    glTexCoord2f(0.0f, 4.0f); glVertex3f(-hallSize, 0.0f, hallSize);
    glEnd();
}

void LuxuryCarHall::drawWalls() {
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    float wallHeight = 18.0f;
    float texRepeat = 5.0f;
    glBegin(GL_QUADS);
    // ... (ﬂÊœ —”„ «·Ãœ—«‰)
    glEnd();
}

void LuxuryCarHall::drawCeiling() {
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    float ceilingHeight = 18.0f;
    glBegin(GL_QUADS);
    // ... (ﬂÊœ —”„ «·”ﬁ›)
    glEnd();
}
