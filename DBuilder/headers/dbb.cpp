#include<dbb.h>
#include<iostream>


namespace dbb {
	std::string id() {
		return "ID INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY";
	}
	std::string string(const std::string& s) {
		return s+" TEXT";
	}

	/**
	* a creating table function :
	* @param name: table name (const char*)
	* @param tx: active transaction
	* @param num: number of columns
	* @param args (...) columns of the table 
	* 
	* note that after calling `create_table` for all tables in the application, you need to commit them manually
	*/
	bool create_table(const std::string& tablename,const std::vector<std::string>& columns, pqxx::work& tx) {
		std::ostringstream query;
		query << "CREATE TABLE IF NOT EXISTS " << tablename << "( ";

		for (size_t i = 0; i < columns.size(); i++) {
			query << columns[i];
			if (i + 1 < columns.size()) query << ", ";
		}
		query << ");";
		try {
			tx.exec(query.str());
			return true;
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
			return false;
		}
	}
	/**
	* drop table 
	* @param tablename
	* @param tx: active transaction
	*/
	bool drop_table(const std::string& tablename, pqxx::work& tx) {
		try {
			tx.exec((std::string)("DROP TABLE IF EXIST" + tablename));
			tx.commit();
			return true;
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
			return false;
		}
	}
}



connection::connection(){
	try {
		m_conn = new pqxx::connection("dbname=postgres user=postgres password=postgre");
		if (!m_conn->is_open()) m_conn = nullptr;
	}
	catch (const std::exception& e) {
		std::cerr << "Connection failed: " << e.what() << std::endl;
		m_conn = nullptr;
	}
}

void connection::reconnect() {
	if (m_conn) delete m_conn;
	try {
	m_conn = new pqxx::connection("dbname=postgres user=postgres password=postgre");
		if (!m_conn->is_open()) {
			std::cerr << "Connection Failed" << std::endl;
			m_conn = nullptr;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Reconnection Failed: " << e.what() << std::endl;
		m_conn = nullptr;
	}
}

connection* connection::getInstance() {
	std::lock_guard<std::mutex> lock(s_mutex);
	if (!s_instance) s_instance = new connection();
	return s_instance;
}

pqxx::work connection::transaction() {
	if (!m_conn || !m_conn->is_open()) reconnect();
	if (m_conn) return pqxx::work(*m_conn);
	else throw std::runtime_error("Database connection is unavailable");
}
void connection::cleanup() {
	std::lock_guard<std::mutex> lock(s_mutex);
	if (s_instance) {
		delete s_instance;
		s_instance = nullptr;
	}
}

connection::~connection(){
	cleanup();
}

connection* connection::s_instance = nullptr;
std::mutex connection::s_mutex;



