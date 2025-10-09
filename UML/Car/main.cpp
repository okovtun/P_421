#pragma warning(disable:4326)
#include<iostream>
using namespace std;

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
	Tank(int volume):
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

void main()
{
	setlocale(LC_ALL, "");
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
}