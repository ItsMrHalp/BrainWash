#include <vector>
#include <iostream>
#include <utility>
#include <map>
#include <string>
#include <stack>
#include <tuple>

#define Log

std::map<std::string,std::vector<unsigned char>> read_from_file(char* path)
{
	std::map<std::string, std::vector<unsigned char>> funcmass;
	FILE* basicfile;
	errno_t err;
	err = fopen_s(&basicfile, path, "r");
	if(err)
		throw std::exception("Incorrect Path");
	std::vector<unsigned char> code;
	std::string name("");
	char c;
	bool CodeOrComment = true; // Code = true / Comment = false
	bool OutsideFunc = true;
	while ((c=getc(basicfile))!= EOF) {
		if (c == '#') 
		{
			CodeOrComment = !CodeOrComment;
			continue;
		}
		if (c == '\n' || c==' ' || c=='\t')
			continue;
		if (CodeOrComment) 
		{
			if (c != '<' && c != '>' && c != '[' && c != ']' && c != '+' && c != '-' && c != '.' && c != ',' && c !='c' && c != 'r' && c != 'i')
				throw std::exception("Contain incorrect sympols");
			if (c == 'c')
			{
				OutsideFunc = !OutsideFunc;
				if (OutsideFunc)
				{
					funcmass.insert({ name,code });
					name = "";
					code.clear();
				}
				continue;
			}
			if (OutsideFunc)
			{
				name += c;
			}
			else
				code.push_back(unsigned char(c));
		}
	}
	if (!funcmass.size())
		throw std::exception("File empty");
	err = fclose(basicfile);
	if (err)
		throw std::exception("File doesn't want to close");
	return funcmass;
}



std::vector<std::pair<int, int>> find_pair(std::vector<unsigned char> code)
{
	std::vector<std::pair<int, int>> ret;
	int i=code.size()-1, j;
	while (true) {
		while (code[i] != '[')
		{
			i--;
			if (i < 0) return ret;
		}
		j = i;
		while (code[j] != ']' && j < code.size())
			j++;
		code[i] = 0;
		code[j] = 0;
		std::pair<int, int> p{ i,j };
		ret.push_back(p);
	}
	return ret;
}

int main(int argc, char* argv[])
{
	int memsize = 30;
	int count;
	std::map<std::string, std::vector<unsigned char>> funcmass;
	std::stack<std::tuple<std::string, int, unsigned char*>> stackfunc;
	std::tuple<std::string, int, unsigned char*> get_info_back;
	bool funccall = false;
	std::string funccallname = "";
	std::string actualfunc = "+][+";
	std::vector<unsigned char> code;
	unsigned char* MainArray;
	unsigned char* TempArray;
	try {
		if(argc < 2)
			throw std::exception("Not enough arguments");
		funcmass = read_from_file(argv[1]);
		if (argc == 3)
			memsize = std::atoi(argv[2]);

		//ищем мейн
		if (funcmass.find("+][+") == funcmass.end())
			throw std::exception("There are no +][+");
		else
			code = funcmass["+][+"];

		// создаём для него массив
		MainArray = (unsigned char*)calloc(memsize, 1);
		if (MainArray == NULL)
			throw std::exception("Not enough memory to generate a main array");

		unsigned char* mempointer = MainArray;
		const unsigned char* endmempointer = &MainArray[memsize - 1];
		std::vector<std::pair<int, int>> braket = find_pair(code);
		//разбираем мейн
		for (int Noperation = 0; Noperation < code.size(); Noperation++)
		{
			if (funccall && code[Noperation]!='i')
			{
				funccallname += code[Noperation];
				continue;
			}
			switch (code[Noperation])
			{
			case 'i':
				funccall = !funccall;				
				if (funccall == false)
				{
					stackfunc.push({actualfunc,Noperation,MainArray});
					actualfunc = funccallname;
					if (funcmass.find(funccallname) == funcmass.end())
						throw std::exception("There are no called function");
					code = funcmass[funccallname];
					braket = find_pair(code);
					funccallname = "";
					Noperation = -1;
					TempArray = (unsigned char*)calloc(memsize, 1);
					if (TempArray == NULL)
						throw std::exception("Not enough memory to generate a main array");
					count = *mempointer;
					for (int interrator3 = 0; interrator3 < count; interrator3++)
					{
						mempointer++;
						TempArray[interrator3] = *mempointer;
					}
					MainArray = TempArray;
					mempointer = MainArray;
				}
				break;
			case 'r':
				get_info_back = stackfunc.top();
				stackfunc.pop();
				actualfunc = std::get<0>(get_info_back);
				code = funcmass[actualfunc];
				braket = find_pair(code);
				Noperation = std::get<1>(get_info_back);
				TempArray = MainArray;
				MainArray = std::get<2>(get_info_back);
				count = *mempointer;
				for (int interrator4 = 0; interrator4 < count; interrator4 ++)
				{
					mempointer++;
					MainArray[interrator4] = *mempointer;
				}
				free(TempArray);
				mempointer = MainArray;
				break;
			case '+':
				(*mempointer)++;
				break;
			case '-':
				(*mempointer)--;
				break;
			case '>':
				if (mempointer != endmempointer)
					mempointer++;
				else
					throw std::exception("You go out from memory");
				break;
			case '<':
				if (mempointer != MainArray)
					mempointer--;
				else
					throw std::exception("You go out from memory");
				break;
			case '.':
				std::cout << *mempointer;
				break;
			case ',':
				std::cin >> *mempointer;
				break;
			case '[':
				if (*mempointer == 0)
				{
					for (int iternal1 = 0; iternal1 < braket.size(); iternal1++)
					{
						if (braket[iternal1].first == Noperation)
						{
							Noperation = braket[iternal1].second;
							break;
						}
					}
				}

				break;
			case ']':
				for (int iternal2 = 0; iternal2 < braket.size(); iternal2++)
				{
					if (braket[iternal2].second == Noperation)
					{
						Noperation = braket[iternal2].first-1;
						break;
					}
				}
				break;
			}
		}
#ifdef Log
		for (int i = 0; i < memsize; i++)
			std::cout << int(MainArray[i]) << ' ';
		std::cout << std::endl;
#endif 
	}
	catch(std::exception e)
	{
		std::cout << e.what();
	}
}