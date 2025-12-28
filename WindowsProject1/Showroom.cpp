#include "Showroom.h"
#include "stb_image.h"
#include "model.h"
#include <cmath>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define SND_MUSIC 1
#define SND_DOOR 2
#define SND_ELEVATOR 3

Showroom::Showroom() {
    floorTex = wallTex = glassTex = ceilingTex = roadTex = signTex = skyTex = skyCeilingTex = 0;
    paintingTex[0] = paintingTex[1] = paintingTex[2] = paintingTex[3] = 0;
    carDisplayList = crystalDisplayList = 0;
    doorOpen = 0;
    doorOpening = false;
    carRotation = 0;
    elevatorY = 0;
    elevatorMoving = false;
    elevatorTarget = 0;
    lightsOn = true;
    isNightMode = false;
    currentFloor = 0;
    inDriverSeat = false;
    driverCarX = driverCarY = driverCarZ = driverCarAngle = 0;
    doorSoundPlayed = false;
    elevatorSoundPlayed = false;
    cameraX = cameraZ = 0;
    mainCar = new DetailedCar(0, 1.3f, 40, 0);
    mercModel = nullptr;
    shelbyModel = nullptr;
}

Showroom::~Showroom() {
    if(mainCar) delete mainCar;
    if(mercModel) delete mercModel;
    if(shelbyModel) delete shelbyModel;
}

void Showroom::init() {
    floorTex = loadTexture("textures/floor.jpg");
    wallTex = loadTexture("textures/wall.jpg");
    ceilingTex = loadTexture("textures/ceiling.jpg");
    skyTex = loadTexture("textures/sky.jpg");
    signTex = loadTexture("textures/sign.png");
    
    paintingTex[0] = loadTexture("textures/painting1.jpg");
    paintingTex[1] = loadTexture("textures/painting2.jpg");
    paintingTex[2] = loadTexture("textures/painting3.jpg");
    paintingTex[3] = loadTexture("textures/painting4.jpg");
    
    createDisplayLists();
    
    // Load car models
    mercModel = new Model();
    if(!mercModel->load("models/merc/uploads_files_2787791_Mercedes+Benz+GLS+580.obj")) {
        delete mercModel;
        mercModel = nullptr;
    }
    
    shelbyModel = new Model();
    if(!shelbyModel->load("models/Shelby/Shelby.obj")) {
        delete shelbyModel;
        shelbyModel = nullptr;
    }
}

void Showroom::setSignTexture(const char* path) {
    signTex = loadTexture(path);
}

void Showroom::createDisplayLists() {
    // Car Display List
    carDisplayList = glGenLists(1);
    glNewList(carDisplayList, GL_COMPILE);
    
    glPushMatrix();
    glScalef(4, 0.8f, 2);
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.5f, 0.9f, 0);
    glScalef(2.2f, 0.9f, 1.8f);
    glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.05f, 0.05f, 0.05f);
    glPushMatrix();
    glTranslatef(-1.8f, 0.4f, 0);
    glScalef(0.3f, 0.15f, 2.1f);
    glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.1f, 0.1f, 0.1f);
    float wheelPos[4][2] = {{1.5f, 0.9f}, {1.5f, -0.9f}, {-1.5f, 0.9f}, {-1.5f, -0.9f}};
    for(int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(wheelPos[i][0], -0.3f, wheelPos[i][1]);
        glRotatef(90, 0, 0, 1);
        glutSolidTorus(0.15, 0.35, 8, 16);
        glPopMatrix();
    }
    
    glColor3f(0.9f, 0.9f, 0.1f);
    glPushMatrix();
    glTranslatef(-1.95f, 0.35f, 0.6f);
    glScalef(0.05f, 0.1f, 0.1f);
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-1.95f, 0.35f, -0.6f);
    glScalef(0.05f, 0.1f, 0.1f);
    glutSolidCube(1);
    glPopMatrix();
    
    glEndList();
    
    // Crystal Display List
    crystalDisplayList = glGenLists(1);
    glNewList(crystalDisplayList, GL_COMPILE);
    
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 1, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(-0.3f, 0, 0.3f);
    glVertex3f(0.3f, 0, 0.3f);
    
    glVertex3f(0, 1, 0);
    glVertex3f(0.3f, 0, 0.3f);
    glVertex3f(0.3f, 0, -0.3f);
    
    glVertex3f(0, 1, 0);
    glVertex3f(0.3f, 0, -0.3f);
    glVertex3f(-0.3f, 0, -0.3f);
    
    glVertex3f(0, 1, 0);
    glVertex3f(-0.3f, 0, -0.3f);
    glVertex3f(-0.3f, 0, 0.3f);
    
    glNormal3f(0, -1, 0);
    glVertex3f(0, -0.5f, 0);
    glVertex3f(0.3f, 0, 0.3f);
    glVertex3f(-0.3f, 0, 0.3f);
    
    glVertex3f(0, -0.5f, 0);
    glVertex3f(0.3f, 0, -0.3f);
    glVertex3f(0.3f, 0, 0.3f);
    
    glVertex3f(0, -0.5f, 0);
    glVertex3f(-0.3f, 0, -0.3f);
    glVertex3f(0.3f, 0, -0.3f);
    
    glVertex3f(0, -0.5f, 0);
    glVertex3f(-0.3f, 0, 0.3f);
    glVertex3f(-0.3f, 0, -0.3f);
    glEnd();
    
    glEndList();
}

void Showroom::update() {
    if(doorOpening) {
        if(doorOpen < 5) {
            doorOpen += 0.15f;
        }
    } else {
        if(doorOpen > 0) doorOpen -= 0.15f;
        if(doorOpen < 0) doorOpen = 0;
    }
    
    carRotation += 0.5f;
    if(carRotation > 360) carRotation -= 360;
    
    if(elevatorMoving) {
        float targetY = elevatorTarget * floorHeight;
        if(abs(elevatorY - targetY) < 0.1f) {
            elevatorY = targetY;
            elevatorMoving = false;
            if(!elevatorSoundPlayed) {
                mciSendStringA("close elevator", NULL, 0, NULL);
                mciSendStringA("open \"sounds\\elevetor-arrives.wav\" type waveaudio alias elevator", NULL, 0, NULL);
                mciSendStringA("play elevator", NULL, 0, NULL);
                elevatorSoundPlayed = true;
            }
        } else if(elevatorY < targetY) {
            elevatorY += 0.08f;
            elevatorSoundPlayed = false;
        } else {
            elevatorY -= 0.08f;
            elevatorSoundPlayed = false;
        }
    }
    
    float hw = buildingWidth / 2;
    float hd = buildingDepth / 2;
    bool insideShowroom = (cameraX > -hw && cameraX < hw && cameraZ > -hd && cameraZ < hd);
    
    static bool musicPlaying = false;
    if(insideShowroom && !musicPlaying) {
        PlaySoundA("sounds\\ambient-music.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        musicPlaying = true;
    } else if(!insideShowroom && musicPlaying) {
        PlaySoundA(NULL, NULL, 0);
        musicPlaying = false;
    }
    
    if(mainCar) mainCar->update();
}

void Showroom::draw() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    drawSkybox();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    setupLighting();
    
    drawGround();
    drawSurroundingBuildings();
    
    float hw = buildingWidth / 2;
    float hd = buildingDepth / 2;
    float th = floorHeight * numFloors;
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, wallTex);
    glColor3f(0.95f, 0.95f, 1);
    
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0); glVertex3f(-hw, 0, hd);
    glTexCoord2f(5, 0); glVertex3f(hw, 0, hd);
    glTexCoord2f(5, 2); glVertex3f(hw, th, hd);
    glTexCoord2f(0, 2); glVertex3f(-hw, th, hd);
    
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-hw, 0, hd);
    glTexCoord2f(5, 0); glVertex3f(-hw, 0, -hd);
    glTexCoord2f(5, 2); glVertex3f(-hw, th, -hd);
    glTexCoord2f(0, 2); glVertex3f(-hw, th, hd);
    
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(hw, 0, -hd);
    glTexCoord2f(5, 0); glVertex3f(hw, 0, hd);
    glTexCoord2f(5, 2); glVertex3f(hw, th, hd);
    glTexCoord2f(0, 2); glVertex3f(hw, th, -hd);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    for(int i = 0; i < numFloors; i++) {
        drawFloor(i);
    }
    drawElevatorShaft();
    drawElevator();
    drawEntranceDecor();
    
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    drawGlassFrontWithDoor();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    
    drawHUD();
}

