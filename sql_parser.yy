%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {SQL}

%define api.parser.class {SQL_Parser}

%code requires{
    namespace SQL {
        class SQL_Driver;
        class SQL_Scanner;
    }

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif
}

%parse-param { SQL_Scanner  &scanner  }
%parse-param { SQL_Driver  &driver  }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   
   /* include for all driver functions */
   #include "sql_driver.hpp"

#undef yylex
#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%token SEMICOLON 0 "end of input"
%token CREATE USE DROP SHOW TABLE TABLES DATABASE DATABASES PRIMARYKEY INSERTINTO DELETE SELECT FROM START WHERE UPDATE SET EQUAL VALUES OBRACE EBRACE COMMA;
%token <std::string> TYPEINT
%token <std::string> TYPECHAR
%token <std::string> NAME
%token <std::string> VALUESTRING

%locations

%%
sql_command:
create_database 
| use_database
| drop_database
| show_databases
| create_table
| drop_table
| show_tables
| insert_into
| delete_record
| update_record
| select_from
;

/* database operation */
/* 1. create database */
create_database:
CREATE DATABASE NAME
{
    driver.create_database($3);
}
;

/* 2. use database */
use_database:
USE NAME
{
    driver.use_database($2);
}
;

/* 3. drop database */
drop_database:
DROP DATABASE NAME
{
    driver.drop_database($3);
}

/* 4. show databases */
show_databases:
SHOW DATABASES
{
    driver.show_databases();
}

/* table operation */
/* 1. create table */
create_table:
CREATE TABLE NAME OBRACE columns EBRACE 
{
    driver.create_table($3);
}
;

columns:
column
| columns COMMA column
;

column:
name_type
| name_type PRIMARYKEY
{
    driver.create_table_set_primary();
}
;

name_type:
NAME TYPEINT
{
    driver.create_table_fill_strList($1, $2);
}
| NAME TYPECHAR
{
    driver.create_table_fill_strList($1, $2);
}
;

/* 2. drop table */
drop_table:
DROP TABLE NAME
{
     driver.drop_table($3);
}
;

/* 3. show tables */
show_tables:
SHOW TABLES
{
    driver.show_tables();
}

/* record operation */
/* 1. insert record into table */
insert_into:
INSERTINTO NAME VALUES OBRACE insert_values EBRACE
{
    driver.insert_into($2);
}
| INSERTINTO NAME OBRACE insert_columns EBRACE VALUES OBRACE insert_values EBRACE
{
    driver.insert_into($2);
}
;

insert_columns:
insert_column
| insert_columns COMMA insert_column
;

insert_column:
NAME
{
    driver.fill_name_list($1);
}
;

insert_values:
insert_value
| insert_values COMMA insert_value
;

insert_value:
NAME
{
    driver.fill_value_list($1);
}
| VALUESTRING
{
    driver.fill_value_list($1);
}
;

/* 2. delete record */
delete_record:
DELETE FROM NAME
{
    driver.delete_all_record($3);
}
| DELETE FROM NAME WHERE NAME EQUAL insert_value
{
    driver.delete_some_record($3, $5);
}
| DELETE START FROM NAME
{
    driver.delete_all_record($4);
}
;

/* 3. update record */
update_record:
UPDATE NAME SET set_statements WHERE insert_column EQUAL insert_value
{
    driver.update_record($2);
}
;

set_statements:
set_statement
| set_statements COMMA set_statement
;

set_statement:
NAME EQUAL insert_value
{
    driver.fill_name_list($1);
}
;

/* 4. select records from table */
select_from:
SELECT START FROM NAME 
{
    driver.select_from_star($4);
}
| SELECT insert_columns FROM NAME
{
    driver.select_from_c($4);
}
| SELECT START FROM NAME WHERE insert_column EQUAL insert_value
{
    driver.select_from_star_equal($4);
}
| SELECT insert_columns FROM NAME WHERE insert_column EQUAL insert_value
{
    driver.select_from_c_equal($4);
}
%%

void SQL::SQL_Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "[Error] " << err_message << " at " << l << "\n";
}
