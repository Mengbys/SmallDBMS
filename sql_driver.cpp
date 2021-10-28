#include <iostream>
#include <cassert>
#include <istream>
#include "sql_driver.hpp"
#include "sql_parser.tab.hh"

SQL::SQL_Driver::SQL_Driver()
{
    cmd="";
    name="";
    index=0;
    index2=0;
    primary_index=-1;
    have_star=0;
    have_equal=0;
}

SQL::SQL_Driver::~SQL_Driver()
{
    delete(scanner);
    scanner = nullptr;
    delete(parser);
    parser = nullptr;
}

void SQL::SQL_Driver::parse(std::istream &stream)
{
    delete(scanner);
    try
    {
        scanner = new SQL::SQL_Scanner(&stream);
    }
    catch(std::bad_alloc &ba)
    {
        std::cerr << "Failed to allocate scanner: (" <<
            ba.what() << "), exiting!!\n";
        exit( EXIT_FAILURE );
    }

    delete(parser);
    try
    {
        parser = new SQL::SQL_Parser(
                (*scanner) /* scanner */,
                (*this) /* driver */
                );
    }
   catch(std::bad_alloc &ba)
   {
      std::cerr << "Failed to allocate parser: (" << 
         ba.what() << "), exiting!!\n";
      exit( EXIT_FAILURE );
   }

   const int accept(0);
   if( parser->parse() != accept )
   {
       std::cerr << "[ERROR] Parse failed!!\n";
   }
   return;
}

void SQL::SQL_Driver::fill_name_list(std::string &str)
{
    strList1[index2]=str;
    index2++;
}

void SQL::SQL_Driver::fill_value_list(std::string &str)
{
    strList2[index]=str;
    index++;
}

void SQL::SQL_Driver::create_database(std::string &str)
{
    cmd="create_database";
    name=str;
}

void SQL::SQL_Driver::use_database(std::string &str)
{
    cmd="use_database";
    name=str;
}

void SQL::SQL_Driver::drop_database(std::string &str)
{
    cmd="drop_database";
    name=str;
}

void SQL::SQL_Driver::show_databases()
{
    cmd="show_databases";
}

void SQL::SQL_Driver::create_table(std::string &str)
{
    cmd="create_table";
    name=str;
}

void SQL::SQL_Driver::create_table_fill_strList(std::string &str1, std::string &str2)
{
    strList1[index]=str1;
    strList2[index]=str2;
    index++;
}

void SQL::SQL_Driver::create_table_set_primary()
{
    primary_index=index-1;
}

void SQL::SQL_Driver::drop_table(std::string &str)
{
    cmd="drop_table";
    name=str;
}

void SQL::SQL_Driver::show_tables()
{
    cmd="show_tables";
}

void SQL::SQL_Driver::insert_into(std::string &str)
{
    cmd="insert_into";
    name=str;
}

void SQL::SQL_Driver::delete_all_record(std::string &str)
{
    cmd="delete_record";
    name=str;
    index2=1;
}

void SQL::SQL_Driver::delete_some_record(std::string &str1, std::string &str2)
{
    cmd="delete_record";
    name=str1;
    index2=0;
    strList1[0]=str2;
}

void SQL::SQL_Driver::update_record(std::string &str)
{
    cmd="update_record";
    name=str;
}

void SQL::SQL_Driver::select_from_star(std::string &str)
{
    cmd="select_from";
    name=str;
    have_star=1;
}

void SQL::SQL_Driver::select_from_c(std::string &str)
{
    cmd="select_from";
    name=str;
}

void SQL::SQL_Driver::select_from_star_equal(std::string &str)
{
    cmd="select_from";
    name=str;
    have_star=1;
    have_equal=1;
}

void SQL::SQL_Driver::select_from_c_equal(std::string &str)
{
    cmd="select_from";
    name=str;
    have_equal=1;
}
