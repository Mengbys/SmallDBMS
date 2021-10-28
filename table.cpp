#include"table.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<sys/stat.h>
#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/text_oarchive.hpp>
#include<boost/serialization/access.hpp>
#include<boost/serialization/vector.hpp>
#include<boost/serialization/string.hpp>

using namespace std;
using namespace boost::archive;

Table::Table() {}

Table::Table(string _tableName, int _numOfColumn, int _primaryIndex)
{
    tableName = _tableName;
    numOfColumn = _numOfColumn;
    primaryIndex = _primaryIndex;
}

Table::Table(const Table &t)
{
    tableName = t.tableName;
    numOfColumn = t.numOfColumn;
    primaryIndex = t.primaryIndex;
    nameOfColumn = t.nameOfColumn;
    typeOfColumn = t.typeOfColumn;
}

string Table::getName()
{
    return tableName;
}

int Table::getNumOfColumn()
{
    return numOfColumn;
}

int Table::getPrimaryIndex()
{
    return primaryIndex;
}

void Table::addColumn(string name, string type)
{
    nameOfColumn.push_back(name);
    typeOfColumn.push_back(type);
}

string Table::getTypeOfColumnAtPos(int pos)
{
    return typeOfColumn.at(pos);
}

string Table::getNameOfColumnAtPos(int pos)
{
    return nameOfColumn.at(pos);
}

template <typename Archive>
void serialize(Archive &ar, Table &t, const unsigned int version)
{
    ar & t.tableName & t.numOfColumn & t.primaryIndex & t.nameOfColumn & t.typeOfColumn;
}

bool checkTableExist(string name, string file)
{
    ifstream infile(file);
    // DATABASE_NAME.txt doesn't exist
    if(!infile.good())  return 0;
    string serialData;
    Table t;
    while(getline(infile, serialData))
    {
        stringstream ss;
        ss << serialData;
        text_iarchive ia(ss);
        ia >> t;
        if(t.getName() == name)
            return 1;
    }

    return 0;
}

void createTable(string name, string databaseName, int num, int primary_index, string nameOfColumnList[], string typeOfColumnList[], string path)
{
    Table table0(name, num, primary_index);
    for (int i = 0; i < num; i++)
    {
        table0.addColumn(nameOfColumnList[i], typeOfColumnList[i]);
    }

    // create TABLE_NAME.txt
    ofstream outfile;
    outfile.open(path+name+".txt");
    outfile.close();

    // write to DATABASE_NAME.txt(create when no exist)
    outfile.open(path+databaseName+".txt", ios::app);
    // write serialization data
    text_oarchive oa(outfile);
    oa << table0;
    outfile << "\n";
    outfile.close();
}

void dropTable(string name, string databaseName, string path)
{
    string tableFile=path+databaseName+"/"+name+".txt";
    string databaseFile=path+databaseName+"/"+databaseName+".txt";
    string databaseFileTmp=path+databaseName+"/"+"tmp.txt";


    remove(tableFile.c_str());

    ifstream ifile(databaseFile);
    ofstream ofile(databaseFileTmp);
    stringstream ss;
    string serialData;
    Table t;
    while(getline(ifile, serialData))
    {
        ss << serialData;
        text_iarchive ia(ss);
        ia >> t;
        if(t.getName()!=name)
            ofile<<serialData<<"\n";
    }
    ifile.close();
    ofile.close();

    remove(databaseFile.c_str());
    rename(databaseFileTmp.c_str(), databaseFile.c_str());
}

void showTables(string databaseName, string path)
{
    string databaseFile=path+databaseName+"/"+databaseName+".txt";

    ifstream ifile(databaseFile);
    stringstream ss;
    string serialData;
    Table t;
    while(getline(ifile, serialData))
    {
        ss << serialData;
        text_iarchive ia(ss);
        ia >> t;

        cout<<t.getName()<<endl;
    }
}

Table getTable(string name, string file)
{
    ifstream ifile(file);
    stringstream ss;
    string serialData;
    Table t;
    while(getline(ifile, serialData))
    {
        ss << serialData;
        text_iarchive ia(ss);
        ia >> t;
        if(name == t.getName())
            return t;
    }
    return t;
}
