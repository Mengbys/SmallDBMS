#include <istream>
#ifndef __SQLDRIVER_HPP__
#define __SQLDRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <iostream>

#include "sql_scanner.hpp"
#include "sql_parser.tab.hh"

namespace SQL{
    class SQL_Driver{
        public:
            SQL_Driver();
            virtual ~SQL_Driver();
            void fill_name_list(std::string &str);
            void fill_value_list(std::string &str);
            void create_database(std::string &str);
            void use_database(std::string &str);
            void drop_database(std::string &str);
            void show_databases();
            void create_table(std::string &str);
            void create_table_fill_strList(std::string &str1, std::string &str2);
            void create_table_set_primary();
            void drop_table(std::string &str);
            void show_tables();
            void insert_into(std::string &str);
            void delete_all_record(std::string &str);
            void delete_some_record(std::string &str1, std::string &str22);
            void update_record(std::string &str);
            void select_from_star(std::string &str);
            void select_from_c(std::string &str);
            void select_from_star_equal(std::string &str);
            void select_from_c_equal(std::string &str);

            void parse(std::istream &stream);

            std::string cmd;
            std::string name;
            int index;
            int index2;
            int primary_index;
            bool have_star;
            bool have_equal;
            std::string strList1[100];
            std::string strList2[100];

        private:
            SQL::SQL_Parser *parser = nullptr;
            SQL::SQL_Scanner *scanner = nullptr;

    };
}

#endif

