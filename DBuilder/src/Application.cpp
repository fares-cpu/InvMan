#include<iostream>
#include<dbb.h>

void build_tables() {
	pqxx::work tx = connection::getInstance()->transaction();
	dbb::create_table(std::string("name"),
		std::vector<std::string>({ dbb::id() }),
		tx
	);
}