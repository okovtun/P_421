#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<time.h>
using std::cin;
using std::cout;
using std::endl;

#define tab "\t"
#define delimiter "\n-------------------------------------\n"

//Default constructor - это конструктор, который может быть вызван без параметров.
#define HUMAN_TAKE_PARAMETERS const std::string& last_name, const std::string& first_name, const std::string& birth_date
#define HUMAN_GIVE_PARAMETERS last_name, first_name, birth_date
//Директива - это указание компилятору на выполнение некоторых действий.
//Директива #define создает МАКРООПРЕДЕЛЕНИЕ (МАКРОС).
//У макроса есть ИМЯ и значение, и везде, где компилятор видет ИМЯ МАКРОСА он вставляет значение макроса,
//все это происходит во время компиляции.
//Имя макроса можно считать псевдонимом для фрагмента кода.

//Директива #define показывает компилятору что заменить и чем заменить.
//В нашем случае 'HUMAN_TAKE_PARAMETERS' заменяется на реальный список принимаемых параметров,
//а 'HUMAN_GIVE_PARAMETERS' - на список передаваемых параметров.
//Здесь 'HUMAN_GIVE_PARAMETERS' - это имя макроса
//'last_name, first_name, birth_date' - значение макроса.
//#ifdef MACROS ... #endif
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
	Human(HUMAN_TAKE_PARAMETERS) :
		LAST_NAME(last_name), FIRST_NAME(first_name), BIRTH_DATE(parse_date(birth_date))
	{
		cout << "HConstructor:\t" << this << endl;
	}
	~Human()
	{
		cout << "HDestructor:\t" << this << endl;
	}
	virtual std::ostream& info(std::ostream& os)const
	{
		return os << LAST_NAME << " " << FIRST_NAME << " " << get_age();
	}
	/*
	-----------------------------
	__vfptr - Virtual Functions Pointers (Таблица указателей на виртуальные функции)
	virtual type name(parameters) modifiers
	{
		......;
		......;
		......;
	}
	-----------------------------
	*/
};
std::ostream& operator<<(std::ostream& os, const Human& obj)
{
	return obj.info(os);
	/*return os 
		<< obj.get_last_name() 
		<< " " << obj.get_first_name() 
		<< " " << obj.get_age();*/
}

#define ACADEMY_MEMBER_TAKE_PARAMETERS const std::string& speciality
#define ACADEMY_MEMBER_GIVE_PARAMETERS speciality
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
	AcademyMember(HUMAN_TAKE_PARAMETERS, ACADEMY_MEMBER_TAKE_PARAMETERS) :Human(HUMAN_GIVE_PARAMETERS)
	{
		set_speciality(speciality);
		cout << "AMConstructor:\t" << this << endl;
	}
	~AcademyMember()
	{
		cout << "AMDestructor:\t" << this << endl;
	}
	std::ostream& info(std::ostream& os)const override
	{
		return Human::info(os) << " " << speciality;
		//Human::info(os);	//Вызываем метод info() для класса 'Human'.
		//cout << speciality << endl;
	}
};

#define STUDENT_TAKE_PARAMETERS const std::string& group, double rating, double attendance
#define STUDENT_GIVE_PARAMETERS group, rating, attendance
class Student :public AcademyMember
{
	std::string group;
	double rating;
	double attendance;
public:
	const std::string& get_group()const
	{
		return group;
	}
	double get_rating()const
	{
		return rating;
	}
	double get_attendance()const
	{
		return attendance;
	}
	void set_group(const std::string& group)
	{
		this->group = group;
	}
	void set_rating(double rating)
	{
		this->rating = rating;
	}
	void set_attendance(double attendance)
	{
		this->attendance = attendance;
	}

	//				Constructors:
	Student(HUMAN_TAKE_PARAMETERS, ACADEMY_MEMBER_TAKE_PARAMETERS, STUDENT_TAKE_PARAMETERS) :
		AcademyMember(HUMAN_GIVE_PARAMETERS, ACADEMY_MEMBER_GIVE_PARAMETERS)
	{
		set_group(group);
		set_rating(rating);
		set_attendance(attendance);
		cout << "SConstructor:\t" << this << endl;
	}
	~Student()
	{
		cout << "SDestructor:\t" << this << endl;
	}

