#ifndef TABLE_H
#define TABLE_H
#include<string>
#include<vector>
#include<boost/serialization/access.hpp>

class Table
{
private:
    std::string tableName;
    int numOfColumn;
    int primaryIndex;
    std::vector<std::string> nameOfColumn;
    std::vector<std::string> typeOfColumn;
    friend class boost::serialization::access;
    template <typename Archive>
    friend void serialize(Archive &ar, Table &t, const unsigned int version);
public:
    Table();
    Table(std::string _tableName, int _numOfColumn, int _primaryIndex);
    Table(const Table &t);
    std::string getName();
    int getNumOfColumn();
    int getPrimaryIndex();
    void addColumn(std::string name, std::string type);
    std::string getTypeOfColumnAtPos(int pos);
    std::string getNameOfColumnAtPos(int pos);
};

bool checkTableExist(std::string name, std::string file);
void createTable(std::string name, std::string databaseName, int num, int primary_index, std::string nameOfColumnList[], std::string typeOfColumnList[], std::string path);
void dropTable(std::string name, std::string databaseName, std::string path);
void showTables(std::string databaseName, std::string path);
Table getTable(std::string name, std::string file);

#endif
