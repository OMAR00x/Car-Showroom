#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <windows.h>
#include <mmsystem.h>
#include <cmath>

class DetailedCar {
private:
    float x, y, z;
    float angle;
    float doorAngle;
    bool doorOpen;
    bool engineOn;
    float wheelRotation;
    bool wheelsSpinning;

    void drawBody() {
        glColor3f(0.9f, 0.1f, 0.1f);

        // Lower body - hollow (no side walls where doors are)
        // Front
        glPushMatrix();
        glTranslatef(1.65f, 0.5f, 0);
        glScalef(0.2f, 0.6f, 2.1f);
        glutSolidCube(1);
        glPopMatrix();

        // Back
        glPushMatrix();
        glTranslatef(-1.65f, 0.5f, 0);
        glScalef(0.2f, 0.6f, 2.1f);
        glutSolidCube(1);
        glPopMatrix();

        // Floor
        glPushMatrix();
        glTranslatef(0, 0.22f, 0);
        glScalef(3.3f, 0.04f, 2.3f);
        glutSolidCube(1);
        glPopMatrix();

        // Hood
        glPushMatrix();
        glTranslatef(1.3f, 0.55f, 0);
        glScalef(1.0f, 0.4f, 2.3f);
        glutSolidCube(1);
        glPopMatrix();

        // Trunk
        glPushMatrix();
        glTranslatef(-1.3f, 0.55f, 0);
        glScalef(1.0f, 0.4f, 2.3f);
        glutSolidCube(1);
        glPopMatrix();

        // Bumpers
        glColor3f(0.2f, 0.2f, 0.2f);
        glPushMatrix();
        glTranslatef(1.85f, 0.3f, 0);
        glScalef(0.15f, 0.4f, 2.5f);
        glutSolidCube(1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-1.85f, 0.3f, 0);
        glScalef(0.15f, 0.4f, 2.5f);
        glutSolidCube(1);
        glPopMatrix();

        // Headlights
        glColor3f(1.0f, 1.0f, 0.9f);
        glPushMatrix();
        glTranslatef(1.88f, 0.45f, 0.5f);
        glutSolidSphere(0.12f, 10, 10);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(1.88f, 0.45f, -0.5f);
        glutSolidSphere(0.12f, 10, 10);
        glPopMatrix();

        // Taillights
        glColor3f(0.9f, 0.1f, 0.1f);
        glPushMatrix();
        glTranslatef(-1.88f, 0.45f, 0.6f);
        glutSolidSphere(0.1f, 8, 8);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-1.88f, 0.45f, -0.6f);
        glutSolidSphere(0.1f, 8, 8);
        glPopMatrix();
    }

    void drawWheels() {
        float wheelPos[4][2] = {{1.2f, 1.2f}, {1.2f, -1.2f}, {-1.2f, 1.2f}, {-1.2f, -1.2f}};
        for (int i = 0; i < 4; i++) {
            glPushMatrix();
            glTranslatef(wheelPos[i][0], -0.25f, wheelPos[i][1]);
            if (wheelsSpinning) {
                glRotatef(wheelRotation, 0, 0, 1);
            }

            // Tire
            glColor3f(0.1f, 0.1f, 0.1f);
            glutSolidTorus(0.15, 0.4, 12, 16);

            // Rim
            glColor3f(0.7f, 0.7f, 0.75f);
            glutSolidTorus(0.06, 0.22, 8, 12);

            // Spokes
            for (int j = 0; j < 5; j++) {
                glPushMatrix();
                glRotatef(j * 72, 0, 0, 1);
                glPushMatrix();
                glTranslatef(0.0f, 0.0f, -0.08f);
                glScalef(0.04f, 0.25f, 0.15f);
                glutSolidCube(1);
                glPopMatrix();
                glPopMatrix();
            }

            glPopMatrix();
        }
    }

    void drawInterior() {
        // Dashboard
        glColor3f(0.15f, 0.15f, 0.15f);
        glPushMatrix();
        glTranslatef(1.2f, 0.5f, 0);
        glScalef(0.3f, 0.3f, 1.6f);
        glutSolidCube(1);
        glPopMatrix();

        // Steering wheel - أكبر وأوضح
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
        glTranslatef(0.85f, 0.6f, -0.4f);
        glRotatef(90, 0, 1, 0);
        glRotatef(-20, 1, 0, 0);
        glutSolidTorus(0.06, 0.22, 12, 20);
        // Center of steering wheel
        glColor3f(0.2f, 0.2f, 0.2f);
        glutSolidSphere(0.08f, 10, 10);
        glPopMatrix();

        // Dashboard buttons
        glColor3f(0.8f, 0.2f, 0.2f);
        glPushMatrix();
        glTranslatef(1.15f, 0.55f, -0.2f);
        glutSolidSphere(0.03f, 8, 8);
        glPopMatrix();

        glColor3f(0.2f, 0.8f, 0.2f);
        glPushMatrix();
        glTranslatef(1.15f, 0.55f, 0.0f);
        glutSolidSphere(0.03f, 8, 8);
        glPopMatrix();

        glColor3f(0.2f, 0.2f, 0.8f);
        glPushMatrix();
        glTranslatef(1.15f, 0.55f, 0.2f);
        glutSolidSphere(0.03f, 8, 8);
        glPopMatrix();

        // Gear shift
        glColor3f(0.15f, 0.15f, 0.15f);
        glPushMatrix();
        glTranslatef(0.3f, 0.35f, 0);
        glScalef(0.08f, 0.25f, 0.08f);
        glutSolidCube(1);
        glPopMatrix();
        
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
        glTranslatef(0.3f, 0.5f, 0);
        glutSolidSphere(0.06f, 8, 8);
        glPopMatrix();

        // Center console
        glColor3f(0.15f, 0.15f, 0.15f);
        glPushMatrix();
        glTranslatef(0.5f, 0.3f, 0);
        glScalef(0.8f, 0.15f, 0.4f);
        glutSolidCube(1);
        glPopMatrix();

        // Driver seat base
        glColor3f(0.2f, 0.2f, 0.2f);
        glPushMatrix();
        glTranslatef(0.0f, 0.35f, -0.4f);
        glScalef(0.7f, 0.3f, 0.6f);
        glutSolidCube(1);
        glPopMatrix();

        // Driver seat back
        glPushMatrix();
        glTranslatef(-0.3f, 0.65f, -0.4f);
        glScalef(0.1f, 0.5f, 0.6f);
        glutSolidCube(1);
        glPopMatrix();

        // Passenger seat base
        glPushMatrix();
        glTranslatef(0.0f, 0.35f, 0.4f);
        glScalef(0.7f, 0.3f, 0.6f);
        glutSolidCube(1);
        glPopMatrix();

        // Passenger seat back
        glPushMatrix();
        glTranslatef(-0.3f, 0.65f, 0.4f);
        glScalef(0.1f, 0.5f, 0.6f);
        glutSolidCube(1);
        glPopMatrix();
    }

    void drawLeftDoor() {
        const float hingeX = 0.6f;
        const float hingeY = 0.5f;
        const float hingeZ = -1.15f;
        const float doorLength = 1.5f;
        const float doorHeight = 0.7f;
        const float doorThick = 0.08f;

        glPushMatrix();
        glTranslatef(hingeX, hingeY, hingeZ);
        glRotatef(-doorAngle, 0, 1, 0);

        glPushMatrix();
        glTranslatef(-doorLength/2.0f, 0.0f, -doorThick / 2.0f);
        glColor3f(0.9f, 0.1f, 0.1f);
        glScalef(doorLength, doorHeight, doorThick);
        glutSolidCube(1.0);
        glPopMatrix();

        // Door handle
        glPushMatrix();
        glTranslatef(-0.5f, 0.0f, -doorThick * 4.0f);
        glColor3f(0.3f, 0.3f, 0.3f);
        glScalef(0.18f, 0.05f, 0.15f);
        glutSolidCube(1.0);
        glPopMatrix();

        // Side mirror
        glPushMatrix();
        glTranslatef(0.6f, 0.2f, -doorThick * 5.0f);
        glColor3f(0.1f, 0.1f, 0.1f);
        glScalef(0.1f, 0.15f, 0.2f);
        glutSolidCube(1.0);
        glPopMatrix();

        glPopMatrix();
    }

    void drawRightDoor() {
        const float hingeX = 0.6f;
        const float hingeY = 0.5f;
        const float hingeZ = 1.15f;
        const float doorLength = 1.5f;
        const float doorHeight = 0.7f;
        const float doorThick = 0.08f;

        glPushMatrix();
        glTranslatef(hingeX, hingeY, hingeZ);
        glRotatef(doorAngle, 0, 1, 0);

        glPushMatrix();
        glTranslatef(-doorLength/2.0f, 0.0f, doorThick / 2.0f);
        glColor3f(0.9f, 0.1f, 0.1f);
        glScalef(doorLength, doorHeight, doorThick);
        glutSolidCube(1.0);
        glPopMatrix();

        // Door handle
        glPushMatrix();
        glTranslatef(-0.5f, 0.0f, doorThick * 4.0f);
        glColor3f(0.3f, 0.3f, 0.3f);
        glScalef(0.18f, 0.05f, 0.15f);
        glutSolidCube(1.0);
        glPopMatrix();

        // Side mirror
        glPushMatrix();
        glTranslatef(0.6f, 0.2f, doorThick * 5.0f);
        glColor3f(0.1f, 0.1f, 0.1f);
        glScalef(0.1f, 0.15f, 0.2f);
        glutSolidCube(1.0);
        glPopMatrix();

        glPopMatrix();
    }