	//			/Methods:
	std::ostream& info(std::ostream& os)const override
	{
		return AcademyMember::info(os) << " " << group << " " << rating << " " << attendance;
		//AcademyMember::info();
		//cout << group << " " << rating << " " << attendance << endl;
	}
};

#define TEACHER_TAKE_PARAMETERS int experience
#define TEACHER_GIVE_PARAMETERS experience
class Teacher :public AcademyMember
{
	int experience;
public:
	int get_experience()const
	{
		return experience;
	}
	void set_experience(int experience)
	{
		this->experience = experience;
	}
	//				Cnstructors:
	Teacher(HUMAN_TAKE_PARAMETERS, ACADEMY_MEMBER_TAKE_PARAMETERS, TEACHER_TAKE_PARAMETERS) :
		AcademyMember(HUMAN_GIVE_PARAMETERS, ACADEMY_MEMBER_GIVE_PARAMETERS)
	{
		set_experience(experience);
		cout << "TConstructor:\t" << this << endl;
	}
	~Teacher()
	{
		cout << "TDestructor:\t" << this << endl;
	}
	//				Methods:
	std::ostream& info(std::ostream& os)const 
	{
		return AcademyMember::info(os) << " " << experience;
		//AcademyMember::info();
		//cout << experience << endl;
	}
};

#define GRADUATE_TAKE_PARAMETERS const std::string& subject
#define GRADUATE_GIVE_PARAMETERS subject
class Graduate :public Student
{
	std::string subject;
public:
	const std::string& get_subject()const
	{
		return subject;
	}
	void set_subject(const std::string& subject)
	{
		this->subject = subject;
	}
	Graduate
	(
		HUMAN_TAKE_PARAMETERS,
		ACADEMY_MEMBER_TAKE_PARAMETERS,
		STUDENT_TAKE_PARAMETERS,
		GRADUATE_TAKE_PARAMETERS
	) :Student(HUMAN_GIVE_PARAMETERS, ACADEMY_MEMBER_GIVE_PARAMETERS, STUDENT_GIVE_PARAMETERS)
	{
		set_subject(subject);
		cout << "GConstructor:\t" << this << endl;
	}
	~Graduate()
	{
		cout << "GDestructor:\t" << this << endl;
	}
	std::ostream& info(std::ostream& os)const override
	{
		return Student::info(os) << " " << subject;
		//Student::info();
		//cout << subject << endl;
	}
};

//#define INHERITANCE

void main()
{
	setlocale(LC_ALL, "");

#ifdef INHERITANCE
	cout << "Academy" << endl;
	Human human("Тупенко", "Василий", "2009.09.02");
	human.info();

	AcademyMember am("Тупенко", "Василий", "2000.10.24", "Флуктуации пространства в вакууме");
	am.info();

	Student student("Чухарев", "Матвей", "2009.09.02", "Разработка программного обеспечения", "P_421", 100, 100);
	student.info();

	Teacher teacher("Einstein", "Albert", "1979.03.14", "Astronomy", 20);
	teacher.info();
#endif // INHERITANCE
	//Generalisation - обобщение;
	//Upcast - это приведение дочернего объекта к базовому типу;
	Human* group[] =
	{
		new Student("Чухарев", "Матвей", "2009.09.02", "Разработка программного обеспечения", "P_421", 100, 100),
		new Teacher("Einstein", "Albert", "1979.03.14", "Astronomy", 20),
		new Student("Гусев", "Савелий", "2010.08.29", "Разработка программного обеспечения", "P_421",98,98),
		new Teacher("Олег", "Анатольевич","1985.01.16", "Разработка программного обеспечения", 16),
		new Graduate("Львов", "Константин", "2009.09.21", "Разработка программного обеспечения", "P_421", 100, 98, "Разработка системы доставки пиццы")
	};
	cout << sizeof(group) << endl;
	cout << delimiter << endl;
	for (int i = 0; i < sizeof(group) / sizeof(group[0]); i++)
	{
		//group[i]->info();
		//<< - оператор перенаправления в поток
		cout << *group[i] << endl;
		cout << delimiter << endl;
	}
	/*
	Полиморфизм (Polymorphism)
	Polymorphism (poly - много, morphis - форма)
	AdHoc polymorphism
	Runtime polymorphism
		1. Указатели на базовый класс;
		2. Виртуальные функции; Specialization
	*/
}