#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include "DetailedCar.h"
#include "model.h"

class Showroom {
private:
    GLuint floorTex, wallTex, glassTex, ceilingTex, roadTex, signTex, skyTex, skyCeilingTex;
    GLuint carDisplayList, crystalDisplayList;
    GLuint paintingTex[4];
    
    float buildingWidth = 80.0f;
    float buildingDepth = 100.0f;
    float floorHeight = 8.0f;
    int numFloors = 4;
    
    bool doorOpening;
    float carRotation;
    bool elevatorMoving;
    int elevatorTarget;
    bool lightsOn;
    bool isNightMode;
    int currentFloor;
    
    // Driver seat mode
    bool inDriverSeat;
    float driverCarX, driverCarY, driverCarZ;
    float driverCarAngle;
    
    bool doorSoundPlayed;
    bool elevatorSoundPlayed;
    
    float cameraX, cameraZ;
    
    DetailedCar* mainCar;
    Model* mercModel;
    Model* shelbyModel;
    
    void setupLighting();
    void drawSkybox();
    void drawGround();
    void drawSurroundingBuildings();
    void drawSimpleBuilding(float x, float y, float z, float w, float h, float d);
    void drawMainBuilding();
    void drawGlassFrontWithDoor();
    void drawEntranceDecor();
    void drawFloor(int level);
    void drawElevator();
    void drawElevatorShaft();
    void drawCeilingLights(int level);
    void drawFloorSign(int level);
    void drawShowroomPlatforms(int level);
    void drawPlatform(float x, float y, float z, float w, float d, int type);
    void drawCar(float x, float y, float z, float angle, int type);
    void drawCarLabel(float x, float y, float z, const char* name, const char* price);
    void drawCrystal(float x, float y, float z, float size, float r, float g, float b, float rotation);
    void drawRotatingPlatform(float x, float y, float z, float radius, float rotation);
    void drawNeonLines(int level);
    void drawHUD();
    
    // Hall-specific drawing
    void drawSportsHall(int level);
    void drawFamilyHall(int level);
    void drawLuxuryHall(int level);
    void drawClassicHall(int level);
    
    GLuint loadTexture(const char* path);

public:
    float doorOpen;
    float elevatorY;
    Showroom();
    ~Showroom();
    void init();
    void createDisplayLists();
    void update();
    void draw();
    void toggleDoor();
    void toggleLights();
    void toggleNightMode();
    void callElevator(int floor);
    void updateCurrentFloor(float cameraY);
    void updateCameraPos(float x, float z) { cameraX = x; cameraZ = z; }
    void setSignTexture(const char* path);
    void setSkyCeilingTexture(const char* path);
    void interactWithCar();
    bool checkCollision(float x, float z, float radius);
    bool checkBuildingCollision(float x, float z);
    bool checkElevatorCollision(float x, float z);
    bool isNearCar(float camX, float camZ, float& carX, float& carY, float& carZ, float& carAngle);
    bool getDriverSeatMode() { return inDriverSeat; }
    void getDriverCarPos(float& x, float& y, float& z, float& angle) { x = driverCarX; y = driverCarY; z = driverCarZ; angle = driverCarAngle; }
    void updateDriverCar(float angle) { driverCarAngle = angle; }
    void toggleCarWheels();
    void enterCar();
    void toggleEngine();
};