void Showroom::setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat light0_pos[] = {0, 30, 0, 1};
    GLfloat light0_diff[] = {0.5f, 0.5f, 0.5f, 1};
    GLfloat light0_amb[] = {0.4f, 0.4f, 0.4f, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_amb);
    
    if(lightsOn) {
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
        glEnable(GL_LIGHT3);
        glEnable(GL_LIGHT4);
        
        GLfloat light1_pos[] = {0, 4, 20, 1};
        GLfloat light1_diff[] = {0.6f, 0.2f, 0.2f, 1};
        glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diff);
        
        GLfloat light2_pos[] = {0, 12, 20, 1};
        GLfloat light2_diff[] = {0.2f, 0.6f, 0.2f, 1};
        glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diff);
        
        GLfloat light3_pos[] = {0, 20, 20, 1};
        GLfloat light3_diff[] = {0.6f, 0.6f, 0.2f, 1};
        glLightfv(GL_LIGHT3, GL_POSITION, light3_pos);
        glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diff);
        
        GLfloat light4_pos[] = {0, 28, 20, 1};
        GLfloat light4_diff[] = {0.5f, 0.3f, 0.6f, 1};
        glLightfv(GL_LIGHT4, GL_POSITION, light4_pos);
        glLightfv(GL_LIGHT4, GL_DIFFUSE, light4_diff);
    } else {
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT3);
        glDisable(GL_LIGHT4);
    }
}

void Showroom::drawSkybox() {
    glDisable(GL_LIGHTING);
    
    if(skyTex != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, skyTex);
        if(isNightMode) glColor3f(0.3f, 0.3f, 0.4f);
        else glColor3f(1, 1, 1);
    } else {
        glDisable(GL_TEXTURE_2D);
        if(isNightMode) glColor3f(0.1f, 0.1f, 0.2f);
        else glColor3f(0.53f, 0.81f, 0.92f);
    }
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f(-500, 0, -500);
    glTexCoord2f(1, 1); glVertex3f(500, 0, -500);
    glTexCoord2f(1, 0); glVertex3f(500, 200, -500);
    glTexCoord2f(0, 0); glVertex3f(-500, 200, -500);
    
    glTexCoord2f(0, 1); glVertex3f(-500, 0, 500);
    glTexCoord2f(1, 1); glVertex3f(-500, 0, -500);
    glTexCoord2f(1, 0); glVertex3f(-500, 200, -500);
    glTexCoord2f(0, 0); glVertex3f(-500, 200, 500);
    
    glTexCoord2f(0, 1); glVertex3f(500, 0, -500);
    glTexCoord2f(1, 1); glVertex3f(500, 0, 500);
    glTexCoord2f(1, 0); glVertex3f(500, 200, 500);
    glTexCoord2f(0, 0); glVertex3f(500, 200, -500);
    
    glTexCoord2f(0, 1); glVertex3f(500, 0, 500);
    glTexCoord2f(1, 1); glVertex3f(-500, 0, 500);
    glTexCoord2f(1, 0); glVertex3f(-500, 200, 500);
    glTexCoord2f(0, 0); glVertex3f(500, 200, 500);
    glEnd();
    
    if(skyCeilingTex != 0) {
        glBindTexture(GL_TEXTURE_2D, skyCeilingTex);
        if(isNightMode) glColor3f(0.2f, 0.2f, 0.3f);
        else glColor3f(1, 1, 1);
    } else {
        if(isNightMode) glColor3f(0.05f, 0.05f, 0.15f);
        else glColor3f(0.4f, 0.6f, 0.95f);
    }
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-500, 200, -500);
    glTexCoord2f(1, 0); glVertex3f(500, 200, -500);
    glTexCoord2f(1, 1); glVertex3f(500, 200, 500);
    glTexCoord2f(0, 1); glVertex3f(-500, 200, 500);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

void Showroom::drawGround() {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    if(isNightMode) glColor3f(0.15f, 0.15f, 0.18f);
    else glColor3f(0.3f, 0.3f, 0.32f);
    
    float hw = buildingWidth / 2;
    float hd = buildingDepth / 2;
    
    glBegin(GL_QUADS);
    glVertex3f(-500, -0.1f, -500);
    glVertex3f(500, -0.1f, -500);
    glVertex3f(500, -0.1f, 500);
    glVertex3f(-500, -0.1f, 500);
    glEnd();
    
    if(isNightMode) glColor3f(0.8f, 0.8f, 0.2f);
    else glColor3f(1, 1, 0);
    glLineWidth(1);
    glBegin(GL_LINES);
    for(float x = -490; x < 490; x += 40) {
        for(float z = -490; z < -hd; z += 30) {
            glVertex3f(x, 0, z);
            glVertex3f(x, 0, z + 10);
        }
        for(float z = hd; z < 490; z += 30) {
            glVertex3f(x, 0, z);
            glVertex3f(x, 0, z + 10);
        }
    }
    for(float z = -hd; z < hd; z += 30) {
        for(float x = -490; x < -hw; x += 40) {
            glVertex3f(x, 0, z);
            glVertex3f(x, 0, z + 10);
        }
        for(float x = hw; x < 490; x += 40) {
            glVertex3f(x, 0, z);
            glVertex3f(x, 0, z + 10);
        }
    }
    glEnd();
    glLineWidth(1);
}

void Showroom::drawSurroundingBuildings() {
    glEnable(GL_LIGHTING);
    if(isNightMode) glColor3f(0.15f, 0.15f, 0.18f);
    else glColor3f(0.3f, 0.3f, 0.35f);
    
    drawSimpleBuilding(-150, 0, -100, 30, 40, 25);
    drawSimpleBuilding(120, 0, -120, 35, 50, 30);
    drawSimpleBuilding(-140, 0, 150, 25, 35, 20);
    drawSimpleBuilding(130, 0, 140, 40, 45, 35);
    drawSimpleBuilding(0, 0, 180, 50, 30, 40);
}

void Showroom::drawSimpleBuilding(float x, float y, float z, float w, float h, float d) {
    glEnable(GL_LIGHTING);
    glColor3f(0.3f, 0.3f, 0.35f);
    
    glBegin(GL_QUADS);
    // All 6 faces
    glNormal3f(0, 0, -1);
    glVertex3f(x, y, z);
    glVertex3f(x+w, y, z);
    glVertex3f(x+w, y+h, z);
    glVertex3f(x, y+h, z);
    
    glNormal3f(0, 0, 1);
    glVertex3f(x+w, y, z+d);
    glVertex3f(x, y, z+d);
    glVertex3f(x, y+h, z+d);
    glVertex3f(x+w, y+h, z+d);
    
    glNormal3f(-1, 0, 0);
    glVertex3f(x, y, z+d);
    glVertex3f(x, y, z);
    glVertex3f(x, y+h, z);
    glVertex3f(x, y+h, z+d);
    
    glNormal3f(1, 0, 0);
    glVertex3f(x+w, y, z);
    glVertex3f(x+w, y, z+d);
    glVertex3f(x+w, y+h, z+d);
    glVertex3f(x+w, y+h, z);
    
    glNormal3f(0, 1, 0);
    glVertex3f(x, y+h, z);
    glVertex3f(x+w, y+h, z);
    glVertex3f(x+w, y+h, z+d);
    glVertex3f(x, y+h, z+d);
    
    glNormal3f(0, -1, 0);
    glVertex3f(x, y, z);
    glVertex3f(x, y, z+d);
    glVertex3f(x+w, y, z+d);
    glVertex3f(x+w, y, z);
    glEnd();
    
    // Windows
    glDisable(GL_LIGHTING);
    if(isNightMode) glColor3f(0.3f, 0.3f, 0.2f);
    else glColor3f(1, 1, 0.8f);
    for(float wy = 5; wy < h-2; wy += 5) {
        for(float wx = 5; wx < w-2; wx += 5) {
            // Front windows
            glBegin(GL_QUADS);
            glVertex3f(x+wx, wy, z-0.1f);
            glVertex3f(x+wx+2, wy, z-0.1f);
            glVertex3f(x+wx+2, wy+2, z-0.1f);
            glVertex3f(x+wx, wy+2, z-0.1f);
            glEnd();
            
            // Back windows
            glBegin(GL_QUADS);
            glVertex3f(x+wx, wy, z+d+0.1f);
            glVertex3f(x+wx+2, wy, z+d+0.1f);
            glVertex3f(x+wx+2, wy+2, z+d+0.1f);
            glVertex3f(x+wx, wy+2, z+d+0.1f);
            glEnd();
        }
    }
    glEnable(GL_LIGHTING);
}

void Showroom::drawMainBuilding() {
    float hw = buildingWidth / 2;
    float hd = buildingDepth / 2;
    float th = floorHeight * numFloors;
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, wallTex);
    glColor3f(0.95f, 0.95f, 1);
    
    glBegin(GL_QUADS);
    
    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0); glVertex3f(-hw, 0, hd);
    glTexCoord2f(5, 0); glVertex3f(hw, 0, hd);
    glTexCoord2f(5, 2); glVertex3f(hw, th, hd);
    glTexCoord2f(0, 2); glVertex3f(-hw, th, hd);
    
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-hw, 0, -hd);
    glTexCoord2f(5, 0); glVertex3f(-hw, 0, hd);
    glTexCoord2f(5, 2); glVertex3f(-hw, th, hd);
    glTexCoord2f(0, 2); glVertex3f(-hw, th, -hd);
    
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(hw, 0, hd);
    glTexCoord2f(5, 0); glVertex3f(hw, 0, -hd);
    glTexCoord2f(5, 2); glVertex3f(hw, th, -hd);
    glTexCoord2f(0, 2); glVertex3f(hw, th, hd);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    drawGlassFrontWithDoor();
    
    for(int i = 0; i < numFloors; i++) {
        drawFloor(i);
    }
}

void Showroom::drawGlassFrontWithDoor() {
    float hw = buildingWidth / 2;
    float hd = buildingDepth / 2;
    float th = floorHeight * numFloors;
    float doorMax = 5.0f;
    float actualDoorOpen = (doorOpen > doorMax) ? doorMax : doorOpen;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    
    // Glass panels
    glColor4f(0.6f, 0.7f, 0.85f, 0.3f);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    
    glVertex3f(-hw, 0, -hd + 0.1f);
    glVertex3f(-5.05f, 0, -hd + 0.1f);
    glVertex3f(-5.05f, th, -hd + 0.1f);
    glVertex3f(-hw, th, -hd + 0.1f);
    
    glVertex3f(5.05f, 0, -hd + 0.1f);
    glVertex3f(hw, 0, -hd + 0.1f);
    glVertex3f(hw, th, -hd + 0.1f);
    glVertex3f(5.05f, th, -hd + 0.1f);
    
    glVertex3f(-5.05f, 5, -hd + 0.1f);
    glVertex3f(5.05f, 5, -hd + 0.1f);
    glVertex3f(5.05f, th, -hd + 0.1f);
    glVertex3f(-5.05f, th, -hd + 0.1f);
    glEnd();
    
    // Door panels (close completely)
    glColor4f(0.4f, 0.5f, 0.7f, 0.5f);
    glBegin(GL_QUADS);
    glVertex3f(-5 - actualDoorOpen, 0, -hd + 0.05f);
    glVertex3f(-actualDoorOpen, 0, -hd + 0.05f);
    glVertex3f(-actualDoorOpen, 5, -hd + 0.05f);
    glVertex3f(-5 - actualDoorOpen, 5, -hd + 0.05f);
    
    glVertex3f(actualDoorOpen, 0, -hd + 0.05f);
    glVertex3f(5 + actualDoorOpen, 0, -hd + 0.05f);
    glVertex3f(5 + actualDoorOpen, 5, -hd + 0.05f);
    glVertex3f(actualDoorOpen, 5, -hd + 0.05f);
    glEnd();
    
    glEnable(GL_LIGHTING);
    
    // Door tracks
    glColor3f(0.3f, 0.3f, 0.35f);
    glBegin(GL_QUADS);
    glVertex3f(-10, 5.05f, -hd + 0.2f);
    glVertex3f(10, 5.05f, -hd + 0.2f);
    glVertex3f(10, 5.2f, -hd + 0.2f);
    glVertex3f(-10, 5.2f, -hd + 0.2f);
    
    glVertex3f(-10, 0, -hd + 0.2f);
    glVertex3f(10, 0, -hd + 0.2f);
    glVertex3f(10, 0.05f, -hd + 0.2f);
    glVertex3f(-10, 0.05f, -hd + 0.2f);
    glEnd();
    
    // Frame lines
    glDisable(GL_LIGHTING);
    glColor3f(0.1f, 0.1f, 0.15f);
    glLineWidth(3);
    
    glBegin(GL_LINE_LOOP);
    glVertex3f(-hw, 0, -hd - 0.05f);
    glVertex3f(hw, 0, -hd - 0.05f);
    glVertex3f(hw, th, -hd - 0.05f);
    glVertex3f(-hw, th, -hd - 0.05f);
    glEnd();
    
    for(int i = 1; i < numFloors; i++) {
        float y = i * floorHeight;
        glBegin(GL_LINES);
        glVertex3f(-hw, y, -hd - 0.05f);
        glVertex3f(hw, y, -hd - 0.05f);
        glEnd();
    }
    
    for(float x = -hw + 10; x < hw; x += 12) {
        glBegin(GL_LINES);
        glVertex3f(x, 0, -hd - 0.05f);
        glVertex3f(x, th, -hd - 0.05f);
        glEnd();
    }
    
    glLineWidth(1);
}

