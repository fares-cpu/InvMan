#include<iostream>
#include<pqxx\pqxx>

class Model {
	
};
struct Condition {
	std::string colname;
	std::string op = "=";
	std::string value;

	Condition(std::string con) {

	}
}const;

class Product {
public:
	int			id;
	std::string name;
	std::string company;
	std::string description;
	float		bprice_usd;
	int			bprice_sp;
	float		hprice_usd;
	int			hprice_sp;
	float		rprice_usd;
	int			rprice_sp;
	int			off;
	int			quantity;

	static bool insert(std::string name,
	std::string description,
	std::string company,
	float		bprice_usd,
	int			bprice_sp,
	float		hprice_usd,
	int			hprice_sp,
	float		rprice_usd,
	int			rprice_sp,
	int			off,
	int			quantity,
	pqxx::transaction_base& tx)
	{
		pqxx::result r = tx.exec_prepared("product.insert" , name, company,
			description, bprice_usd, bprice_sp, hprice_usd, hprice_sp, rprice_usd, rprice_sp, off, quantity);
		return r.affected_rows() == 1;
	}
	static Product find(std::string& id, pqxx::transaction_base& tx) {
		return Product(tx.exec_prepared("product.find", id).one_row());
	}
	static std::vector<Product> where(const std::string& column_name, const std::string& value, pqxx::transaction_base& tx, const std::string& op="=") {
		pqxx::result r = tx.exec_params("select * from products where $1 $2 $3", column_name, op, value);
		return Product::collection(r);
	}
	void update(const std::vector<std::pair<std::string, std::string>> colval ,pqxx::transaction_base& tx) {
		std::string query = "update products set ";
		pqxx::params params;
		for (auto i = 0; i < colval.size(); i++) {
			query = query + colval[i].first + " = " + "$" + std::to_string(i+2);
			if (i != colval.size() - 1) query += ", ";
			params.append(colval[i].second);
		}
		query = query + " where id = $1";
		params.append(this->id);
		for (std::vector<Product>::size_type i = 0; i < colval.size(); ++i)
			
		tx.exec(query, params);
	}
	bool del(pqxx::transaction_base& tx) {
		pqxx::result r = tx.exec_params("delete from products where id = $1", this->id);
		return r.affected_rows() == 1;
	}
	static std::vector<Product> filter(pqxx::transaction_base& tx, const std::vector<Condition>& conditions) {
		pqxx::params params;
		std::string query = "select * from products where ";
		for (size_t i = 0; i < conditions.size(); ++i) {
			query += conditions[i].colname + " " + conditions[i].op + " $" + std::to_string(i) + " ";
			params.append(conditions[i].value);
			if (i < conditions.size() - 1) query += "and ";
		}
		query += ";";
		tx.exec(query, params);
	}
	

private:
	Product(const pqxx::row& r) {
		id = r[0].as<int>();
		name = r[1].as<std::string>();
		company = r[2].as<std::string>();
		description = r[3].as<std::string>();
		bprice_usd = r[4].as<float>();
		bprice_sp = r[5].as<int>();
		hprice_usd = r[6].as<float>();
		hprice_sp = r[7].as<int>();
		rprice_usd = r[8].as<float>();
		rprice_sp = r[9].as<int>();
		off = r[10].as<int>();
		quantity = r[11].as<int>();
	}
	static std::vector<Product> collection(const pqxx::result& r) {
		std::vector<Product> coll;
		for (auto i = 0; i < r.size(); ++i) {
			coll.emplace_back(Product(r[i]));
		}
		return coll;
	}

};

class Customer {
public:
	int id;
	std::string name;
	std::string phone;
	std::string address;

	static bool insert(const std::string& name, const std::string& phone, const std::string& address, pqxx::transaction_base& tx){
		pqxx::result r = tx.exec_prepared("customer.insert", name, phone, address);
		return r.affected_rows() == 1;
	}

	static Customer find(const std::string& id, pqxx::transaction_base& tx) {
		return Customer(tx.exec_prepared("customer.find", id).one_row());
	}

	static std::vector<Customer> where(const std::string& column_name, const std::string& value, pqxx::transaction_base& tx, const std::string& op="=") {
		pqxx::result r = tx.exec_params("select * from customers where $1 $2 $3", column_name, value);
		return Customer::collection(r);
	}

	bool del(pqxx::transaction_base& tx) {
		pqxx::result r = tx.exec_params("delete from customers where id = $1", this->id);
		return r.affected_rows() == 1;
	}
	void update(const std::vector<std::pair<std::string, std::string>> colval, pqxx::transaction_base& tx) {
		std::string query = "update customers set ";
		for (auto i = 0; i < colval.size(); ++i) {
			query = query + colval[i].first + " = " + "$" + std::to_string(i + 2);
			if (i != colval.size() - 1) query += ", ";
		}
		query = query + " where id = $1";
		pqxx::params params;
		params.append(this->id);
		for (auto i = 0; i < colval.size(); i++)
			params.append(colval[i].second);
		tx.exec(query, params);
	}
	static std::vector<Customer> filter(pqxx::transaction_base& tx, const std::vector<Condition>& conditions) {
		pqxx::params params;
		std::string query = "select * from customers where ";
		for (size_t i = 0; i < conditions.size(); ++i) {
			query += conditions[i].colname + " " + conditions[i].op + " $" + std::to_string(i) + " ";
			params.append(conditions[i].value);
			if (i < conditions.size() - 1) query += "and ";
		}
		query += ";";
		tx.exec(query, params);
	}
	

private:
	Customer(const pqxx::row& r) {
		id = r["id"].as<int>();
		name = r["name"].as<std::string>();
		phone = r["phone"].as<std::string>();
		address = r["address"].as<std::string>();
	}

	static std::vector<Customer> collection(pqxx::result r) {
		std::vector<Customer> coll;
		for (auto i = 0; i < r.size(); i++) {
			coll.emplace_back(Customer(r[i]));
		}
		return coll;
	}
};
//expected behaviour:
