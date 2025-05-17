#include<iostream>
#include<dbb.h>

void build_tables() {
	pqxx::work tx = connection::getInstance()->transaction();
	dbb::create_table(std::string("customers"),
		std::vector<std::string>({ dbb::id(),
				dbb::string("name"),
				dbb::string("phone"),
				dbb::string("address"),
			}),
		tx
		);
	dbb::create_table(std::string("out bills"),
		std::vector<std::string>({ dbb::id(),
				dbb::string("customer_id"),
				dbb::integer("total_sp"),
				dbb::SMALLinteger("total_usd"),
				dbb::timestamp("date/time")
			}),
		tx
	);
	dbb::create_table(std::string("sells"),
		std::vector<std::string>({ dbb::id(),
				dbb::string("product_id"),
				dbb::integer("quantity"),
				dbb::string("outbill_id"),
				dbb::timestamp("date/time")
			}),
		tx
	);
	dbb::create_table(std::string("products"),
		std::vector<std::string>({ dbb::id(),
				dbb::string("name"),
				dbb::integer("bprice_sp"),
				dbb::SMALLinteger("bprice_usd"),
				dbb::integer("hprice_sp"),
				dbb::SMALLinteger("hprice_usd"),
				dbb::integer("rprice_sp"),
				dbb::SMALLinteger("rprice_usd"),
				dbb::integer("quantity"),
				dbb::numeric(3, 2),
				dbb::string("buy 1 get 1 (:3)"),
			}),
		tx
	);
	dbb::create_table(std::string("in_bills"),
		std::vector<std::string>({ dbb::id(),
				dbb::BIGinteger("total_sp"),
				dbb::integer("total_usd"),
				dbb::string("supplier_id"),
				dbb::timestamp("date/time")
			}),
		tx
	);
	dbb::create_table(std::string("suppliers"),
		std::vector<std::string>({ dbb::id(),
			std::string("name"),
			std::string("phone"),
			dbb::string("address"),
			}),
		tx
	);
	dbb::create_table(std::string("buys"),
		std::vector<std::string>({ dbb::id(),
			dbb::string("product_id"),
				dbb::integer("quantity"),
				dbb::string("inbill_id"),
				dbb::timestamp("date/time")
			}),
		tx
	);