void Showroom::drawEntranceDecor() {
    float hd = buildingDepth / 2;
    
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    
    // Sign background
    glColor3f(0.1f, 0.1f, 0.15f);
    glPushMatrix();
    glTranslatef(0, 5.5f, -hd - 0.2f);
    glScalef(8, 2, 0.2f);
    glutSolidCube(1);
    glPopMatrix();
    
    // Sign front
    if(signTex != 0) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, signTex);
        glColor4f(1, 1, 1, 1);
        
        glPushMatrix();
        glTranslatef(0, 5.5f, -hd - 0.35f);
        glBegin(GL_QUADS);
        glTexCoord2f(1, 1); glVertex3f(-3.5f, -0.8f, 0);
        glTexCoord2f(0, 1); glVertex3f(3.5f, -0.8f, 0);
        glTexCoord2f(0, 0); glVertex3f(3.5f, 0.8f, 0);
        glTexCoord2f(1, 0); glVertex3f(-3.5f, 0.8f, 0);
        glEnd();
        glPopMatrix();
        
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
    } else {
        glColor3f(0.2f, 0.2f, 0.25f);
        glPushMatrix();
        glTranslatef(0, 5.5f, -hd - 0.35f);
        glBegin(GL_QUADS);
        glVertex3f(-3.5f, -0.8f, 0);
        glVertex3f(3.5f, -0.8f, 0);
        glVertex3f(3.5f, 0.8f, 0);
        glVertex3f(-3.5f, 0.8f, 0);
        glEnd();
        glPopMatrix();
    }
    
    glColor3f(0.1f, 0.6f, 0.2f);
    for(int side = -1; side <= 1; side += 2) {
        float x = side * 25;
        glPushMatrix();
        glTranslatef(x, 0, -hd - 3);
        glColor3f(0.4f, 0.3f, 0.2f);
        glPushMatrix();
        glScalef(0.5f, 3, 0.5f);
        glutSolidCube(1);
        glPopMatrix();
        
        glColor3f(0.1f, 0.6f, 0.2f);
        glTranslatef(0, 3, 0);
        glutSolidSphere(2, 16, 16);
        glPopMatrix();
    }
    
    glEnable(GL_LIGHTING);
}

void Showroom::drawFloor(int level) {
    float y = level * floorHeight;
    float hw = buildingWidth / 2;
    float hd = buildingDepth / 2;
    float sx = buildingWidth/2 - 10;
    float sz = buildingDepth/2 - 15;
    
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, floorTex);
    glColor3f(0.85f, 0.85f, 0.9f);
    
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    // Floor with elevator opening
    glTexCoord2f(0, 0); glVertex3f(-hw, y + 0.1f, -hd);
    glTexCoord2f(3, 0); glVertex3f(sx - 2.5f, y + 0.1f, -hd);
    glTexCoord2f(3, 8); glVertex3f(sx - 2.5f, y + 0.1f, hd);
    glTexCoord2f(0, 8); glVertex3f(-hw, y + 0.1f, hd);
    
    glTexCoord2f(3, 0); glVertex3f(sx + 2.5f, y + 0.1f, -hd);
    glTexCoord2f(6, 0); glVertex3f(hw, y + 0.1f, -hd);
    glTexCoord2f(6, 8); glVertex3f(hw, y + 0.1f, hd);
    glTexCoord2f(3, 8); glVertex3f(sx + 2.5f, y + 0.1f, hd);
    
    glTexCoord2f(0, 0); glVertex3f(sx - 2.5f, y + 0.1f, -hd);
    glTexCoord2f(1, 0); glVertex3f(sx - 2.5f, y + 0.1f, sz - 4.5f);
    glTexCoord2f(1, 1); glVertex3f(sx + 2.5f, y + 0.1f, sz - 4.5f);
    glTexCoord2f(0, 1); glVertex3f(sx + 2.5f, y + 0.1f, -hd);
    
    glTexCoord2f(0, 0); glVertex3f(sx - 2.5f, y + 0.1f, sz + 0.5f);
    glTexCoord2f(1, 0); glVertex3f(sx - 2.5f, y + 0.1f, hd);
    glTexCoord2f(1, 1); glVertex3f(sx + 2.5f, y + 0.1f, hd);
    glTexCoord2f(0, 1); glVertex3f(sx + 2.5f, y + 0.1f, sz + 0.5f);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    // Ceiling with elevator opening (only for floors 0, 1, 2)
    if(level < 3) {
        glEnable(GL_TEXTURE_2D);
        if(ceilingTex != 0) {
            glBindTexture(GL_TEXTURE_2D, ceilingTex);
        }
        glBegin(GL_QUADS);
        glNormal3f(0, -1, 0);
        // Left part
        glTexCoord2f(0, 0); glVertex3f(-hw, y + floorHeight - 0.1f, -hd);
        glTexCoord2f(3, 0); glVertex3f(-hw, y + floorHeight - 0.1f, hd);
        glTexCoord2f(3, 8); glVertex3f(sx - 2.5f, y + floorHeight - 0.1f, hd);
        glTexCoord2f(0, 8); glVertex3f(sx - 2.5f, y + floorHeight - 0.1f, -hd);
        // Right part
        glTexCoord2f(3, 0); glVertex3f(sx + 2.5f, y + floorHeight - 0.1f, -hd);
        glTexCoord2f(6, 0); glVertex3f(sx + 2.5f, y + floorHeight - 0.1f, hd);
        glTexCoord2f(6, 8); glVertex3f(hw, y + floorHeight - 0.1f, hd);
        glTexCoord2f(3, 8); glVertex3f(hw, y + floorHeight - 0.1f, -hd);
        // Front part
        glTexCoord2f(0, 0); glVertex3f(sx - 2.5f, y + floorHeight - 0.1f, -hd);
        glTexCoord2f(1, 0); glVertex3f(sx - 2.5f, y + floorHeight - 0.1f, sz - 4.5f);
        glTexCoord2f(1, 1); glVertex3f(sx + 2.5f, y + floorHeight - 0.1f, sz - 4.5f);
        glTexCoord2f(0, 1); glVertex3f(sx + 2.5f, y + floorHeight - 0.1f, -hd);
        // Back part
        glTexCoord2f(0, 0); glVertex3f(sx - 2.5f, y + floorHeight - 0.1f, sz + 0.5f);
        glTexCoord2f(1, 0); glVertex3f(sx - 2.5f, y + floorHeight - 0.1f, hd);
        glTexCoord2f(1, 1); glVertex3f(sx + 2.5f, y + floorHeight - 0.1f, hd);
        glTexCoord2f(0, 1); glVertex3f(sx + 2.5f, y + floorHeight - 0.1f, sz + 0.5f);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    } else {
        // Full ceiling for top floor
        glEnable(GL_TEXTURE_2D);
        if(ceilingTex != 0) {
            glBindTexture(GL_TEXTURE_2D, ceilingTex);
        }
        glBegin(GL_QUADS);
        glNormal3f(0, -1, 0);
        glTexCoord2f(0, 0); glVertex3f(-hw, y + floorHeight - 0.1f, -hd);
        glTexCoord2f(6, 0); glVertex3f(-hw, y + floorHeight - 0.1f, hd);
        glTexCoord2f(6, 8); glVertex3f(hw, y + floorHeight - 0.1f, hd);
        glTexCoord2f(0, 8); glVertex3f(hw, y + floorHeight - 0.1f, -hd);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    
    drawCeilingLights(level);
    drawNeonLines(level);
    drawShowroomPlatforms(level);
    
    // Draw painting on right wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, paintingTex[level]);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 1); glVertex3f(hw - 0.1f, y + 2, 10);
    glTexCoord2f(1, 1); glVertex3f(hw - 0.1f, y + 2, 25);
    glTexCoord2f(1, 0); glVertex3f(hw - 0.1f, y + 6, 25);
    glTexCoord2f(0, 0); glVertex3f(hw - 0.1f, y + 6, 10);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void Showroom::drawShowroomPlatforms(int level) {
    glDisable(GL_TEXTURE_2D);
    if(level == 0) {
        drawSportsHall(level);
    } else if(level == 1) {
        drawFamilyHall(level);
    } else if(level == 2) {
        drawClassicHall(level);
    } else if(level == 3) {
        drawLuxuryHall(level);
    }
    glEnable(GL_TEXTURE_2D);
}

void Showroom::drawSportsHall(int level) {
    float y = level * floorHeight;
    
    glEnable(GL_LIGHTING);
    
    if(level == 0 && mainCar) {
        mainCar->draw();
    }
    
    drawRotatingPlatform(-15, y, 22, 7, carRotation);
    drawCar(-15, y + 1.3f, 22, carRotation, 0);
    drawCarLabel(-15, y + 1.3f, 22, "Ferrari F8", "$280,000");
    drawCrystal(-25, y, 15, 2.5f, 0, 1, 1, carRotation);
    drawCrystal(-5, y, 15, 2, 1, 0, 1, -carRotation);
    
    drawRotatingPlatform(20, y, 22, 7, -carRotation);
    drawCar(20, y + 1.3f, 22, -carRotation, 0);
    drawCarLabel(20, y + 1.3f, 22, "Lamborghini", "$350,000");
    drawCrystal(10, y, 15, 2, 1, 0.5f, 0, carRotation * 0.7f);
    
    drawCrystal(-10, y + 0.5f, 35, 1.8f, 1, 1, 0, carRotation * 0.3f);
    drawCrystal(10, y + 0.5f, 35, 1.8f, 0, 1, 0.5f, -carRotation * 0.3f);
}

void Showroom::drawFamilyHall(int level) {
    float y = level * floorHeight;
    
    glEnable(GL_LIGHTING);
    
    if(lightsOn) {
        glEnable(GL_LIGHT5);
        GLfloat spot_pos[] = {-15, y + 6, 22, 1};
        GLfloat spot_dir[] = {0, -1, 0};
        GLfloat spot_diff[] = {0.3f, 1, 0.3f, 1};
        glLightfv(GL_LIGHT5, GL_POSITION, spot_pos);
        glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot_dir);
        glLightfv(GL_LIGHT5, GL_DIFFUSE, spot_diff);
        glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 30.0f);
        glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10.0f);
        
        glEnable(GL_LIGHT6);
        GLfloat spot_pos2[] = {20, y + 6, 22, 1};
        glLightfv(GL_LIGHT6, GL_POSITION, spot_pos2);
        glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, spot_dir);
        glLightfv(GL_LIGHT6, GL_DIFFUSE, spot_diff);
        glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 30.0f);
        glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 10.0f);
        
        glEnable(GL_LIGHT7);
        GLfloat spot_pos3[] = {0, y + 6, 40, 1};
        glLightfv(GL_LIGHT7, GL_POSITION, spot_pos3);
        glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, spot_dir);
        glLightfv(GL_LIGHT7, GL_DIFFUSE, spot_diff);
        glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 30.0f);
        glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 10.0f);
    }
    
    drawRotatingPlatform(-15, y, 22, 7, carRotation * 0.7f);
    drawCar(-15, y + 1.3f, 22, carRotation * 0.7f, 1);
    drawCarLabel(-15, y + 1.3f, 22, "Toyota Camry", "$28,000");
    drawCrystal(-25, y, 15, 2, 0, 1, 0, carRotation * 0.5f);
    drawCrystal(-5, y, 15, 2.5f, 0.5f, 1, 0.5f, -carRotation * 0.5f);
    
    drawRotatingPlatform(20, y, 22, 7, -carRotation * 0.7f);
    drawCar(20, y + 1.3f, 22, -carRotation * 0.7f, 1);
    drawCarLabel(20, y + 1.3f, 22, "Honda Accord", "$26,000");
    drawCrystal(10, y, 15, 2.2f, 0, 0.8f, 1, carRotation * 0.6f);
    
    drawRotatingPlatform(0, y, 40, 7, carRotation * 0.3f);
    drawCar(0, y + 1.3f, 40, carRotation * 0.3f, 1);
    drawCarLabel(0, y + 1.3f, 40, "Ford Fusion", "$24,000");
    drawCrystal(-10, y + 0.5f, 35, 1.5f, 0, 1, 1, carRotation * 0.4f);
    drawCrystal(10, y + 0.5f, 35, 1.5f, 1, 1, 0, -carRotation * 0.4f);
    
    if(lightsOn) {
        glDisable(GL_LIGHT5);
        glDisable(GL_LIGHT6);
        glDisable(GL_LIGHT7);
    }
}

void Showroom::drawLuxuryHall(int level) {
    float y = level * floorHeight;
    
    glEnable(GL_LIGHTING);
    
    if(lightsOn) {
        glEnable(GL_LIGHT5);
        GLfloat spot_pos[] = {-15, y + 6, 22, 1};
        GLfloat spot_dir[] = {0, -1, 0};
        GLfloat spot_diff[] = {1, 1, 0.3f, 1};
        glLightfv(GL_LIGHT5, GL_POSITION, spot_pos);
        glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot_dir);
        glLightfv(GL_LIGHT5, GL_DIFFUSE, spot_diff);
        glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 30.0f);
        glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10.0f);
        
        glEnable(GL_LIGHT6);
        GLfloat spot_pos2[] = {20, y + 6, 22, 1};
        glLightfv(GL_LIGHT6, GL_POSITION, spot_pos2);
        glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, spot_dir);
        glLightfv(GL_LIGHT6, GL_DIFFUSE, spot_diff);
        glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 30.0f);
        glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 10.0f);
        
        glEnable(GL_LIGHT7);
        GLfloat spot_pos3[] = {0, y + 6, 40, 1};
        glLightfv(GL_LIGHT7, GL_POSITION, spot_pos3);
        glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, spot_dir);
        glLightfv(GL_LIGHT7, GL_DIFFUSE, spot_diff);
        glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 30.0f);
        glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 10.0f);
    }
    
    drawRotatingPlatform(-15, y, 22, 7, carRotation * 0.3f);
    drawCar(-15, y + 1.3f, 22, carRotation * 0.3f, 2);
    drawCarLabel(-15, y + 1.3f, 22, "Rolls Royce", "$450,000");
    drawCrystal(-25, y, 15, 3, 1, 1, 0, carRotation * 0.2f);
    drawCrystal(-5, y, 15, 2.5f, 1, 0.8f, 0, -carRotation * 0.2f);
    
    drawRotatingPlatform(20, y, 22, 7, -carRotation * 0.3f);
    drawCar(20, y + 1.3f, 22, -carRotation * 0.3f, 2);
    drawCarLabel(20, y + 1.3f, 22, "Bentley", "$380,000");
    drawCrystal(10, y, 15, 2.8f, 1, 0.9f, 0.3f, carRotation * 0.25f);
    
    drawRotatingPlatform(0, y, 40, 7, carRotation * 0.2f);
    drawCar(0, y + 1.3f, 40, carRotation * 0.2f, 2);
    drawCarLabel(0, y + 1.3f, 40, "Mercedes S", "$110,000");
    drawCrystal(-10, y + 0.5f, 35, 2, 1, 0.7f, 0, carRotation * 0.15f);
    drawCrystal(10, y + 0.5f, 35, 2, 0.9f, 0.9f, 0, -carRotation * 0.15f);
    
    if(lightsOn) {
        glDisable(GL_LIGHT5);
        glDisable(GL_LIGHT6);
        glDisable(GL_LIGHT7);
    }
}

void Showroom::drawClassicHall(int level) {
    float y = level * floorHeight;
    
    glEnable(GL_LIGHTING);
    
    if(lightsOn) {
        glEnable(GL_LIGHT5);
        GLfloat spot_pos[] = {-15, y + 6, 22, 1};
        GLfloat spot_dir[] = {0, -1, 0};
        GLfloat spot_diff[] = {0.8f, 0.5f, 1, 1};
        glLightfv(GL_LIGHT5, GL_POSITION, spot_pos);
        glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot_dir);
        glLightfv(GL_LIGHT5, GL_DIFFUSE, spot_diff);
        glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 30.0f);
        glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10.0f);
        
        glEnable(GL_LIGHT6);
        GLfloat spot_pos2[] = {20, y + 6, 22, 1};
        glLightfv(GL_LIGHT6, GL_POSITION, spot_pos2);
        glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, spot_dir);
        glLightfv(GL_LIGHT6, GL_DIFFUSE, spot_diff);
        glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 30.0f);
        glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 10.0f);
        
        glEnable(GL_LIGHT7);
        GLfloat spot_pos3[] = {0, y + 6, 40, 1};
        glLightfv(GL_LIGHT7, GL_POSITION, spot_pos3);
        glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, spot_dir);
        glLightfv(GL_LIGHT7, GL_DIFFUSE, spot_diff);
        glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 30.0f);
        glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 10.0f);
    }
    
    drawRotatingPlatform(-15, y, 22, 7, carRotation * 0.4f);
    drawCar(-15, y + 1.3f, 22, carRotation * 0.4f, 0);
    drawCarLabel(-15, y + 1.3f, 22, "Ford Mustang", "$55,000");
    drawCrystal(-25, y, 15, 2.5f, 0.5f, 0, 0.5f, carRotation * 0.3f);
    drawCrystal(-5, y, 15, 2, 0.8f, 0.5f, 1, -carRotation * 0.3f);
    
    drawRotatingPlatform(20, y, 22, 7, -carRotation * 0.4f);
    drawCar(20, y + 1.3f, 22, -carRotation * 0.4f, 1);
    drawCarLabel(20, y + 1.3f, 22, "Chevy Camaro", "$42,000");
    drawCrystal(10, y, 15, 2.3f, 0.6f, 0.3f, 0.8f, carRotation * 0.35f);
    drawCrystal(30, y, 15, 2.5f, 0.4f, 0, 0.6f, -carRotation * 0.35f);
    
    drawRotatingPlatform(0, y, 40, 7, carRotation * 0.25f);
    drawCar(0, y + 1.3f, 40, carRotation * 0.25f, 2);
    drawCarLabel(0, y + 1.3f, 40, "Corvette", "$65,000");
    drawCrystal(-10, y + 0.3f, 35, 2, 0.7f, 0.4f, 0.9f, carRotation * 0.2f);
    drawCrystal(10, y + 0.3f, 35, 2, 0.5f, 0.2f, 0.7f, -carRotation * 0.2f);
    
    if(lightsOn) {
        glDisable(GL_LIGHT5);
        glDisable(GL_LIGHT6);
        glDisable(GL_LIGHT7);
    }
}

void Showroom::drawPlatform(float x, float y, float z, float w, float d, int type) {
    // Platform removed - cars sit directly on floor
}

void Showroom::drawCar(float x, float y, float z, float angle, int type) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, 0, 1, 0);
    
    // Use models for some cars, keep simple cars for others
    if(type == 0 && shelbyModel) {
        glTranslatef(0, -1.5f, 0);
        shelbyModel->draw();
    } else if(type == 1 && mercModel) {
        glTranslatef(0, -0.2f, 0);
        mercModel->draw();
    } else {
        // Fallback to simple car
        glScalef(1.5f, 1.5f, 1.5f);
        if(type == 0) glColor3f(1, 0.1f, 0.1f);
        else if(type == 1) glColor3f(0.2f, 0.4f, 0.9f);
        else glColor3f(0.1f, 0.1f, 0.1f);
        glCallList(carDisplayList);
    }
    
    glPopMatrix();
}



void Showroom::drawCeilingLights(int level) {
    if(!lightsOn) return;
    
    float y = level * floorHeight + floorHeight - 0.2f;
    float sx = buildingWidth/2 - 10;
    
    glDisable(GL_LIGHTING);
    GLfloat emission[] = {0.9f, 0.9f, 0.8f, 1};
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    for(float x = -30; x <= 30; x += 20) {
        for(float z = 10; z <= 40; z += 15) {
            glColor4f(1, 1, 0.9f, 0.9f);
            glPushMatrix();
            glTranslatef(x, y, z);
            glScalef(3, 0.1f, 3);
            glutSolidCube(1);
            glPopMatrix();
            
            glColor4f(0.8f, 0.9f, 1.0f, 0.3f);
            glPushMatrix();
            glTranslatef(x, y - 0.3f, z);
            glScalef(3.5f, 0.05f, 3.5f);
            glutSolidCube(1);
            glPopMatrix();
        }
    }
    
    glLineWidth(2);
    glColor4f(0.5f, 0.7f, 1.0f, 0.6f);
    for(float x = -30; x <= 30; x += 20) {
        if(x < sx - 5 || x > sx + 5) {
            glBegin(GL_LINES);
            glVertex3f(x, y, 10);
            glVertex3f(x, y, 40);
            glEnd();
        }
    }
    glLineWidth(1);
    
    glDisable(GL_BLEND);
    GLfloat noEmission[] = {0, 0, 0, 1};
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
    glEnable(GL_LIGHTING);
}

