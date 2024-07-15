#include "WeatherDrawer.h"
#include "pictures.h"

const int kWidth = 13;
const int kLength = 25;
const int kLineElements = 6;
const int kIndent = 2;
const int kLineNumber = 1;
const int kWindowIndent = 3;

void WeatherDrawer::DrawWindow(WeatherRequest& obj, int curr_day, int curr_city) {
    WINDOW* new_window = derwin(stdscr, kWidth, kLength, kWidth * (curr_day / kLineElements) + kIndent,
                                kLength * (curr_day % kLineElements));
    std::string date = obj.cities_[curr_city].first["daily"]["time"][curr_day];
    std::string max_temperature = to_string(obj.cities_[curr_city].first["daily"]["temperature_2m_max"][curr_day]);
    std::string min_temperature = to_string(obj.cities_[curr_city].first["daily"]["temperature_2m_min"][curr_day]);
    std::string wind_speed = to_string(obj.cities_[curr_city].first["daily"]["windspeed_10m_max"][curr_day]) + " km/h";
    std::string precipitation =
            to_string(obj.cities_[curr_city].first["daily"]["precipitation_sum"][curr_day]) + " mm | " +
            to_string(obj.cities_[curr_city].first["daily"]["precipitation_probability_max"][curr_day]) + " percent";
    mvwprintw(new_window, kLineNumber, kWindowIndent + kIndent,
              weather_pictures[obj.cities_[curr_city].first["daily"]["weathercode"][curr_day]].c_str());
    mvwprintw(new_window, kLineNumber * 8, kWindowIndent,
              weathercode[obj.cities_[curr_city].first["daily"]["weathercode"][curr_day]].c_str());
    mvwprintw(new_window, kLineNumber * 9, kWindowIndent,
              (max_temperature + "(" + min_temperature + ")" + " C").c_str());
    mvwprintw(new_window, kLineNumber * 10, kWindowIndent, wind_speed.c_str());
    mvwprintw(new_window, kLineNumber * 11, kWindowIndent, precipitation.c_str());
    box(new_window, 0, 0);
    mvwprintw(new_window, kLineNumber * 0, kWindowIndent + kIndent * 2, date.c_str());
    wrefresh(new_window);
    windows_.emplace_back(new_window);
}

void WeatherDrawer::ClearWindow() {
    werase(windows_[windows_.size() - 1]);
    wrefresh(windows_[windows_.size() - 1]);
    delwin(windows_[windows_.size() - 1]);
    windows_.pop_back();
}

WeatherDrawer::~WeatherDrawer() {
    for (int i = 0; i < windows_.size(); ++i) {
        delete windows_[i];
    }
}

void WeatherDrawer::ClearPtr() {
    for (int i = 0; i < windows_.size(); ++i) {
        delete windows_[i];
    }
    windows_.clear();
}
