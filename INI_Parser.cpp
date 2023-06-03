#include <iostream>
#include "INI_Parser.h"
#include <Windows.h>

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	ini_parser parser("INI.txt");
	try {
		auto value = parser.get_value<int>("Section1.var1");
		std::cout << value;
	}
	catch (std::exception& ex) {
		std::cout << ex.what();
	}
	return 0;
}