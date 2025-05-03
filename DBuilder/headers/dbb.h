#include<iostream>
#include<pqxx\pqxx>
#include<cstdarg>
#include<sstream>
#include<string>
#pragma once
//connection and transaction construction
pqxx::connection cx();
pqxx::work tx(cx);

namespace dbb {
	std::string id() {
		return "ID INT AUTO INCREMENT PRIMARY KEY";
	}
	std::string string(std::string& s) {
		return s + " TEXT";
	}

	/**
	* a creating table function :
	* @param name: table name (const char*)
	* @param num: number of columns
	* @param args (...) columns of the table 
	*/
	bool create_table(const char*& name, int num, ...) {
		va_list arguments;
		std::vector<const char*> args(10);
		va_start(arguments, num);
		std::ostringstream query;
		query << "CREATE TABLE IF NOT EXISTS" << name << "(";
		for (int i = 0; i < num; i++) {
			query << va_arg(arguments, const char*) << ", ";
		}
		query << ");";
		try {
			tx.exec(query.str());
			return true;
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
			return false;
		}
	}
};