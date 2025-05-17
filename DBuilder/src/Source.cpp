/*
* @author knightu
* whith support from @author ali22uwu
* this program is the database builder, nothing more. 
* it will have many user-interactive edits sooner than expected.
*/
#include<iostream>
#include<dbb.h>

void build_tables() {
	pqxx::work tx = connection::getInstance()->transaction();
	dbb::create_table(std::string("products"),
		std::vector<std::string>({ dbb::id(),
				dbb::string("name"),
				dbb::string("description"),
				dbb::integer("bprice_sp"),
				dbb::numeric("bprice_usd", 7, 2),
				dbb::integer("hprice_sp"),
				dbb::numeric("hprice_usd", 7, 2),
				dbb::integer("rprice_sp"),
				dbb::numeric("rprice_usd", 7, 2),
				dbb::integer("quantity"),
				dbb::numeric("off", 2, 0)
			}),
		tx
	);
	dbb::create_table(std::string("customers"),
		std::vector<std::string>({ dbb::id(),
				dbb::string("name") + " NOT NULL",
				dbb::string("phone")+ " NOT NULL UNIQUE",
				dbb::string("address"),
			}),
		tx
	);
	dbb::create_table(std::string("suppliers"),
		std::vector<std::string>({ dbb::id(),
			dbb::string("name") + " NOT NULL",
			dbb::string("phone")+ " NOT NULL UNIQUE",
			dbb::string("address"),
			}),
		tx
	);
	dbb::create_table(std::string("out_bills"),
		std::vector<std::string>({ dbb::id(),
				dbb::foreign_key("customer"),
				dbb::integer("total_sp"),
				dbb::numeric("total_usd", 7, 2),
				dbb::timestamps()
			}),
		tx
	);
	
	dbb::create_table(std::string("in_bills"),
		std::vector<std::string>({ dbb::id(),
				dbb::integer("total_sp"),
				dbb::numeric("total_usd", 7, 2),
				dbb::foreign_key("supplier"),
				dbb::timestamps()
			}),
		tx
	);
	dbb::create_table(std::string("sells"),
		std::vector<std::string>({ dbb::id(),
				dbb::foreign_key("product"),
				dbb::integer("quantity"),
				dbb::foreign_key("out_bill"),
			}),
		tx
	);

	
	dbb::create_table(std::string("buys"),
		std::vector<std::string>({ dbb::id(),
			dbb::foreign_key("product"),
			dbb::integer("quantity"),
			dbb::foreign_key("in_bill"),
			}),
		tx
	);
	std::string sql_function = R"(
            CREATE OR REPLACE FUNCTION update_updated_at_column()
            RETURNS TRIGGER AS $$
            BEGIN
                NEW.updated_at = NOW();
                RETURN NEW;
            END;
            $$ LANGUAGE plpgsql;
        )";

	std::string sql_trigger = R"(
            CREATE TRIGGER set_updated_at
            BEFORE UPDATE ON in_bills
            FOR EACH ROW
            EXECUTE FUNCTION update_updated_at_column();
        )";
	std::string sql_trigger1 = R"(
            CREATE TRIGGER set_updated_at
            BEFORE UPDATE ON out_bills
            FOR EACH ROW
            EXECUTE FUNCTION update_updated_at_column();
        )";
	tx.exec(sql_function);
	tx.exec(sql_trigger);
	tx.exec(sql_trigger1);
	tx.commit();
}
int main() {
	build_tables();
	return 0;
}