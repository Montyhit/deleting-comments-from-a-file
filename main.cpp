// Использование конечного автомата

#include <fstream>
#include <iostream>
#include <cstdio>

enum states{search, text, del_comment_1, del_comment_2};	// 4 состояния: поиск коммента и запись в файл данных, обработка "текста", удаление коммента //, удаление коммента /**/

int main(int argc, char* argv[])
{

	if (argc != 2)
	{
		std::cout << "Invalid number of parameters entered." << std::endl;
		return 1;
	}

	std::fstream file (argv[1], std::ios::in);
	std::fstream new_file("new file.txt", std::ios::out | std::ios::app);

	char symbol_1;
	char symbol_2;
	int st = search;

	while (!file.eof())
	{
		file.read(reinterpret_cast<char*>(&symbol_1), sizeof(char));

		switch (st)
		{
		case search:
			if (symbol_1 == '/')
			{
				file.read(reinterpret_cast<char*>(&symbol_2), sizeof(char));
				if (symbol_1 == symbol_2)
				{
					st = del_comment_1;
					break;
				}
				if (symbol_1 == '/' && symbol_2 == '*')
				{
					st = del_comment_2;
					break;
				}
			}
			if (symbol_1 == '"')
			{
				st = text;
				new_file.write(reinterpret_cast<char*>(&symbol_1), sizeof(char));
				break;
			}
			new_file.write(reinterpret_cast<char*>(&symbol_1), sizeof(char));
			break;
		case text:
			if (symbol_1 == '\n' || symbol_1 == '"')
			{
				st = search;
				new_file.write(reinterpret_cast<char*>(&symbol_1), sizeof(char));
				break;
			}
			new_file.write(reinterpret_cast<char*>(&symbol_1), sizeof(char));
			break;
		case del_comment_1:
			if (symbol_1 == '\n')
			{
				st = search;
			}
			break;
		case del_comment_2:
			if (symbol_1 == '*')
			{
				file.read(reinterpret_cast<char*>(&symbol_2), sizeof(char));
				if (symbol_2 == '/')
				{
					st = search;
				}
			}
			break;
		}
	}

	file.close();
	new_file.close();

	return 0;
}

