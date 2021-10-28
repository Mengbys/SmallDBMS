CXX = g++
COMPILEOPT_FLEX_INC = -I ./include/
COMPILEOPT_BOOST_INC = -I D:/boost/include/boost-1_77/
LINKOPT = ./lib/libboost_serialization-mgw8-mt-x64-1_77.a

BISON = ./win_flex_bison3-latest/win_bison.exe
FLEX = ./win_flex_bison3-latest/win_flex.exe

EXE = smalldbms.exe
OBJS = parser.o lexer.o driver.o record.o table.o database.o main.o
CLEANLIST = $(OBJS) \
			sql_parser.tab.cc sql_parser.tab.hh \
			location.hh position.hh \
			stack.hh sql_parser.output \
			sql_lexer.yy.cc $(EXE) 


all: sql

sql: $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(LINKOPT) 

parser.o: sql_parser.yy
	$(BISON) -d -v sql_parser.yy
	$(CXX) -c sql_parser.tab.cc -o parser.o $(COMPILEOPT_FLEX_INC)

lexer.o: sql_lexer.l
	$(FLEX) --outfile=sql_lexer.yy.cc sql_lexer.l
	$(CXX) -c sql_lexer.yy.cc -o lexer.o $(COMPILEOPT_FLEX_INC)

driver.o: sql_driver.cpp sql_driver.hpp
	$(CXX) -c sql_driver.cpp -o driver.o $(COMPILEOPT_FLEX_INC)

record.o: record.cpp record.h
	$(CXX) -c record.cpp -o record.o $(COMPILEOPT_BOOST_INC)

table.o: table.cpp table.h
	$(CXX) -c table.cpp -o table.o $(COMPILEOPT_BOOST_INC)

database.o: database.cpp database.h
	$(CXX) -c database.cpp -o database.o $(COMPILEOPT_BOOST_INC)

main.o: main.cpp record.h table.h database.h sql_driver.hpp
	$(CXX) -c main.cpp -o main.o $(COMPILEOPT_BOOST_INC) $(COMPILEOPT_FLEX_INC)


.PHONY: clean
clean:
	del $(CLEANLIST)

