#pragma once
#include "INI_Parser.h"

/*
Конструктор, принимающий название INI файла.
В конструктор происходит запись секций и их параметров из открытого файла путём построчного чтения файла.
*/

ini_parser::ini_parser(std::string filename) {
	fin.open(filename);
	if (!fin.is_open()) {
		throw std::runtime_error("file open failed"); // обработка ошибки неисправности файла
	}
	std::string section_name; // название секции
	while (!(fin.eof())) {
		std::string value_name = "empty"; // название параметра
		std::string value = "empty"; // значение параметра
		std::string current_string = "empty"; // текцщая строка файла
		std::getline(fin, current_string);
		// проверка на случай, если в файле между секциями разделение выполнено пробелами
		if (std::count(current_string.begin(), current_string.end(), ' ') == current_string.size()) {
			continue;
		}

		// проверка содержания строки, если условие в скобках выполняется, то в строке содержится 
		// название секции, которое записывается в переменную для дальнейшей работы
		if (current_string.find("[Section") == 0 && current_string.find("]") != std::string::npos) {
			current_string.resize(current_string.find(']') + 1);
			section_name = current_string.substr(1, current_string.size() - 2);
		}
		else {
			// если в строке присутствует комментарий, начинающийся после параметра
			if (current_string.find(";") != std::string::npos && current_string.find(";") != 0 && current_string.at(0) != ' ') {
				current_string.resize(current_string.find(';')); // затираем этот параметр
				value_name = current_string.substr(0, current_string.find('=')); // часть строки до равенства записывается в название переменной 
				value_name.erase(std::remove(value_name.begin(), value_name.end(), ' '), value_name.end()); // в названии параметра не должно быть пробелов
				value = current_string.substr(current_string.find('=') + 1, current_string.size() - value_name.size() - 1); // часть строки после равно записывается в значение параметра
				sections[section_name][value_name] = value; // полученные имена секции и переменной с её значением записываются в словарь
			}
			// если в строке нет комментария после параметра
			else if (current_string.find(";") == std::string::npos) {
				value_name = current_string.substr(0, current_string.find('=')); // часть строки до равенства записывается в название переменной 
				value_name.erase(std::remove(value_name.begin(), value_name.end(), ' '), value_name.end()); // в названии параметра не должно быть пробелов
				value = current_string.substr(current_string.find('=') + 1, current_string.size() - value_name.size() - 1);  // часть строки после равно записывается в значение параметра
				// параметр в секции есть, но у него нет значения, то записвается в значение пустая строка 
				if (value.size() == 0) {
					value = "";
				}
				sections[section_name][value_name] = value; // полученные имена секции и переменной с её значением записываются в словарь
			}
			// если строка в секции начинается с комментария, то пропускаем эту строку
			else {
				continue;
			}
		}
		// может быть случай, что секция существует, но в ней нет параметров, 
		// тогда просто записвается название секции
		if (value_name == "empty") {
			sections[section_name];
		}
	}
}

void ini_parser::ini_parser_error(std::string& input_section_name, std::string& input_value_name) {
	/*
	Обработка ошибки на тот случай, если введено название секции, которого нет в INI файле.
	В случае возникновения ошибки выводится сама ошибка и названия секций, присутствующих в INI файле.
	*/
	if (sections.count(input_section_name) == 0) {
		std::string section_error = "In INI file no input section. Look at sections.\n";
		for (const auto& el1 : sections) {
			section_error += '[' + el1.first + ']' + '\n';
		}
		throw std::runtime_error(section_error);
	}
	else {
		/*
		Обработка ошибки на тот случай, если введено название переменной, которого нет в INI файле.
		В случае возникновения ошибки выводится сама ошибка и переменные, присутствующие в выбранной секции INI файла.
		*/
		if (sections[input_section_name].count(input_value_name) == 0) {
			std::string value_error = "In INI file no input value. Look at [" + input_section_name + "] content.\n[" + input_section_name + "]\n";
			for (const auto& el2 : sections[input_section_name]) {
				value_error += el2.first + '=' + el2.second + '\n';
			}
			throw std::runtime_error(value_error);
		}
	}
}

void ini_parser::ini_parser_type_error(std::string& input_section_name, std::map<std::string, std::map<std::string, std::string>>& sections) {
	std::string type_error = "Invalid value type. Look at [" + input_section_name + "] content.\n[" + input_section_name + "]\n";
	for (const auto& el2 : sections[input_section_name]) {
		type_error += el2.first + '=' + el2.second + '\n';
	}
	throw std::runtime_error(type_error);
}

template<>
int ini_parser::get_value(std::string input_section_value) {
	// обработка входящей строки
	std::string input_section_name = input_section_value.substr(0, // название введенной секции
		input_section_value.find('.')); 
	std::string input_value_name = input_section_value.substr(input_section_value.find('.') + 1, // название введенной переменной
		input_section_value.size() - input_section_name.size() - 1); 
	int result_value; // результирующее значение
	ini_parser_error(input_section_name, input_value_name);
	// обработка ошибки невозможности преобразования искомого значения параметра к типу int
	try {
		result_value = stoi(sections[input_section_name][input_value_name]);
	}
	catch (...) {
		ini_parser_type_error(input_section_name, sections);
	}
	return result_value;
}

template<>
std::string ini_parser::get_value(std::string input_section_value) {
	// обработка входящей строки
	std::string input_section_name = input_section_value.substr(0,  // название введенной секции
		input_section_value.find('.'));
	std::string input_value_name = input_section_value.substr(input_section_value.find('.') + 1,  // название введенной переменной
		input_section_value.size() - input_section_name.size() - 1);
	std::string result_value; // результирующее значение
	ini_parser_error(input_section_name, input_value_name);
	result_value = sections[input_section_name][input_value_name];
	return result_value;
}

template<>
double ini_parser::get_value(std::string input_section_value) {
	// обработка входящей строки
	std::string input_section_name = input_section_value.substr(0, // название введенной секции
		input_section_value.find('.'));
	std::string input_value_name = input_section_value.substr(input_section_value.find('.') + 1, // название введенной переменной
		input_section_value.size() - input_section_name.size() - 1);
	double result_value; // результирующее значение
	ini_parser_error(input_section_name, input_value_name);
	// обработка ошибки невозможности преобразования искомого значения параметра к типу double
	try {
		result_value = stod(sections[input_section_name][input_value_name]);
	}
	catch (...) {
		ini_parser_type_error(input_section_name, sections);
	}
	return result_value;
}

template<>
float ini_parser::get_value(std::string input_section_value) {
	// обработка входящей строки
	std::string input_section_name = input_section_value.substr(0, // название введенной секции
		input_section_value.find('.'));
	std::string input_value_name = input_section_value.substr(input_section_value.find('.') + 1, // название введенной переменной
		input_section_value.size() - input_section_name.size() - 1);
	float result_value; // результирующее значение
	ini_parser_error(input_section_name, input_value_name);
	// обработка ошибки невозможности преобразования искомого значения параметра к типу float
	try {
		result_value = stof(sections[input_section_name][input_value_name]);
	}
	catch (...) {
		ini_parser_type_error(input_section_name, sections);
	}
	return result_value;
}