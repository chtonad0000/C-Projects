#pragma once

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using namespace nlohmann;

class WeatherRequest {
public:
    WeatherRequest(const std::vector<std::string>& names);

    void GetRequest();

    std::pair<json, std::string> GetCity(const std::string& city_name);

    friend class WeatherDrawer;

    std::string GetCityName(int number);

    std::mutex mutex;
    std::condition_variable cond_var;
private:
    std::vector<std::pair<json, std::string>> cities_;
    std::vector<std::string> names_;
    std::vector<std::thread> threads_;
};

void Refresh(WeatherRequest& obj, bool& flag, size_t time);

