#include<iostream>
#include<fstream>
using std::cin;
using std::cout;
using std::endl;

//#define WRITE_TO_FILE
#define READ_FROM_FILE

void main()
{
	setlocale(LC_ALL, "");

#ifdef WRITE_TO_FILE
	//cout << "Hello Word!" << endl;

//1) Создаем поток:
	std::ofstream fout;

	//2) Открываем поток:
	fout.open("File.txt");

	//3) Пишем в поток:
	fout << "Hello Word!" << endl;

	//4) После использования потоки обязательно нужно закрывать!!!
	fout.close();

	system("notepad File.txt");
#endif // WRITE_TO_FILE

	//1) Создаем и открываем поток:
	std::ifstream fin("File.txt");

	//2) Проверяем состояние потока:
	if (fin.is_open())
	{
		//TODO: Читаем файл
		while (!fin.eof())//eof() - End Of File
		{
			const int SIZE = 256;
			char sz_buffer[SIZE] = {};
			//fin >> sz_buffer;
			fin.getline(sz_buffer, SIZE);
			cout << sz_buffer << endl;
		}
	}
	else
	{
		std::cerr << "Error: File not found" << endl;
	}
	fin.close();
}