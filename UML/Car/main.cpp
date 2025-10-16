#pragma warning(disable:4326)
#include<Windows.h>
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
	double give_fuel(double amount)
	{
		fuel_level -= amount;
		if (fuel_level < 0)fuel_level = 0;
		return fuel_level;
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
	double get_consumption_per_second()const
	{
		return consumption_per_second;
	}
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
	int accelleration;
	bool driver_inside;
	struct Threads	//Создаем и описываем структуру
	{
		std::thread panel_thread;
		std::thread engine_idle_thread;
		std::thread free_wheeling_thread;
	}threads;		//Создаем экземпляр этой структуры сразу же после ее описания
public:
	Car(int consumption, int volume, int max_speed = 250, int accelleration = 20) :
		engine(consumption),
		tank(volume),
		MAX_SPEED
		(
			max_speed < MAX_SPEED_LOWER_LIMIT ? MAX_SPEED_LOWER_LIMIT :
			max_speed > MAX_SPEED_UPPER_LIMIT ? MAX_SPEED_UPPER_LIMIT :
			max_speed
		),
		speed(0),
		accelleration(accelleration)
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
	void start()
	{
		if (tank.get_fuel_level())
		{
			engine.start();
			threads.engine_idle_thread = std::thread(&Car::engine_idle, this);
		}
	}
	void stop()
	{
		engine.stop();
		if (threads.engine_idle_thread.joinable())threads.engine_idle_thread.join();
	}
	void accellerate()
	{
		speed += accelleration;
		if (speed > MAX_SPEED)speed = MAX_SPEED;
		if (!threads.free_wheeling_thread.joinable())
			threads.free_wheeling_thread = std::thread(&Car::free_wheeling, this);
		std::this_thread::sleep_for(1s);
	}
	void slow_down()
	{
		speed -= accelleration;
		if (speed < 0)speed = 0;
		std::this_thread::sleep_for(1s);
	}
	void control()
	{
		char key;
		do
		{
			key = 0;
			if (_kbhit())key = _getch();
			switch (key)
			{
			case Enter:
				if (driver_inside)get_out();
				else get_in();
				break;
			case 'F':
			case 'f':
				if (driver_inside)
				{
					cout << "Get out of your car\a" << endl;
				}
				else
				{
					double amount;
					cout << "How much do you whant: ";
					cin >> amount;
					//cin.clear();
					//cin.ignore();
					tank.fill(amount);
				}
				break;
			case 'i':	//Ignition - зажигание
			case 'I':
				if (engine.started())stop();
				else start();
				break;
			case 'w':
			case 'W':
				if (driver_inside && engine.started())accellerate();
				break;
			case 's':
			case 'S':
				if (driver_inside)slow_down();
				break;
			case Escape:
				stop();
				get_out();
			}
			if (tank.get_fuel_level() == 0)engine.stop();
			if (speed == 0 && threads.free_wheeling_thread.joinable())
				threads.free_wheeling_thread.join();
		} while (key != Escape);
	}
	void free_wheeling()
	{
		while (speed > 0)
		{
			speed--;
			if (speed < 0)speed = 0;
			std::this_thread::sleep_for(1s);
		}
	}
	void engine_idle()
	{
		while (engine.started() && tank.give_fuel(engine.get_consumption_per_second()))
		{
			std::this_thread::sleep_for(1s);
		}
	}
	void panel()const
	{
		while (driver_inside)
		{
			system("CLS");
			cout << "Speed:\t" << speed << " km/h\n";
			cout << "Fuel level: " << tank.get_fuel_level() << " liters.";
			if (tank.get_fuel_level() < 5)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, 0xCF);
				cout << " LOW FUEL ";
				SetConsoleTextAttribute(hConsole, 0x07);
			}
			cout << endl;
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