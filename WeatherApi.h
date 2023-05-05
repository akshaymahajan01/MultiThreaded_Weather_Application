#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#pragma comment(lib, "wininet.lib")

using json = nlohmann::json;
using namespace std;


class WeatherApi
{

private:


	string Temperature;
	string wind_speed;
	string Humidity;
	double future_temp[5];
	double future_hours_temp[5];


public:


	int Fetch_and_parse(string city)
	{
		// Open an internet session
		HINTERNET hInternet = InternetOpen(TEXT("MyApp"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
		if (hInternet == NULL)
		{
			cerr << "Failed to open internet session. Error code: " << GetLastError() << endl;
			return 1;
		}

		// Open a connection to the weather website
		HINTERNET hConnection = InternetConnect(hInternet, TEXT("api.openweathermap.org"), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
		if (hConnection == NULL)
		{
			cerr << "Failed to connect to the weather website. Error code: " << GetLastError() << endl;
			InternetCloseHandle(hInternet);
			return 1;
		}

		wstring url = L"/data/2.5/weather?q=" + wstring(city.begin(), city.end()) + L"&appid=f90a9af9897eb0f93f13ac73cf60cb6c";


		// Open a request for the current weather data
		HINTERNET hRequest = HttpOpenRequest(hConnection, TEXT("GET"), url.c_str(), NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
		if (hRequest == NULL)
		{
			cerr << "Failed to open request for weather data. Error code: " << GetLastError() << endl;
			InternetCloseHandle(hConnection);
			InternetCloseHandle(hInternet);
			return 1;
		}

		// Send the request and retrieve the weather data
		if (!HttpSendRequest(hRequest, NULL, 0, NULL, 0))
		{
			cerr << "Failed to send request for weather data. Error code: " << GetLastError() << endl;
			InternetCloseHandle(hRequest);
			InternetCloseHandle(hConnection);
			InternetCloseHandle(hInternet);
			return 1;
		}

		string weatherData;
		const int bufferSize = 1024;
		char buffer[bufferSize];
		DWORD bytesRead;
		while (InternetReadFile(hRequest, buffer, bufferSize, &bytesRead) && bytesRead != 0)
		{
			weatherData.append(buffer, bytesRead);
		}

		// Close the request, connection, and internet session
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hInternet);

		// Parse the weather data as JSON
		json j = json::parse(weatherData);

		// Extract the temperature, wind speed, and humidity from the JSON data
		double temperature = j["main"]["temp"];
		double windSpeed = j["wind"]["speed"];
		int humidity = j["main"]["humidity"];

		// Display the weather data on the screen
		Temperature = "Temperature: " + to_string(temperature) + "K";
		wind_speed = "Wind Speed : " + to_string(windSpeed) + "m/s";
		Humidity = "Humidity : " + to_string(humidity) + "%";
		// MessageBoxA(NULL, message.c_str(), "Weather Data", MB_OK);a

	}

	void fetch_future_temperatures(string city, string api_key) {
		// Open an internet session
		HINTERNET hInternet = InternetOpen(TEXT("MyApp"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
		if (hInternet == NULL)
		{
			cerr << "Failed to open internet session. Error code: " << GetLastError() << endl;
			return;
		}

		// Open a connection to the weather website
		HINTERNET hConnection = InternetConnect(hInternet, TEXT("api.openweathermap.org"), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
		if (hConnection == NULL)
		{
			cerr << "Failed to connect to the weather website. Error code: " << GetLastError() << endl;
			InternetCloseHandle(hInternet);
			return;
		}

		wstring url = L"/data/2.5/forecast?q=" + wstring(city.begin(), city.end()) + L"&appid=" + wstring(api_key.begin(), api_key.end());

		// Open a request for the weather forecast
		HINTERNET hRequest = HttpOpenRequest(hConnection, TEXT("GET"), url.c_str(), NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
		if (hRequest == NULL)
		{
			cerr << "Failed to open request for weather forecast. Error code: " << GetLastError() << endl;
			InternetCloseHandle(hConnection);
			InternetCloseHandle(hInternet);
			return;
		}

		// Send the request and retrieve the weather forecast data
		if (!HttpSendRequest(hRequest, NULL, 0, NULL, 0))
		{
			cerr << "Failed to send request for weather forecast data. Error code: " << GetLastError() << endl;
			InternetCloseHandle(hRequest);
			InternetCloseHandle(hConnection);
			InternetCloseHandle(hInternet);
			return;
		}

		string weatherData;
		const int bufferSize = 1024;
		char buffer[bufferSize];
		DWORD bytesRead;
		while (InternetReadFile(hRequest, buffer, bufferSize, &bytesRead) && bytesRead != 0)
		{
			weatherData.append(buffer, bytesRead);
		}

		// Close the request, connection, and internet session
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hInternet);

		// Parse the weather forecast data as JSON
		json j = json::parse(weatherData);

		// Extract the temperature for the next five days from the JSON data
		json daily = j["list"];
		for (int i = 0; i < 5; i++) {
			future_temp[i] = daily[i]["main"]["temp"];
		}
	}



	void fetch_future_Hours_temperatures(string city, string api_key)
	{
		// Open an internet session
		HINTERNET hInternet = InternetOpen(TEXT("MyApp"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
		if (hInternet == NULL)
		{
			cerr << "Failed to open internet session. Error code: " << GetLastError() << endl;
			return;
		}

		// Open a connection to the weather website
		HINTERNET hConnection = InternetConnect(hInternet, TEXT("api.openweathermap.org"), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
		if (hConnection == NULL)
		{
			cerr << "Failed to connect to the weather website. Error code: " << GetLastError() << endl;
			InternetCloseHandle(hInternet);
			return;
		}

		wstring url = L"/data/2.5/forecast?q=" + wstring(city.begin(), city.end()) + L"&appid=" + wstring(api_key.begin(), api_key.end());

		// Open a request for the weather forecast
		HINTERNET hRequest = HttpOpenRequest(hConnection, TEXT("GET"), url.c_str(), NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
		if (hRequest == NULL)
		{
			cerr << "Failed to open request for weather forecast. Error code: " << GetLastError() << endl;
			InternetCloseHandle(hConnection);
			InternetCloseHandle(hInternet);
			return;
		}

		// Send the request and retrieve the weather forecast data
		if (!HttpSendRequest(hRequest, NULL, 0, NULL, 0))
		{
			cerr << "Failed to send request for weather forecast data. Error code: " << GetLastError() << endl;
			InternetCloseHandle(hRequest);
			InternetCloseHandle(hConnection);
			InternetCloseHandle(hInternet);
			return;
		}

		string weatherData;
		const int bufferSize = 1024;
		char buffer[bufferSize];
		DWORD bytesRead;
		while (InternetReadFile(hRequest, buffer, bufferSize, &bytesRead) && bytesRead != 0)
		{
			weatherData.append(buffer, bytesRead);
		}

		// Close the request, connection, and internet session
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hInternet);

		// Parse the weather forecast data as JSON
		json j = json::parse(weatherData);

		// Extract the temperature for the next five days from the JSON data
		json hourly = j["list"];
		for (int i = 0; i < 5; i++) {
			future_hours_temp[i] = hourly[i]["main"]["temp"];
		}

	}



	string getTemperature()
	{
		return Temperature;
	}

	string getwind_speed()
	{
		return wind_speed;
	}

	string getHumidity()
	{
		return Humidity;
	}

	double* get_future_temp() {

		double* arr = new double[5];

		for (int i = 0; i < 5; i++) {

			arr[i] = future_temp[i];

		}
		return arr;
	}


	double* get_future_hours_temp() {

		double* arr = new double[5];

		for (int i = 0; i < 5; i++) {

			arr[i] = future_hours_temp[i];

		}
		return arr;
	}



};