void Showroom::drawFloorSign(int level) {
    float y = level * floorHeight;
    float hw = buildingWidth / 2;
    
    glDisable(GL_LIGHTING);
    glColor3f(0.2f, 0.2f, 0.25f);
    glPushMatrix();
    glTranslatef(hw - 5, y + 4, 0);
    glRotatef(90, 0, 1, 0);
    glScalef(8, 3, 0.2f);
    glutSolidCube(1);
    glPopMatrix();
    
    if(level == 0) glColor3f(1, 0.3f, 0.3f);
    else if(level == 1) glColor3f(0.3f, 1, 0.3f);
    else glColor3f(1, 1, 0.3f);
    
    glPushMatrix();
    glTranslatef(hw - 4.8f, y + 5, 0);
    glRotatef(90, 0, 1, 0);
    glScalef(7, 2, 0.1f);
    glutSolidCube(1);
    glPopMatrix();
    
    glEnable(GL_LIGHTING);
}

void Showroom::toggleDoor() {
    doorOpening = !doorOpening;
    
    float hw = buildingWidth / 2;
    float hd = buildingDepth / 2;
    
    mciSendStringA("close door", NULL, 0, NULL);
    mciSendStringA("open \"sounds\\glass-door.wav\" type waveaudio alias door", NULL, 0, NULL);
    mciSendStringA("play door", NULL, 0, NULL);
}

bool Showroom::checkCollision(float x, float z, float radius) {
    return checkBuildingCollision(x, z) || checkElevatorCollision(x, z);
}

bool Showroom::checkBuildingCollision(float x, float z) {
    float hw = buildingWidth / 2 - 2;
    float hd = buildingDepth / 2 - 2;
    
    if(x < -hw || x > hw) return true;
    if(z < -hd || z > hd) return true;
    
    return false;
}

bool Showroom::checkElevatorCollision(float x, float z) {
    float sx = buildingWidth/2 - 10;
    float sz = buildingDepth/2 - 15;
    
    if(x > sx - 3 && x < sx + 3 && z > sz - 5 && z < sz + 1) {
        return true;
    }
    
    return false;
}

GLuint Showroom::loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    
    if(data) {
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    
    return textureID;
}

