#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include <cstdlib>
using namespace std;

struct WeatherData {
    string city;
    string temperature;
    string windSpeed;
    string humidity;
    string pressure;
    string precipitation;
};

bool downloadPage(const string& url, const string& destFile) {
    string command = "curl -s " + url + " -o " + destFile;
    return system(command.c_str()) == 0;
}

WeatherData getWeatherData(const string& city) {
    string url = "https://sinoptik.ua/pohoda/" + city;
    string destFile = "weather.html";
    WeatherData data;
    data.city = city;

    if (!downloadPage(url, destFile)) {
        cout << "Помилка завантаження сторінки для міста " << city << ". URL: " << url << "\n";
        return data;
    }

    ifstream f(destFile);
    if (!f.is_open()) {
        cout << "Не вдалося відкрити файл: " << destFile << endl;
        return data;
    }

    string line;
    while (getline(f, line)) {
        smatch match;

        regex temperatureRegex(R"(<p class="_6fYCPKSx">(.*?)</p>)");
        if (regex_search(line, match, temperatureRegex)) {
            data.temperature = match[1];
        }

        regex windSpeedRegex(R"(<td class="some-wind-class">(.*?)</td>)");
        if (regex_search(line, match, windSpeedRegex)) {
            data.windSpeed = match[1];
        }

        regex humidityRegex(R"(<td class="some-humidity-class">(.*?)</td>)");
        if (regex_search(line, match, humidityRegex)) {
            data.humidity = match[1];
        }

        regex pressureRegex(R"(<td class="some-pressure-class">(.*?)</td>)");
        if (regex_search(line, match, pressureRegex)) {
            data.pressure = match[1];
        }

        regex precipitationRegex(R"(<td class="some-precipitation-class">(.*?)</td>)");
        if (regex_search(line, match, precipitationRegex)) {
            data.precipitation = match[1];
        }
    }

    f.close();
    remove(destFile.c_str());

    return data;
}

int main() {
    vector<string> cities = { "kremenchuk", "odesa", "kyiv", "lviv", "praha", "london", "paryzh", "berlin", "kyshyniv", "varshava" };
    vector<WeatherData> weatherDataList;

    for (const auto& city : cities) {
        WeatherData data = getWeatherData(city);
        weatherDataList.emplace_back(data);
    }

    for (const auto& data : weatherDataList) {
        cout << "Місто: " << data.city << endl;
        cout << "Температура: " << data.temperature << endl;
        cout << "Швидкість вітру: " << data.windSpeed << endl;
        cout << "Вологість: " << data.humidity << endl;
        cout << "Тиск: " << data.pressure << endl;
        cout << "Імовірність опадів: " << data.precipitation << endl;
        cout << "-----------------------------" << endl;
    }

    return 0;
}
