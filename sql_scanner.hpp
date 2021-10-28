#ifndef __SQLSCANNER_HPP__
#define __SQLSCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "sql_parser.tab.hh"
#include "location.hh"

namespace SQL{

class SQL_Scanner : public yyFlexLexer{
public:
   
   SQL_Scanner(std::istream *in) : yyFlexLexer(in)
   {
   };
   virtual ~SQL_Scanner() {
   };

   //get rid of override virtual function warning
   using FlexLexer::yylex;

   virtual
   int yylex( SQL::SQL_Parser::semantic_type * const lval, 
              SQL::SQL_Parser::location_type *location );
   // YY_DECL defined in SQL_lexer.l
   // Method body created by flex in sql_lexer.yy.cc


private:
   /* yyval ptr */
   SQL::SQL_Parser::semantic_type *yylval = nullptr;
};

} /* end namespace SQL */

#endif // end __SQLSCANNER_HPP__


