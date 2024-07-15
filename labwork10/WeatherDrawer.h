#pragma once

#include "Request.h"
#include <curses.h>

class WeatherDrawer {
public:
    void DrawWindow(WeatherRequest& obj, int curr_day, int curr_city);

    void ClearWindow();

    void ClearPtr();

    ~WeatherDrawer();

private:
    std::vector<WINDOW*> windows_;
};
