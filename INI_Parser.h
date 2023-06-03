#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

class ini_parser {
public:
	ini_parser(std::string);

	ini_parser(ini_parser&) = delete;
	ini_parser(ini_parser&&) = delete;
	ini_parser& operator=(ini_parser&) = delete;
	ini_parser& operator=(ini_parser&&) = delete;


	template<class T>
	T get_value(std::string);

	~ini_parser() {
		fin.close();
	}

private:
	std::ifstream fin;
	std::map<std::string, std::map<std::string, std::string>> sections;
	void ini_parser_error(std::string&, std::string&);
};


