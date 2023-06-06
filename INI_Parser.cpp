#include <iostream>
#include "INI_Parser.h"
#include <Windows.h>

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	ini_parser parser("INI.ini");
	try {
		std::cout << parser.get_value<int>("Section1.var1") << std::endl;
		std::cout << parser.get_value<std::string>("Section1.var1") << std::endl;
		std::cout << parser.get_value<double>("Section1.var1") << std::endl;
		std::cout << parser.get_value<float>("Section1.var1") << std::endl;
	}
	catch (std::exception& ex) {
		std::cout << ex.what();
	}
	return 0;
}