#pragma once
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

class SoundManager {
private:
    bool soundEnabled;
    
public:
    SoundManager() {
        soundEnabled = true;
    }
    
    void playAmbient() {
        if(soundEnabled) {
            PlaySound(TEXT("sounds/ambient-music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        }
    }
    
    void playCarEngine() {
        if(soundEnabled) {
            PlaySound(TEXT("sounds/car-engine.wav"), NULL, SND_FILENAME | SND_ASYNC);
        }
    }
    
    void playDoor() {
        if(soundEnabled) {
            PlaySound(TEXT("sounds/open-car-door.wav"), NULL, SND_FILENAME | SND_ASYNC);
        }
    }
    
    void playElevator() {
        if(soundEnabled) {
            PlaySound(TEXT("sounds/elevetor-arrives.wav"), NULL, SND_FILENAME | SND_ASYNC);
        }
    }
    
    void playFootstep() {
        if(soundEnabled) {
            PlaySound(TEXT("sounds/footstep.wav"), NULL, SND_FILENAME | SND_ASYNC);
        }
    }
    
    void stopAll() {
        PlaySound(NULL, NULL, 0);
    }
    
    void toggle() {
        soundEnabled = !soundEnabled;
        if(!soundEnabled) stopAll();
    }
};
