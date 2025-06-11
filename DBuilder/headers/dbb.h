#pragma once

#include<pqxx\pqxx>
#include<sstream>
#include<string>
#include<mutex>


namespace dbb {
	std::string id();
	std::string string(const std::string& s);
	std::string integer(const std::string& s);
	std::string bigInteger(const std::string& s);
	std::string smallInteger(const std::string& s);
	std::string Float(const std::string& s);
	std::string Float8(const std::string& s);
	std::string real(const std::string& s);
	std::string date(const std::string& s);
	std::string boolean(const std::string& s);
	std::string timestamp(const std::string& s);
	std::string timestamps();
	std::string timestampz(const std::string& s);
	std::string numeric(const std::string& s, int before, int after);
	std::string foreign_key(const std::string& table);

	/**
	* a creating table function :
	* @param name: table name (const char*)
	* @param tx: active transaction
	* @param num: number of columns
	* @param args (...) columns of the table 
	* 
	* note that after calling `create_table` for all tables in the application, you need to commit them manually
	*/
	bool create_table(const std::string& tablename, const std::vector<std::string>& columns, pqxx::transaction_base& tx);
	
	/**
	* drop table 
	* @param tablename
	* @param tx: active transaction
	*/
	bool drop_table(const std::string& tablename, pqxx::transaction_base& tx);

	class Model {
	public:

	};
}


class connection {
private:
	pqxx::connection* m_conn;
	static connection* s_instance;
	static std::mutex s_mutex;

	connection();
	connection(connection& c) = delete;

	void reconnect();
public:
	static connection* getInstance();

	pqxx::work transaction();
	void cleanup();

	~connection();
};




