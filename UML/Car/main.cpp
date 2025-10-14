#pragma warning(disable:4326)
#include<iostream>
#include<conio.h>
#include<thread>
using namespace std;
using namespace std::chrono_literals;

#define Enter	13
#define Escape	27

#define MIN_TANK_VOLUME 30
#define MAX_TANK_VOLUME 120
class Tank
{
	const int VOLUME;	//Объем бака
	double fuel_level;	//Уровень топлива
public:
	int get_VOLUME()const
	{
		return VOLUME;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}
	void fill(double amount)
	{
		if (amount < 0) return;
		if (fuel_level + amount < VOLUME)fuel_level += amount;
		else fuel_level = VOLUME;
	}
	Tank(int volume) :
		VOLUME
		(
			volume < MIN_TANK_VOLUME ? MIN_TANK_VOLUME :
			volume > MAX_TANK_VOLUME ? MAX_TANK_VOLUME :
			volume
		)
	{
		//	VOLUME = volume;
		//	l-value = r-value;
			//	l-value - это переменная слева от оператора 'присвоить' =
		fuel_level = 0;
		cout << "Tank is ready:\t" << this << endl;
	}
	//expression - выражение
	~Tank()
	{
		cout << "Tank is over:\t" << this << endl;
	}

	void info()const
	{
		cout << "Volume:\t\t" << VOLUME << " liters.\n";
		cout << "Fuel level:\t" << fuel_level << " liters.\n";
	}
};

#define MIN_ENGINE_CONSUMPTION 3
#define MAX_ENGINE_CONSUMPTION 25
class Engine
{
	const double CONSUMPTION;		//Расход топлива на 100 км
	double consumption_per_second;	//Расход топлива за 1 секунду
	bool is_started;
public:
	Engine(double consumption) :
		CONSUMPTION
		(
			consumption < MIN_ENGINE_CONSUMPTION ? MIN_ENGINE_CONSUMPTION :
			consumption > MAX_ENGINE_CONSUMPTION ? MAX_ENGINE_CONSUMPTION :
			consumption
		)
	{
		consumption_per_second = CONSUMPTION * 3e-5;	//e - Exponent (Для десятичной системы счисления = 10)
		is_started = false;
		cout << "Engine is ready:\t" << this << endl;
	}
	~Engine()
	{
		cout << "Engine is over:\t" << this << endl;
	}
	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	bool started()const
	{
		return is_started;
	}

	void info()const
	{
		cout << "Engine:\n";
		cout << "Consumption per 100 km:\t" << CONSUMPTION << " liters\n";
		cout << "Consumption per second:\t" << consumption_per_second << " liters\n";
	}
};

#define MAX_SPEED_LOWER_LIMIT 40
#define MAX_SPEED_UPPER_LIMIT 400
class Car
{
	Engine engine;
	Tank tank;
	const int MAX_SPEED;
	int speed;
	bool driver_inside;
	struct Threads	//Создаем и описываем структуру
	{
		std::thread panel_thread;
	}threads;		//Создаем экземпляр этой структуры сразу же после ее описания
public:
	Car(int consumption, int volume, int max_speed = 250) :
		engine(consumption),
		tank(volume),
		MAX_SPEED
		(
			max_speed < MAX_SPEED_LOWER_LIMIT ? MAX_SPEED_LOWER_LIMIT :
			max_speed > MAX_SPEED_UPPER_LIMIT ? MAX_SPEED_UPPER_LIMIT :
			max_speed
		),
		speed(0)
	{
		driver_inside = false;
		cout << "Your car is ready:\t" << this << endl;
		cout << "Press Enter to get in:" << endl;
	}
	~Car()
	{
		cout << "Car is over:\t" << this << endl;
	}
	void get_in()
	{
		driver_inside = true;
		//panel();
		threads.panel_thread = std::thread(&Car::panel, this);
	}
	void get_out()
	{
		driver_inside = false;
		if (threads.panel_thread.joinable())threads.panel_thread.join();
		system("CLS");	//Очистка экрана
		cout << "You are out of you car" << endl;
	}
	void control()
	{
		char key;
		do
		{
			key = _getch();
			switch (key)
			{
			case Enter:
				if (driver_inside)get_out();
				else get_in();
				break;
			case 'F':
			case 'f':
				double amount;
				cout << "How much do you whant: "; cin >> amount;
				tank.fill(amount);
				break;
			}
		} while (key != Escape);
	}
	void panel()const
	{
		while (driver_inside)
		{
			system("CLS");
			cout << "Fuel level: " << tank.get_fuel_level() << " liters.\n";
			cout << "Engine is " << (engine.started() ? "started" : "stopped") << endl;
			std::this_thread::sleep_for(500ms);
		}
	}
	void info()const
	{
		engine.info();
		tank.info();
		cout << "Max speed: " << MAX_SPEED << " km/h\n";
		cout << "Speed: " << speed << " km/h\n";
	}
};

//#define TANK_CHECK
//#define ENGINE_CHECK

void main()
{
	setlocale(LC_ALL, "");

#ifdef TANK_CHECK
	cout << "Hello Car" << endl;
	Tank tank(-60);
	tank.info();
	do
	{
		double fuel;
		cout << "Введите объем топлива: "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (true);
#endif // TANK_CHECK

#ifdef ENGINE_CHECK
	Engine engine(10);
	engine.info();
#endif // ENGINE_CHECK

	Car bmw(10, 80, 270);
	bmw.control();
}