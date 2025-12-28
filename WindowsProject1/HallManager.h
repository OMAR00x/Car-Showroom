#pragma once

#include "BaseHall.h"
#include "SportsCarHall.h"
#include <vector>

class HallManager {
private:
    std::vector<BaseHall*> halls;
    int currentHallIndex;

public:
    HallManager();
    ~HallManager();
    void init();
    void update();
    void draw();
    void switchHall(int index);
    int getCurrentHallIndex() const { return currentHallIndex; }
};
