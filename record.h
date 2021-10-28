#ifndef RECORD_H
#define RECORD_H
#include<string>
#include<vector>
#include<boost/serialization/access.hpp>

class Record
{
private:
    std::vector<std::string> valueList;
    friend class boost::serialization::access;
    template <typename Archive>
    friend void serialize(Archive &ar, Record &r, const unsigned int version);
public:
    Record();
    void addValue(std::string value);
    std::string getValueAtPos(int pos);
    void setValueAtPos(int pos, std::string value);
};

bool checkRecordPrimaryKeyExist(std::string primary_value, int primary_index, std::string file);
bool checkRecordTypeAndValue(int num, std::string typeList[], std::string valueList[]);
void createRecord(int numOfColumn, std::string valueStrList[], std::string file);
int deleteRecord(std::string tableName, std::string path, int index, std::string value);
void deleteAllRecord(std::string tableName, std::string path);
int updateRecord(int numOfSet, int numOfColumn, std::string nameOfSet[], std::string valueOfSet[], std::string nameStrList_all[], std::string file);
void selectFrom_s(int numOfColumn, std::string nameStrList_all[], int primary_index, std::string file);
int selectFrom_s_e(int numOfColumn, std::string nameStrList_all[], int primary_index, std::string equal_name, std::string equal_value, std::string file);
void selectFrom(int numOfSelect, int numOfColumn, std::string nameSelectList[], std::string nameStrList_all[], int primary_index, std::string file);
int selectFrom_e(int numOfSelect, int numOfColumn, std::string nameSelectList[], std::string nameStrList_all[], int primary_index, std::string equal_name, std::string equal_value, std::string file);

#endif