public:
    DetailedCar(float px, float py, float pz, float pangle) {
        x = px; y = py; z = pz; angle = pangle;
        doorAngle = 0;
        doorOpen = false;
        engineOn = false;
        wheelRotation = 0;
        wheelsSpinning = false;
    }

    void draw() {
        glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(angle, 0, 1, 0);
        glScalef(1.5f, 1.5f, 1.5f);

        drawBody();
        drawWheels();
        drawInterior();
        // -- تعديل: استدعاء دوال رسم الأبواب الجديدة --
        drawLeftDoor();
        drawRightDoor();

        glPopMatrix();
    }

    void toggleDoor() {
        doorOpen = !doorOpen;
        if (doorOpen) {
            mciSendStringA("close cardoor", NULL, 0, NULL);
            mciSendStringA("open \"sounds\\open-car-door.wav\" type waveaudio alias cardoor", NULL, 0, NULL);
            mciSendStringA("play cardoor", NULL, 0, NULL);
        } else {
            // Optional: Add a closing sound
        }
    }

    void toggleWheels() {
        wheelsSpinning = !wheelsSpinning;
    }

    void toggleEngine() {
        mciSendStringA("close carengine", NULL, 0, NULL);
        mciSendStringA("open sounds\\car-engine.wav type mpegvideo alias carengine", NULL, 0, NULL);
        mciSendStringA("play carengine", NULL, 0, NULL);
    }

    void update() {
        if (doorOpen && doorAngle < 70) {
            doorAngle += 2.0f;
        } else if (!doorOpen && doorAngle > 0) {
            doorAngle -= 2.0f;
        }
        if (wheelsSpinning) {
            wheelRotation += 10.0f; // Increased speed for better visuals
            if (wheelRotation > 360) wheelRotation -= 360;
        }
    }

    bool isNear(float camX, float camZ) {
        float dx = camX - x;
        float dz = camZ - z;
        return (dx * dx + dz * dz) < 100.0f; // Increased radius
    }

    void getPosition(float& px, float& py, float& pz, float& pangle) {
        px = x; py = y; pz = z; pangle = angle;
    }

    void getDriverSeatPosition(float& px, float& py, float& pz, float& pangle) {
        float scale = 1.5f;
        float seatX = 0.0f * scale;
        float seatY = 0.6f * scale;
        float seatZ = -0.5f * scale;
        
        float rad = angle * 3.14159f / 180.0f;
        float cosA = cos(rad);
        float sinA = sin(rad);
        
        px = x + (seatX * cosA - seatZ * sinA);
        py = y + seatY;
        pz = z + (seatX * sinA + seatZ * cosA);
        pangle = angle;
    }
};
