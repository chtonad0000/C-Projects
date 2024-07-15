#include "Request.h"

const std::string kApiKey = "ppx0U2sIifYeLWyVTlUGdw==7E90nPEgZpLqOOXx";

void WeatherRequest::GetRequest() {
    std::vector<std::future<std::pair<nlohmann::json, std::string>>> futures;
    for (const std::string& city: names_) {
        futures.emplace_back(std::async(std::launch::async, &WeatherRequest::GetCity, this, city));
    }
    std::vector<std::pair<json, std::string>> cities;
    for (size_t i = 0; i < futures.size(); ++i) {
        std::pair<nlohmann::json, std::string> new_data = futures[i].get();
        cities.emplace_back(new_data);
    }
    cities_ = cities;
}

WeatherRequest::WeatherRequest(const std::vector<std::string>& names) {
    names_ = names;
}

void Refresh(WeatherRequest& obj, bool& flag, size_t time) {
    while (flag) {
        std::unique_lock<std::mutex> lock(obj.mutex);
        obj.cond_var.wait_for(lock, std::chrono::milliseconds(time), [&flag] {
            return !flag;
        });
        if (!flag) {
            return;
        }
        obj.GetRequest();
    }
}

std::string WeatherRequest::GetCityName(int number) {
    return cities_[number].second;
}

std::pair<json, std::string> WeatherRequest::GetCity(const std::string& city_name) {
    std::string name = city_name;
    std::string api_url = "https://api.api-ninjas.com/v1/city?name=" + name;
    cpr::Response response = cpr::Get(cpr::Url{api_url}, cpr::Header{{"X-Api-Key", kApiKey}});
    json j = json::parse(response.text.substr(1, response.text.size() - 2));
    std::string latitude = to_string(j["latitude"]);
    std::string longitude = to_string(j["longitude"]);
    std::string weather_url =
            "https://api.open-meteo.com/v1/forecast?latitude=" + latitude + "&longitude=" + longitude +
            "&daily=weathercode,temperature_2m_max,temperature_2m_min,precipitation_sum,precipitation_probability_max,windspeed_10m_max&current_weather=true&forecast_days=16&timezone=auto";
    cpr::Response weather_data = cpr::Get(cpr::Url{weather_url});
    json weather_info = json::parse(weather_data.text);

    return std::make_pair(weather_info, name);
}
