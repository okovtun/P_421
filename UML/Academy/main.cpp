#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<time.h>
using std::cin;
using std::cout;
using std::endl;
//Default constructor - это конструктор, который может быть вызван без параметров.
class Human
{
	const std::string LAST_NAME;
	const std::string FIRST_NAME;
	const tm BIRTH_DATE;
public:
	const std::string& get_last_name()const
	{
		return LAST_NAME;
	}
	const std::string& get_first_name()const
	{
		return FIRST_NAME;
	}
	tm get_birth_date()const
	{
		return BIRTH_DATE;
	}
	int get_age()const
	{
		//1) Получаем текущее время в формате Timestamp:
		time_t t_today = time(NULL);
		tm tm_today = *localtime(&t_today);
		//2) Преобразуем дату рождения в формат Timestamp:
		tm birth_date = BIRTH_DATE;
		time_t t_birth_date = mktime(&birth_date);

		//3) Находим разницу во времени:
		time_t difference = t_today - t_birth_date;

		//4) Преобразуем разницу во времени в формат 'tm':
		tm age = *localtime(&difference);
		age.tm_year -= 70;
		return age.tm_year;
	}
	tm parse_date(const std::string& date)
	{
		//yyyy-MM-dd
		//yyyy.MM.dd
		//yyyy/MM/dd
		char sz_date[32] = {};
		strcpy(sz_date, date.c_str());
		int date_parts[3] = {};
		int n = 0;
		const char delimiters[] = "-./";
		for (char* pch = strtok(sz_date, delimiters); pch; pch = strtok(NULL, delimiters))
			date_parts[n++] = atoi(pch);
		date_parts[0] -= 1900;	//структура 'tm' хранит дату с 1 января 1900 года.
		date_parts[1]--;		//месяца нумеруются с нуля

		tm tm_birth_date = {};
		tm_birth_date.tm_year = date_parts[0];
		tm_birth_date.tm_mon = date_parts[1];
		tm_birth_date.tm_mday = date_parts[2];
		return tm_birth_date;
	}
	Human(const std::string& last_name, const std::string& first_name, const std::string& birth_date) :
		LAST_NAME(last_name), FIRST_NAME(first_name), BIRTH_DATE(parse_date(birth_date))
	{
		cout << "HConstructor:\t" << this << endl;
	}
	~Human()
	{
		cout << "HDestructor:\t" << this << endl;
	}
	void info()const
	{
		cout << LAST_NAME << " " << FIRST_NAME << " " << get_age() << endl;
	}
};

class AcademyMember :public Human
{
	std::string speciality;
public:
	const std::string get_speciality()const
	{
		return speciality;
	}
	void set_speciality(const std::string& speciality)
	{
		this->speciality = speciality;
	}
	AcademyMember
	(
		const std::string& last_name, const std::string& first_name, const std::string& birth_date,
		const std::string& speciality
	):Human(last_name,first_name,birth_date)
	{
		set_speciality(speciality);
		cout << "AMConstructor:\t" << this << endl;
	}
	~AcademyMember()
	{
		cout << "AMDestructor:\t" << this << endl;
	}
	void info()const
	{
		Human::info();	//Вызываем метод info() для класса 'Human'.
		cout << speciality << endl;
	}
};

void main()
{
	setlocale(LC_ALL, "");
	cout << "Academy" << endl;
	Human human("Тупенко", "Василий", "2009.09.02");
	human.info();

	AcademyMember am("Тупенко", "Василий", "2000.10.24", "Флуктуации пространства в вакууме");
	am.info();
}