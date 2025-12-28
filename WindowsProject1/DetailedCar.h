#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <windows.h>
#include <mmsystem.h>

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
        
        // Lower body
        glPushMatrix();
        glTranslatef(0, 0.5f, 0);
        glScalef(3.5f, 0.6f, 1.8f);
        glutSolidCube(1);
        glPopMatrix();
        
        // Hood
        glPushMatrix();
        glTranslatef(1.3f, 0.55f, 0);
        glScalef(1.0f, 0.4f, 1.7f);
        glutSolidCube(1);
        glPopMatrix();
        
        // Trunk
        glPushMatrix();
        glTranslatef(-1.3f, 0.55f, 0);
        glScalef(1.0f, 0.4f, 1.7f);
        glutSolidCube(1);
        glPopMatrix();
        
        // Bumpers
        glColor3f(0.2f, 0.2f, 0.2f);
        glPushMatrix();
        glTranslatef(1.85f, 0.3f, 0);
        glScalef(0.15f, 0.4f, 1.9f);
        glutSolidCube(1);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-1.85f, 0.3f, 0);
        glScalef(0.15f, 0.4f, 1.9f);
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
        float wheelPos[4][2] = {{1.2f, 0.95f}, {1.2f, -0.95f}, {-1.2f, 0.95f}, {-1.2f, -0.95f}};
        for(int i = 0; i < 4; i++) {
            glPushMatrix();
            glTranslatef(wheelPos[i][0], 0.0f, wheelPos[i][1]);
            glRotatef(90, 0, 0, 1);
            if(wheelsSpinning) {
                glRotatef(wheelRotation, 0, 0, 1);
            }
            
            // Tire
            glColor3f(0.1f, 0.1f, 0.1f);
            glutSolidTorus(0.15, 0.4, 12, 16);
            
            // Rim
            glColor3f(0.7f, 0.7f, 0.75f);
            glutSolidTorus(0.06, 0.22, 8, 12);
            
            // Spokes
            for(int j = 0; j < 5; j++) {
                glPushMatrix();
                glRotatef(j * 72, 0, 0, 1);
                glTranslatef(0, 0.12f, 0);
                glScalef(0.04f, 0.15f, 0.04f);
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
        glTranslatef(1.0f, 0.9f, 0);
        glScalef(0.3f, 0.3f, 1.6f);
        glutSolidCube(1);
        glPopMatrix();
        
        // Steering wheel
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
        glTranslatef(0.75f, 0.95f, -0.35f);
        glRotatef(90, 0, 1, 0);
        glRotatef(-15, 1, 0, 0);
        glutSolidTorus(0.04, 0.18, 10, 16);
        glPopMatrix();
        
        // Driver seat
        glColor3f(0.2f, 0.2f, 0.2f);
        glPushMatrix();
        glTranslatef(-0.1f, 0.65f, -0.4f);
        glScalef(0.6f, 0.3f, 0.6f);
        glutSolidCube(1);
        glPopMatrix();
        
        // Seat back
        glPushMatrix();
        glTranslatef(-0.35f, 0.95f, -0.4f);
        glScalef(0.1f, 0.5f, 0.6f);
        glutSolidCube(1);
        glPopMatrix();
        
        // Passenger seat
        glPushMatrix();
        glTranslatef(-0.1f, 0.65f, 0.4f);
        glScalef(0.6f, 0.3f, 0.6f);
        glutSolidCube(1);
        glPopMatrix();
        
        // Passenger seat back
        glPushMatrix();
        glTranslatef(-0.35f, 0.95f, 0.4f);
        glScalef(0.1f, 0.5f, 0.6f);
        glutSolidCube(1);
        glPopMatrix();
    }
    
    void drawDoor() {
        glPushMatrix();
        glTranslatef(-0.3f, 0.7f, -0.95f);
        glRotatef(-doorAngle, 0, 1, 0);
        glTranslatef(0, 0, -0.3f);
        
        glColor3f(0.9f, 0.1f, 0.1f);
        glPushMatrix();
        glScalef(1.2f, 0.9f, 0.1f);
        glutSolidCube(1);
        glPopMatrix();
        
        // Handle
        glColor3f(0.6f, 0.6f, 0.65f);
        glPushMatrix();
        glTranslatef(0.4f, -0.1f, -0.08f);
        glScalef(0.15f, 0.06f, 0.04f);
        glutSolidCube(1);
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
        glScalef(1.2f, 1.2f, 1.2f);
        
        drawBody();
        drawWheels();
        drawInterior();
        drawDoor();
        
        glPopMatrix();
    }
    
    void toggleDoor() {
        doorOpen = !doorOpen;
        mciSendStringA("close cardoor", NULL, 0, NULL);
        mciSendStringA("open \"sounds\\open-car-door.wav\" type waveaudio alias cardoor", NULL, 0, NULL);
        mciSendStringA("play cardoor", NULL, 0, NULL);
    }
    
    void toggleWheels() {
        wheelsSpinning = !wheelsSpinning;
    }
    
    void update() {
        if(doorOpen && doorAngle < 70) {
            doorAngle += 2.0f;
        } else if(!doorOpen && doorAngle > 0) {
            doorAngle -= 2.0f;
        }
        if(wheelsSpinning) {
            wheelRotation += 5.0f;
            if(wheelRotation > 360) wheelRotation -= 360;
        }
    }
    
    bool isNear(float camX, float camZ) {
        float dx = camX - x;
        float dz = camZ - z;
        return (dx*dx + dz*dz) < 64.0f;
    }
    
    void getPosition(float& px, float& py, float& pz, float& pangle) {
        px = x; py = y; pz = z; pangle = angle;
    }
};