void Showroom::drawCrystal(float x, float y, float z, float size, float r, float g, float b, float rotation) {
    glPushMatrix();
    glTranslatef(x, y + size * 0.5f, z);
    glRotatef(rotation, 0, 1, 0);
    glRotatef(rotation * 0.5f, 1, 0, 0);
    glScalef(size, size, size);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    GLfloat emission[] = {r * 0.4f, g * 0.4f, b * 0.4f, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    glColor4f(r, g, b, 0.75f);
    
    glCallList(crystalDisplayList);
    
    GLfloat noEmission[] = {0, 0, 0, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);
    glDisable(GL_BLEND);
    glPopMatrix();
}

void Showroom::drawRotatingPlatform(float x, float y, float z, float radius, float rotation) {
    glPushMatrix();
    glTranslatef(x, y + 0.2f, z);
    glRotatef(rotation, 0, 1, 0);
    
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0.2f, 0.4f, 0.8f, 0.3f);
    glVertex3f(0, 0, 0);
    for(int i = 0; i <= 32; i++) {
        float angle = i * 2.0f * 3.14159f / 32.0f;
        float cx = cos(angle) * radius;
        float cz = sin(angle) * radius;
        glColor4f(0.4f, 0.6f, 1.0f, 0.5f);
        glVertex3f(cx, 0, cz);
    }
    glEnd();
    
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glColor4f(0.5f, 0.8f, 1.0f, 0.8f);
    for(int i = 0; i < 32; i++) {
        float angle = i * 2.0f * 3.14159f / 32.0f;
        glVertex3f(cos(angle) * radius, 0.01f, sin(angle) * radius);
    }
    glEnd();
    
    for(int i = 0; i < 8; i++) {
        float angle = i * 2.0f * 3.14159f / 8.0f;
        glBegin(GL_LINES);
        glColor4f(0.6f, 0.9f, 1.0f, 0.6f);
        glVertex3f(0, 0.01f, 0);
        glVertex3f(cos(angle) * radius, 0.01f, sin(angle) * radius);
        glEnd();
    }
    glLineWidth(1);
    
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void Showroom::drawNeonLines(int level) {
    if(!lightsOn) return;
    
    float y = level * floorHeight;
    float hw = buildingWidth / 2;
    float hd = buildingDepth / 2;
    
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.5f);
    
    if(level == 0) glColor4f(1, 0.2f, 0.2f, 0.6f);
    else if(level == 1) glColor4f(0.2f, 1, 0.2f, 0.6f);
    else if(level == 2) glColor4f(1, 1, 0.2f, 0.6f);
    else glColor4f(0.6f, 0.3f, 1, 0.6f);
    
    glBegin(GL_LINE_STRIP);
    glVertex3f(-hw + 5, y + 1, hd - 0.2f);
    glVertex3f(hw - 5, y + 1, hd - 0.2f);
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glVertex3f(-hw + 5, y + floorHeight - 1, hd - 0.2f);
    glVertex3f(hw - 5, y + floorHeight - 1, hd - 0.2f);
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glVertex3f(-hw + 0.2f, y + 1, -hd + 5);
    glVertex3f(-hw + 0.2f, y + 1, hd - 5);
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glVertex3f(hw - 0.2f, y + 1, -hd + 5);
    glVertex3f(hw - 0.2f, y + 1, hd - 5);
    glEnd();
    
    glLineWidth(1);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void Showroom::setSkyCeilingTexture(const char* path) {
    skyCeilingTex = loadTexture(path);
}

void Showroom::toggleLights() {
    lightsOn = !lightsOn;
}

void Showroom::toggleNightMode() {
    isNightMode = !isNightMode;
}

void Showroom::callElevator(int floor) {
    if(floor >= 0 && floor < numFloors) {
        elevatorTarget = floor;
        elevatorMoving = true;
    }
}

void Showroom::drawElevatorShaft() {
    float sx = buildingWidth/2 - 10;
    float sz = buildingDepth/2 - 15;
    float th = floorHeight * numFloors;
    
    glEnable(GL_LIGHTING);
    glColor3f(0.4f, 0.4f, 0.45f);
    
    glBegin(GL_QUADS);
    // Left wall
    glNormal3f(1, 0, 0);
    glVertex3f(sx - 2, 0, sz);
    glVertex3f(sx - 2, 0, sz - 4);
    glVertex3f(sx - 2, th, sz - 4);
    glVertex3f(sx - 2, th, sz);
    
    // Right wall
    glNormal3f(-1, 0, 0);
    glVertex3f(sx + 2, 0, sz - 4);
    glVertex3f(sx + 2, 0, sz);
    glVertex3f(sx + 2, th, sz);
    glVertex3f(sx + 2, th, sz - 4);
    
    // Back wall
    glNormal3f(0, 0, 1);
    glVertex3f(sx - 2, 0, sz);
    glVertex3f(sx + 2, 0, sz);
    glVertex3f(sx + 2, th, sz);
    glVertex3f(sx - 2, th, sz);
    glEnd();
}

void Showroom::drawElevator() {
    float sx = buildingWidth/2 - 10;
    float sz = buildingDepth/2 - 15;
    
    glEnable(GL_LIGHTING);
    glColor3f(0.6f, 0.6f, 0.65f);
    
    glPushMatrix();
    glTranslatef(sx, elevatorY + 0.1f, sz - 2);
    
    glBegin(GL_QUADS);
    // Floor
    glNormal3f(0, 1, 0);
    glVertex3f(-1.8f, 0, -1.8f);
    glVertex3f(1.8f, 0, -1.8f);
    glVertex3f(1.8f, 0, 1.8f);
    glVertex3f(-1.8f, 0, 1.8f);
    
    // Back wall
    glNormal3f(0, 0, 1);
    glVertex3f(-1.8f, 0, 1.8f);
    glVertex3f(1.8f, 0, 1.8f);
    glVertex3f(1.8f, 5.5f, 1.8f);
    glVertex3f(-1.8f, 5.5f, 1.8f);
    
    // Right wall
    glNormal3f(1, 0, 0);
    glVertex3f(1.8f, 0, -1.8f);
    glVertex3f(1.8f, 0, 1.8f);
    glVertex3f(1.8f, 5.5f, 1.8f);
    glVertex3f(1.8f, 5.5f, -1.8f);
    
    // Left wall
    glNormal3f(-1, 0, 0);
    glVertex3f(-1.8f, 0, 1.8f);
    glVertex3f(-1.8f, 0, -1.8f);
    glVertex3f(-1.8f, 5.5f, -1.8f);
    glVertex3f(-1.8f, 5.5f, 1.8f);
    
    // Ceiling
    glNormal3f(0, 1, 0);
    glVertex3f(-1.8f, 5.5f, -1.8f);
    glVertex3f(1.8f, 5.5f, -1.8f);
    glVertex3f(1.8f, 5.5f, 1.8f);
    glVertex3f(-1.8f, 5.5f, 1.8f);
    glEnd();
    
    // Floor display
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(0, 5.3f, 1.75f);
    glTranslatef(0, 5.3f, 1.76f);
    glRasterPos3f(-0.1f, 0, 0);      

    glPopMatrix();
    
    // Buttons
    for(int i = 0; i < 4; i++) {
        int elevatorFloor = (int)(elevatorY / floorHeight + 0.5f);
        if(i == elevatorFloor) glColor3f(0, 1, 0);
        else glColor3f(0.3f, 0.3f, 0.35f);
        
        glPushMatrix();
        glTranslatef(-1.0f, 1.5f + i * 0.8f, 1.75f);
        glScalef(0.3f, 0.3f, 0.05f);
        glutSolidCube(1);
        glPopMatrix();
        
        glColor3f(1, 1, 1);
        glRasterPos3f(-1.6f, 1.5f + i * 0.8f, 1.8f);
        char btn[2] = {'1' + i, '\0'};
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, btn[0]);
    }
    
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void Showroom::drawHUD() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1280, 0, 720);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Background panel
    glColor4f(0.1f, 0.1f, 0.15f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(20, 650);
    glVertex2f(280, 650);
    glVertex2f(280, 700);
    glVertex2f(20, 700);
    glEnd();
    
    // Border
    glColor4f(0.3f, 0.5f, 0.8f, 0.9f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(20, 650);
    glVertex2f(280, 650);
    glVertex2f(280, 700);
    glVertex2f(20, 700);
    glEnd();
    glLineWidth(1);
    
    // Floor indicator
    const char* floorNames[] = {"Floor 1: Sports Cars", "Floor 2: Family Cars", "Floor 3: Classic Cars", "Floor 4: Luxury Cars"};
    glColor3f(1, 1, 1);
    glRasterPos2f(30, 675);
    const char* text = floorNames[currentFloor];
    for(int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
    
    // Controls info
    glColor4f(0.1f, 0.1f, 0.15f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(20, 340);
    glVertex2f(280, 340);
    glVertex2f(280, 630);
    glVertex2f(20, 630);
    glEnd();
    
    glColor4f(0.3f, 0.5f, 0.8f, 0.9f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(20, 340);
    glVertex2f(280, 340);
    glVertex2f(280, 630);
    glVertex2f(20, 630);
    glEnd();
    glLineWidth(1);
    
    glColor3f(0.8f, 0.9f, 1);
    glRasterPos2f(70, 605);
    const char* title = "Controls:";
    for(int i = 0; title[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title[i]);
    }
    
    glColor3f(1, 1, 1);
    const char* controls[] = {
        "WASD - Move",
        "Mouse - Look",
        "E - Door",
        "L - Lights",
        "N - Night Mode",
        "1-4 - Elevator",
        "C - Car Door",
        "R - Enter Car",
        "X - Engine",
        "V - Wheels",
        "F - Fullscreen"
    };
    
    for(int i = 0; i < 11; i++) {
        glRasterPos2f(30, 585 - i * 24);
        for(int j = 0; controls[i][j] != '\0'; j++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, controls[i][j]);
        }
    }
    
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    
    // Show "Press R to enter car" message when near a car
    if(!inDriverSeat && currentFloor == 0 && mainCar && mainCar->isNear(cameraX, cameraZ)) {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        
        glColor4f(0.2f, 0.8f, 0.2f, 0.8f);
        glRasterPos2f(500, 360);
        const char* msg = "Press R to enter car";
        for(int i = 0; msg[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);
        }
        
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
    } else if(inDriverSeat) {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        
        glColor4f(0.8f, 0.2f, 0.2f, 0.8f);
        glRasterPos2f(480, 360);
        const char* msg = "R-Exit | X-Engine";
        for(int i = 0; msg[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);
        }
        
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Showroom::updateCurrentFloor(float cameraY) {
    if(cameraY < floorHeight * 0.5f) currentFloor = 0;
    else if(cameraY < floorHeight * 1.5f) currentFloor = 1;
    else if(cameraY < floorHeight * 2.5f) currentFloor = 2;
    else currentFloor = 3;
}

bool Showroom::isNearCar(float camX, float camZ, float& carX, float& carY, float& carZ, float& carAngle) {
    if(currentFloor == 0 && mainCar && mainCar->isNear(camX, camZ)) {
        mainCar->getPosition(carX, carY, carZ, carAngle);
        return true;
    }
    return false;
}

void Showroom::interactWithCar() {
    if(mainCar && mainCar->isNear(cameraX, cameraZ)) {
        mainCar->toggleDoor();
    }
}

void Showroom::drawCarLabel(float x, float y, float z, const char* name, const char* price) {
    // Labels removed - too distracting
}

void Showroom::toggleCarWheels() {
    if(mainCar) {
        mainCar->toggleWheels();
    }
}

void Showroom::enterCar() {
    if(currentFloor == 0 && mainCar && mainCar->isNear(cameraX, cameraZ)) {
        inDriverSeat = !inDriverSeat;
        mciSendStringA("close entercar", NULL, 0, NULL);
        mciSendStringA("open sounds\\open-car-door.wav type mpegvideo alias entercar", NULL, 0, NULL);
        mciSendStringA("play entercar", NULL, 0, NULL);
        
        if(inDriverSeat) {
            mainCar->getPosition(driverCarX, driverCarY, driverCarZ, driverCarAngle);
        }
    }
}

void Showroom::toggleEngine() {
    if(inDriverSeat && mainCar) {
        mainCar->toggleEngine();
    }
}
