#include "WeatherDrawer.h"

#include <iostream>

using namespace nlohmann;

const int kMinArgvSize = 2;
const int kNameIndex = 1;
const int kMaxDays = 16;

int main(int argc, char* argv[]) {
    if (argc < kMinArgvSize) {
        std::cout << "No file name";

        return 0;
    }
    std::ifstream config(argv[kNameIndex]);
    if (!config.is_open()) {
        std::cout << "Wrong file name";
        config.close();

        return 0;
    }
    json json_config;
    config >> json_config;
    config.close();
    std::vector<std::string> names = json_config["cities"].get<std::vector<std::string>>();
    std::vector<std::pair<json, std::string>> cities;
    int refresh_time = json_config["refresh_time"];
    int days = json_config["days"];
    bool flag = true;
    WeatherRequest request(names);
    WeatherDrawer drawer;
    request.GetRequest();
    std::thread RefreshingThread(Refresh, std::ref(request), std::ref(flag), refresh_time);
    initscr();
    keypad(stdscr, true);
    noecho();
    int curr_day = 0;
    int curr_city = 0;
    printw(("Weather in " + request.GetCityName(curr_city) + ":\n").c_str());
    for (int i = 0; i < days; ++i) {
        drawer.DrawWindow(request, curr_day, curr_city);
        ++curr_day;
    }
    char ch;
    while (ch != 27) {
        ch = getch();
        if (ch == '+' && curr_day < kMaxDays) {
            drawer.DrawWindow(request, curr_day, curr_city);
            ++curr_day;
        } else if (ch == '-' && curr_day > 1) {
            drawer.ClearWindow();
            --curr_day;
        } else if (ch == 'n' && curr_city < names.size() - 1) {
            ++curr_city;
            werase(stdscr);
            wrefresh(stdscr);
            printw(("Weather in " + request.GetCityName(curr_city) + ":\n").c_str());
            curr_day = 0;
            drawer.ClearPtr();
            for (int i = 0; i < days; ++i) {
                drawer.DrawWindow(request, curr_day, curr_city);
                ++curr_day;
            }
        } else if (ch == 'p' && curr_city > 0) {
            --curr_city;
            werase(stdscr);
            wrefresh(stdscr);
            printw(("Weather in " + request.GetCityName(curr_city) + ":\n").c_str());
            curr_day = 0;
            drawer.ClearPtr();
            for (int i = 0; i < days; ++i) {
                drawer.DrawWindow(request, curr_day, curr_city);
                ++curr_day;
            }
        }
    }
    drawer.ClearPtr();
    endwin();
    std::unique_lock<std::mutex> lock(request.mutex);
    flag = false;
    lock.unlock();
    request.cond_var.notify_one();
    RefreshingThread.join();

    return 0;
}