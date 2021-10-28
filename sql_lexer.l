%{
#include <string>

/* implementation of yyFlexScanner */
#include "sql_scanner.hpp"
#undef  YY_DECL
#define YY_DECL int SQL::SQL_Scanner::yylex( SQL::SQL_Parser::semantic_type * const lval, SQL::SQL_Parser::location_type *loc )

/* make name shorter */
using token = SQL::SQL_Parser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return(token::SEMICOLON)

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

%option debug
%option nodefault
%option yyclass = "SQL::SQL_Scanner"
%option noyywrap
%option c++
%option caseless

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

create           { return(token::CREATE); }
use              { return(token::USE); }
drop             { return(token::DROP); }
show             { return(token::SHOW); }
table            { return(token::TABLE); }
tables           { return(token::TABLES); }
database         { return(token::DATABASE); }
databases        { return(token::DATABASES); }
primary[ \n]+key { return(token::PRIMARYKEY); }
insert[ \n]+into { return(token::INSERTINTO); }
delete           { return(token::DELETE); }
select           { return(token::SELECT); }
from             { return(token::FROM); }
\*               { return(token::START); }
where            { return(token::WHERE); }
\=               { return(token::EQUAL); }
update           { return(token::UPDATE); }
set              { return(token::SET); }
values           { return(token::VALUES); }
\(               { return(token::OBRACE); }
\)               { return(token::EBRACE); }
,                { return(token::COMMA); }
;                { return(token::SEMICOLON); }


int {
    yylval->build<std::string>(yytext); return(token::TYPEINT);
}

varchar {
    yylval->build<std::string>(yytext); return(token::TYPECHAR);
}

[a-z][a-z0-9]* {
    yylval->build<std::string>(yytext); return(token::NAME);
}

[a-z0-9]+ {
    yylval->build<std::string>(yytext); return(token::VALUESTRING);
}


\n /* ignore end of line */
[ ]+ /* ignore whitespace */
%%
