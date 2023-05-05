#include <iostream>
#include <thread>
#include <chrono>
#include "WeatherApi.h"
#include "user_interaction.h"
extern class USER_INTERACTION user_interaction;

using namespace std;



class WeatherApp
{
public:

	virtual void start() = 0;
	
};

class WeatherData 
{

private:

	string temperature;
	string wind_speed;
	string humidity;
	double* ptr;
	double* hours_ptr;
	bool dataAvailable = false;

public:

	void setdata(WeatherApi& weatherApi)
	{

		temperature = weatherApi.getTemperature();
		wind_speed = weatherApi.getwind_speed();
		humidity = weatherApi.getHumidity();
		ptr = weatherApi.get_future_temp();
		hours_ptr = weatherApi.get_future_hours_temp();
		dataAvailable = true;
	}

	bool isDataAvailable()
	{
		return dataAvailable;
	}

	string  getTemperature()
	{
		return temperature;
	}

	string getwind_speed()
	{
		return wind_speed;
	}

	string getHumidity()
	{
		return humidity;
	}


	double* get_future_temp()
	{
		return ptr;
	}

	double* get_future_hours_temp()
	{
		return hours_ptr;
	}


	void reset()
	{
		dataAvailable = false;
	}
};




class WeatherDisplay 
{

public:

	int command;

	void  displaydata(WeatherData& weatherData)
	{

		cout << "                Temperature: " << weatherData.getTemperature() << endl;
		cout << "                Wind Speed : " << weatherData.getwind_speed() << endl;
		cout << "                Humidity   : " << weatherData.getHumidity() << endl;
		cout << "===============================================================================" << endl;

		double* ptr = weatherData.get_future_temp();
		double* hours_ptr = weatherData.get_future_hours_temp();

		if (command == 1)
		{
			for (int i = 1; i < 5; i++)
			{
				cout << "temperature for day  " << i << " " << *(ptr + i) << endl;
			}
		}
		else
		{
			for (int i = 1; i < 5; i++)
			{
				cout << "temperature for hours  " << i << " " << *(hours_ptr + i) << endl;
			}
		}


	}

};




class WeatherThread : public WeatherApp
{

private:
	WeatherData* weatherDataPtr;

	WeatherDisplay* weatherDisplayPtr;

	static int threadCount;

public:
	WeatherThread(WeatherData* weatherData, WeatherDisplay* weatherDisplay)
	{

		weatherDataPtr = weatherData;
		weatherDisplayPtr = weatherDisplay;

	}

	void start()
	{
		threadCount++;
		cout << "thread " << threadCount << endl;

		try
		{
			thread t(&WeatherThread::displayData, this);
			t.detach();
		}
		catch (const system_error& e)
		{
			std::cerr << "Caught system_error with code " << e.code()
				<< " meaning " << e.what() << '\n';
		}
		catch (const exception& e)
		{
			std::cerr << "Caught exception: " << e.what() << '\n';
		}
		catch (...)
		{
			std::cerr << "Caught unknown exception\n";
		}
	}


	void displayData()
	{

		while (true)
		{
			try {
				if (weatherDataPtr->isDataAvailable())
				{
					weatherDisplayPtr->displaydata(*weatherDataPtr);
					weatherDataPtr->reset();
				}
			}
			catch (exception& ex)
			{
				cerr << "Exception caught in WeatherThread::displayData: " << ex.what() << endl;
			}

		}
	}
};


int WeatherThread::threadCount = 0;



class StartApp : public WeatherApp
{

private:

	bool weatherThreadStarted = false;

public:

	void start()
	{
		user_interaction user_interaction;
		WeatherApi weatherApi;
		WeatherData weatherData;
		WeatherDisplay weatherDisplay;
		WeatherThread weatherThread(&weatherData, &weatherDisplay);

		while (true)
		{
			try
			{
				system("pause");
				string city = user_interaction.user_input();
				int command = user_interaction.get_command();
				weatherDisplay.command = command;
				weatherApi.Fetch_and_parse(city);
				weatherApi.fetch_future_temperatures(city, "f90a9af9897eb0f93f13ac73cf60cb6c");
				weatherApi.fetch_future_Hours_temperatures(city, "f90a9af9897eb0f93f13ac73cf60cb6c");
				weatherData.setdata(weatherApi);

				if (!weatherThreadStarted)
				{
					weatherThread.start();
					weatherThreadStarted = true;
				}

				this_thread::sleep_for(chrono::seconds(1));
			}
			catch (const exception& ex)
			{
				cout << "Exception occurred: " << ex.what() << endl;
				
			}
			catch (...)
			{
				cout << "Unknown exception occurred" << endl;
				
			}
		}
	}

};




int main()
{
	  
	 WeatherApp * app;

	 app = new StartApp();

	 app->start();

	return 0;
}
