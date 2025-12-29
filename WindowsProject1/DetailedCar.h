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
        // --- تعديل: تم تعديل أبعاد الهيكل ليكون أكثر واقعية ---
        glColor3f(0.4f, 0.7f, 1.0f); // Sky blue color for the body

        // Lower body frame
        // Floor
        glPushMatrix();
        glTranslatef(0, 0.22f, 0);
        glScalef(3.5f, 0.04f, 2.1f);
        glutSolidCube(1);
        glPopMatrix();
        // Front wall (under hood)
        glPushMatrix();
        glTranslatef(1.7f, 0.5f, 0);
        glScalef(0.1f, 0.6f, 1.9f);
        glutSolidCube(1);
        glPopMatrix();
        // Back wall (under trunk)
        glPushMatrix();
        glTranslatef(-1.7f, 0.5f, 0);
        glScalef(0.1f, 0.6f, 1.9f);
        glutSolidCube(1);
        glPopMatrix();

        // Hood
        glPushMatrix();
        glTranslatef(1.25f, 0.7f, 0);
        glScalef(0.9f, 0.2f, 1.9f);
        glutSolidCube(1);
        glPopMatrix();

        // Trunk
        glPushMatrix();
        glTranslatef(-1.25f, 0.7f, 0);
        glScalef(0.9f, 0.2f, 1.9f);
        glutSolidCube(1);
        glPopMatrix();

        // Bumpers
        glColor3f(0.2f, 0.2f, 0.2f);
        glPushMatrix();
        glTranslatef(1.8f, 0.4f, 0);
        glScalef(0.1f, 0.3f, 2.2f);
        glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-1.8f, 0.4f, 0);
        glScalef(0.1f, 0.3f, 2.2f);
        glutSolidCube(1);
        glPopMatrix();

        // Headlights
        glColor3f(1.0f, 1.0f, 0.9f);
        glPushMatrix();
        glTranslatef(1.78f, 0.5f, 0.7f);
        glutSolidSphere(0.15f, 10, 10);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(1.78f, 0.5f, -0.7f);
        glutSolidSphere(0.15f, 10, 10);
        glPopMatrix();

        // Taillights
        glColor3f(0.8f, 0.0f, 0.0f);
        glPushMatrix();
        glTranslatef(-1.78f, 0.5f, 0.7f);
        glScalef(0.05f, 0.15f, 0.3f);
        glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-1.78f, 0.5f, -0.7f);
        glScalef(0.05f, 0.15f, 0.3f);
        glutSolidCube(1);
        glPopMatrix();

        // --- إضافة: سقف وأعمدة للسيارة لزيادة الواقعية ---
        // Roof
        glColor3f(0.4f, 0.7f, 1.0f);
        glPushMatrix();
        glTranslatef(0, 1.4f, 0);
        glScalef(2.0f, 0.05f, 2.0f);
        glutSolidCube(1);
        glPopMatrix();

        // Pillars
        glPushMatrix();
        glTranslatef(0.95f, 1.1f, 0.98f);
        glRotatef(20, 0, 0, 1);
        glScalef(0.1f, 0.7f, 0.1f);
        glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.95f, 1.1f, -0.98f);
        glRotatef(20, 0, 0, 1);
        glScalef(0.1f, 0.7f, 0.1f);
        glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.95f, 1.1f, 0.98f);
        glRotatef(-20, 0, 0, 1);
        glScalef(0.1f, 0.7f, 0.1f);
        glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.95f, 1.1f, -0.98f);
        glRotatef(-20, 0, 0, 1);
        glScalef(0.1f, 0.7f, 0.1f);
        glutSolidCube(1);
        glPopMatrix();

        // --- إضافة: زجاج أمامي وخلفي شفاف ---
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.6f, 0.8f, 0.9f, 0.4f);
        // Windshield
        glPushMatrix();
        glTranslatef(1.0f, 1.12f, 0);
        glRotatef(-70, 0, 1, 0);
        glRotatef(-90, 1, 0, 0);
        glScalef(1.95f, 0.05f, 0.7f);
        glutSolidCube(1);
        glPopMatrix();
        // Rear Window
        glPushMatrix();
        glTranslatef(-1.0f, 1.12f, 0);
        glRotatef(70, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        glScalef(1.95f, 0.05f, 0.7f);
        glutSolidCube(1);
        glPopMatrix();
        glDisable(GL_BLEND);
    }

    void drawWheels() {
        float wheelPos[4][2] = {{1.2f, 1.1f}, {1.2f, -1.1f}, {-1.2f, 1.1f}, {-1.2f, -1.1f}};
        for (int i = 0; i < 4; i++) {
            glPushMatrix();
            glTranslatef(wheelPos[i][0], 0.25f, wheelPos[i][1]);
            if (i==0 || i==2) glRotatef(180, 0,1,0); // Flip wheels on one side to have spokes out
            if (wheelsSpinning) {
                glRotatef(wheelRotation, 0, 0, 1);
            }
             // Tire
            glColor3f(0.1f, 0.1f, 0.1f);
            glutSolidTorus(0.1, 0.3, 15, 20);
            // Rim
            glColor3f(0.7f, 0.7f, 0.75f);
            glPushMatrix();
            GLUquadric* quad = gluNewQuadric();
            gluCylinder(quad, 0.2, 0.2, 0.15, 15, 1);
            gluDeleteQuadric(quad);
            glPopMatrix();
            // Spokes
            for (int j = 0; j < 5; j++) {
                glPushMatrix();
                glRotatef(j * 72, 0, 0, 1);
                glTranslatef(0.1f, 0.0f, 0.08f);
                glScalef(0.2f, 0.04f, 0.02f);
                glutSolidCube(1);
                glPopMatrix();
            }
            glPopMatrix();
        }
    }

    void drawInterior() {
        // Dashboard
        glColor3f(0.15f, 0.15f, 0.15f);
        glPushMatrix();
        glTranslatef(0.8f, 0.8f, 0);
        glScalef(0.1f, 0.2f, 1.9f);
        glutSolidCube(1);
        glPopMatrix();
        
        // Steering wheel
        glColor3f(0.05f, 0.05f, 0.05f);
        glPushMatrix();
        glTranslatef(0.5f, 0.75f, -0.4f);
        glRotatef(60, 1, 0, 0);
        glutSolidTorus(0.04, 0.18, 12, 20);
        // Steering column
        glPushMatrix();
        glRotatef(-60, 1, 0, 0);
        glTranslatef(0,0,-0.15);
        glColor3f(0.1f, 0.1f, 0.1f);
        GLUquadric* quad2 = gluNewQuadric();
        gluCylinder(quad2, 0.03, 0.03, 0.3, 8, 1);
        gluDeleteQuadric(quad2);
        glPopMatrix();
        glPopMatrix();

        // --- إضافة: تفاصيل واقعية داخل السيارة ---
        // Rear-view mirror
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
        glTranslatef(0.7f, 1.35f, 0);
        glScalef(0.3f, 0.15f, 0.05f);
        glutSolidCube(1.0);
        glPopMatrix();

        // Pedals
        glPushMatrix();
        glColor3f(0.1f, 0.1f, 0.1f);
        glTranslatef(0.75f, 0.3f, -0.5f); // Gas
        glScalef(0.1f, 0.2f, 0.05f);
        glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.75f, 0.3f, -0.65f); // Brake
        glScalef(0.1f, 0.2f, 0.05f);
        glutSolidCube(1);
        glPopMatrix();

        // Gear shift
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
        glTranslatef(0.2f, 0.45f, 0);
        glScalef(0.06f, 0.35f, 0.06f);
        glutSolidCube(1);
        glPopMatrix();
        glColor3f(0.8f, 0.8f, 0.8f);
        glPushMatrix();
        glTranslatef(0.2f, 0.65f, 0);
        glutSolidSphere(0.06f, 8, 8);
        glPopMatrix();

        // --- تعديل: مقاعد بتصميم أفضل مع مساند للرأس ---
        // Seats
        glColor3f(0.25f, 0.25f, 0.25f);
        // Driver Seat
        glPushMatrix();
        glTranslatef(0.3f, 0.4f, -0.5f);
        // Base
        glPushMatrix();
        glScalef(0.7f, 0.25f, 0.6f);
        glutSolidCube(1);
        glPopMatrix();
        // Back
        glPushMatrix();
        glTranslatef(-0.3f, 0.4f, 0f);
        glRotatef(15, 0, 0, 1);
        glScalef(0.15f, 0.7f, 0.6f);
        glutSolidCube(1);
        glPopMatrix();
        // Headrest
        glPushMatrix();
        glTranslatef(-0.4f, 0.9f, 0f);
        glScalef(0.3f, 0.2f, 0.1f);
        glutSolidCube(1);
        glPopMatrix();
        glPopMatrix();

        // Passenger Seat
        glPushMatrix();
        glTranslatef(0.3f, 0.4f, 0.5f);
        // Base
        glPushMatrix();
        glScalef(0.7f, 0.25f, 0.6f);
        glutSolidCube(1);
        glPopMatrix();
        // Back
        glPushMatrix();
        glTranslatef(-0.3f, 0.4f, 0f);
        glRotatef(15, 0, 0, 1);
        glScalef(0.15f, 0.7f, 0.6f);
        glutSolidCube(1);
        glPopMatrix();
        // Headrest
        glPushMatrix();
        glTranslatef(-0.4f, 0.9f, 0f);
        glScalef(0.3f, 0.2f, 0.1f);
        glutSolidCube(1);
        glPopMatrix();
        glPopMatrix();
    }

    // --- تعديل جذري: تم إصلاح الأبواب بالكامل لتتحرك بشكل صحيح وتتضمن المرايا والزجاج ---
    void drawLeftDoor() {
        const float doorLength = 1.9f;
        const float doorHeight = 1.0f;
        const float doorThick = 0.08f;
        
        // نقطة دوران الباب (المفصلة)
        const float hingeX = 0.9f;
        const float hingeY = 0.3f;
        const float hingeZ = -1.05f;

        glPushMatrix();
        // 1. الانتقال إلى مكان المفصلة
        glTranslatef(hingeX, hingeY, hingeZ);
        // 2. تدوير الباب حول المفصلة
        glRotatef(-doorAngle, 0, 1, 0);

        // 3. رسم جسم الباب الأساسي
        glPushMatrix();
        glTranslatef(-doorLength / 2.0f, doorHeight / 2.0f, 0.0f);
        glScalef(doorLength, doorHeight, doorThick);
        glColor3f(0.4f, 0.7f, 1.0f);
        glutSolidCube(1.0);
        glPopMatrix();

        // 4. رسم مقبض الباب
        glPushMatrix();
        glTranslatef(-doorLength + 0.2f, doorHeight / 2.0f, doorThick / 2.0f);
        glScalef(0.2f, 0.06f, 0.1f);
        glColor3f(0.1f, 0.1f, 0.1f);
        glutSolidCube(1.0);
        glPopMatrix();

        // 5. رسم المرآة الجانبية (أصبحت الآن جزءاً من الباب)
        glPushMatrix();
        glTranslatef(0.0f, doorHeight - 0.1f, -doorThick / 2);
        glRotatef(30, 0, 1, 0);
        // Mirror arm
        glScalef(0.1f, 0.05f, 0.2f);
        glColor3f(0.1f, 0.1f, 0.1f);
        glutSolidCube(1.0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.05f, doorHeight - 0.1f, -doorThick - 0.1f);
        glRotatef(30, 0, 1, 0);
        // Mirror glass
        glScalef(0.2f, 0.15f, 0.05f);
        glColor3f(0.7f, 0.8f, 0.8f);
        glutSolidCube(1.0);
        glPopMatrix();

        // 6. رسم زجاج الباب (شبه شفاف)
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
        glTranslatef(-doorLength / 2.0f, doorHeight * 0.75f , 0.0f);
        glScalef(doorLength - 0.1f, doorHeight * 0.4f, doorThick * 0.7f);
        glColor4f(0.6f, 0.8f, 0.9f, 0.4f);
        glutSolidCube(1.0);
        glPopMatrix();
        glDisable(GL_BLEND);

        glPopMatrix();
    }

    void drawRightDoor() {
        const float doorLength = 1.9f;
        const float doorHeight = 1.0f;
        const float doorThick = 0.08f;
        
        const float hingeX = 0.9f;
        const float hingeY = 0.3f;
        const float hingeZ = 1.05f;

        glPushMatrix();
        glTranslatef(hingeX, hingeY, hingeZ);
        glRotatef(doorAngle, 0, 1, 0);

        glPushMatrix();
        glTranslatef(-doorLength / 2.0f, doorHeight / 2.0f, 0.0f);
        glScalef(doorLength, doorHeight, doorThick);
        glColor3f(0.4f, 0.7f, 1.0f);
        glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-doorLength + 0.2f, doorHeight / 2.0f, -doorThick / 2.0f);
        glScalef(0.2f, 0.06f, 0.1f);
        glColor3f(0.1f, 0.1f, 0.1f);
        glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, doorHeight - 0.1f, doorThick / 2);
        glRotatef(-30, 0, 1, 0);
        glScalef(0.1f, 0.05f, 0.2f);
        glColor3f(0.1f, 0.1f, 0.1f);
        glutSolidCube(1.0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.05f, doorHeight - 0.1f, doorThick + 0.1f);
        glRotatef(-30, 0, 1, 0);
        glScalef(0.2f, 0.15f, 0.05f);
        glColor3f(0.7f, 0.8f, 0.8f);
        glutSolidCube(1.0);
        glPopMatrix();
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
        glTranslatef(-doorLength / 2.0f, doorHeight * 0.75f, 0.0f);
        glScalef(doorLength - 0.1f, doorHeight * 0.4f, doorThick * 0.7f);
        glColor4f(0.6f, 0.8f, 0.9f, 0.4f);
        glutSolidCube(1.0);
        glPopMatrix();
        glDisable(GL_BLEND);

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
        if (doorOpen && doorAngle < 80) { // Increased angle
            doorAngle += 2.0f;
        } else if (!doorOpen && doorAngle > 0) {
            doorAngle -= 2.0f;
        }

        if (wheelsSpinning) {
            wheelRotation += 10.0f;
            if (wheelRotation > 360) wheelRotation -= 360;
        }
    }

    bool isNear(float camX, float camZ) {
        float dx = camX - x;
        float dz = camZ - z;
        return (dx * dx + dz * dz) < 100.0f;
    }

    void getPosition(float& px, float& py, float& pz, float& pangle) {
        px = x; py = y; pz = z; pangle = angle;
    }
    
    void getDriverSeatPosition(float& px, float& py, float& pz, float& pangle) {
        float scale = 1.5f;
        float seatX = 0.3f * scale;
        float seatY = 0.8f * scale;
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
