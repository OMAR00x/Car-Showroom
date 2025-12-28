#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <windows.h>
#include <mmsystem.h>
#include "Showroom.h"

#define M_PI 3.14159265358979323846
enum CameraMode { FREE_ROAM, ORBIT, DRIVER_SEAT };
class Camera {
public:
    CameraMode mode = FREE_ROAM;
    float x, y, z;
    float lx, ly, lz;
    float angleYaw, anglePitch;
    float moveSpeed;
    float mouseSensitivity;
    float minX, maxX, minZ, maxZ;
    Showroom* showroom;
    bool wasInDriverSeat;

    void applyView(float carX, float carY, float carZ, float carAngle) {
        if (mode == FREE_ROAM) {
            gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0, 1, 0);
        }
        else if (mode == DRIVER_SEAT) {
            // �������� ���� �������
            float rad = carAngle * M_PI / 180.0f;
            float ox = sin(rad) * 0.5f; // ����� ����� ����� ������
            float oz = cos(rad) * 0.5f;
            gluLookAt(carX + ox, carY + 1.2f, carZ + oz, carX + sin(rad) * 10, carY + 1.2f, carZ + cos(rad) * 10, 0, 1, 0);
        }
    }


    Camera() {
        x = 0.0f;
        y = 2.5f;
        z = 10.0f;
        
        angleYaw = -90.0f;
        anglePitch = -10.0f;
        updateVectors();
        moveSpeed = 0.2f;
        mouseSensitivity = 0.1f;
        
        minX = -500.0f; maxX = 500.0f;
        minZ = -500.0f; maxZ = 500.0f;
        showroom = nullptr;
        wasInDriverSeat = false;
    }
    
    void updateVectors() {
        lx = cos(angleYaw * M_PI / 180.0) * cos(anglePitch * M_PI / 180.0);
        ly = sin(anglePitch * M_PI / 180.0);
        lz = sin(angleYaw * M_PI / 180.0) * cos(anglePitch * M_PI / 180.0);
    }
    
    void applyView() {
        gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
    }
    
    void processKeys(const bool* keyStates) {
        float strafe_lx = cos((angleYaw + 90.0f) * M_PI / 180.0f);
        float strafe_lz = sin((angleYaw + 90.0f) * M_PI / 180.0f);
        
        float newX = x;
        float newZ = z;
        float newY = y;
        
        bool moving = false;
        
        if (keyStates['w'] || keyStates['W']) {
            newX += lx * moveSpeed;
            newZ += lz * moveSpeed;
            moving = true;
        }
        if (keyStates['s'] || keyStates['S']) {
            newX -= lx * moveSpeed;
            newZ -= lz * moveSpeed;
            moving = true;
        }
        if (keyStates['a'] || keyStates['A']) {
            newX -= strafe_lx * moveSpeed;
            newZ -= strafe_lz * moveSpeed;
            moving = true;
        }
        if (keyStates['d'] || keyStates['D']) {
            newX += strafe_lx * moveSpeed;
            newZ += strafe_lz * moveSpeed;
            moving = true;
        }
        
        static int stepCount = 0;
        if(moving) {
            stepCount++;
            if(stepCount >= 30) {
                PlaySoundA("sounds\\footstep.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
                stepCount = 0;
            }
        } else {
            stepCount = 0;
        }
        
        // Check if inside elevator FIRST
        if(showroom) {
            float sx = 30.0f;
            float sz = 35.0f;
            if(x > sx - 1.8f && x < sx + 1.8f && z > sz - 3.8f && z < sz + 0.2f) {
                newY = showroom->elevatorY + 3.0f;
            }
        }
        
        // Collision detection
        bool collisionX = false;
        bool collisionZ = false;
        
        if(showroom) {
            float hw = 40.2f;
            float hd = 50.2f;
            
            // Check if inside or trying to enter
            bool inside = (x > -hw && x < hw && z > -hd && z < hd);
            bool willBeInside = (newX > -hw && newX < hw && newZ > -hd && newZ < hd);
            
            if(inside) {
                // Inside - block walls
                if(newX < -hw + 1.0f) collisionX = true;
                if(newX > hw - 1.0f) collisionX = true;
                if(newZ > hd - 1.0f) collisionZ = true;
                
                // Front wall - check door
                if(newZ < -hd + 1.0f) {
                    bool inDoorArea = (newX > -5 && newX < 5);
                    bool doorOpen = (showroom->doorOpen > 4.5f);
                    if(!inDoorArea || !doorOpen) {
                        collisionZ = true;
                    }
                }
                
                // Elevator - block 3 sides, allow entry from front
                float sx = 30.0f;
                float sz = 35.0f;
                
                // Block left side
                if(newX > sx - 2.5f && newX < sx - 1.5f && newZ > sz - 4.5f && newZ < sz + 0.5f) {
                    collisionX = true;
                }
                // Block right side
                if(newX > sx + 1.5f && newX < sx + 2.5f && newZ > sz - 4.5f && newZ < sz + 0.5f) {
                    collisionX = true;
                }
                // Block back side
                if(newX > sx - 2.5f && newX < sx + 2.5f && newZ > sz - 0.5f && newZ < sz + 0.5f) {
                    collisionZ = true;
                }
                
                // Cars
                float carPositions[][2] = {{-15, 22}, {20, 22}, {0, 40}};
                for(int i = 0; i < 3; i++) {
                    float dx = newX - carPositions[i][0];
                    float dz = newZ - carPositions[i][1];
                    if(dx*dx + dz*dz < 16.0f) {
                        collisionX = true;
                        collisionZ = true;
                    }
                }
            } else if(!inside && willBeInside) {
                // Trying to enter from outside - only through door
                bool inDoorArea = (newX > -5 && newX < 5 && newZ > -hd - 2 && newZ < -hd + 1);
                bool doorOpen = (showroom->doorOpen > 4.5f);
                if(!inDoorArea || !doorOpen) {
                    collisionX = true;
                    collisionZ = true;
                }
            }
            
            // Surrounding buildings
            if(newX > -150 && newX < -120 && newZ > -100 && newZ < -75) {
                collisionX = true; collisionZ = true;
            }
            if(newX > 120 && newX < 155 && newZ > -120 && newZ < -90) {
                collisionX = true; collisionZ = true;
            }
            if(newX > -140 && newX < -115 && newZ > 150 && newZ < 170) {
                collisionX = true; collisionZ = true;
            }
            if(newX > 130 && newX < 170 && newZ > 140 && newZ < 175) {
                collisionX = true; collisionZ = true;
            }
            if(newX > 0 && newX < 50 && newZ > 180 && newZ < 220) {
                collisionX = true; collisionZ = true;
            }
        }
        
        if (!collisionX && newX >= minX && newX <= maxX) {
            x = newX;
        }
        if (!collisionZ && newZ >= minZ && newZ <= maxZ) {
            z = newZ;
        }
        if (newY >= 0.5f && newY <= 30.0f) {
            y = newY;
        }
    }
    
    void handleMouse(int mx, int my, int mid_x, int mid_y) {
        float deltaX = (float)(mx - mid_x);
        float deltaY = (float)(my - mid_y);
        angleYaw += deltaX * mouseSensitivity;
        anglePitch -= deltaY * mouseSensitivity;
        if (anglePitch > 89.0f) anglePitch = 89.0f;
        if (anglePitch < -89.0f) anglePitch = -89.0f;
        updateVectors();
    }
    
    void setBounds(float minx, float maxx, float minz, float maxz) {
        minX = minx;
        maxX = maxx;
        minZ = minz;
        maxZ = maxz;
    }
};
