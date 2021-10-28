#include"table.h"
#include"database.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<sys/stat.h>
#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/text_oarchive.hpp>
#include<boost/serialization/access.hpp>

using namespace std;
using namespace boost::archive;
using namespace boost::archive;

Database::Database() {}

Database::Database(string _name)
{
    name = _name;
}

Database::Database(const Database &_database)
{
    name = _database.name;
}

string Database::getName()
{
    return name;
}

template <typename Archive>
void serialize(Archive &ar, Database &d, const unsigned int version)
{
    ar & d.name;
}

bool checkDatabaseExist(string name, string path)
{
    ifstream infile(path + "database-list.txt");
    // base-list.txt doesn't exist
    if(!infile.good())  return 0;
    string serialData;
    Database d;
    while(getline(infile, serialData))
    {
        stringstream ss;
        ss << serialData;
        text_iarchive ia(ss);
        ia >> d;
        if(d.getName() == name)
            return 1;
    }

    return 0;
}

void createDatabase(string name, string path)
{
    Database database0(name);

    // create database directory
    string pathStr = path + name;
    mkdir(pathStr.c_str());

    // create DATABASE_NAME.txt
    ofstream outfile;
    outfile.open(pathStr + "/" + name + ".txt");
    outfile.close();

    // write to base-list.txt(create when no exist)
    outfile.open(path + "database-list.txt", ios::app);
    // write serialization data
    text_oarchive oa(outfile);
    oa << database0;
    outfile << "\n";
    outfile.close();
}

void dropDatabase(string name, string path)
{
    string dirPath=path+name+"/";
    string databaseListTxtFile=path+"database-list.txt";
    string databaseListTxtFileTmp=path+"tmp.txt";
    string databaseTxtFile=dirPath+name+".txt";
    string tableTxtFile;

    ifstream ifile(databaseTxtFile);
    string serialData;
    Table t;
    while(getline(ifile, serialData))
    {
        stringstream ss;
        ss << serialData;
        text_iarchive ia(ss);
        ia >> t;

        tableTxtFile=dirPath+t.getName()+".txt";
        remove(tableTxtFile.c_str());
    }
    ifile.close();

    remove(databaseTxtFile.c_str());
    rmdir(dirPath.c_str());

    ofstream ofile(databaseListTxtFileTmp, ios::app);
    ifile.open(databaseListTxtFile);
    Database d;
    while(getline(ifile, serialData))
    {
        stringstream ss;
        ss << serialData;
        text_iarchive ia(ss);
        ia >> d;
        if(d.getName()!=name)
            ofile<<serialData<<"\n";
    }
    ifile.close();
    ofile.close();

    remove(databaseListTxtFile.c_str());
    rename(databaseListTxtFileTmp.c_str(), databaseListTxtFile.c_str());
}

void showDatabases(string path)
{
    string databaseListTxtFile=path+"database-list.txt";

    ifstream ifile(databaseListTxtFile);
    stringstream ss;
    string serialData;
    Database d;
    while(getline(ifile, serialData))
    {
        ss << serialData;
        text_iarchive ia(ss);
        ia >> d;

        cout<<d.getName()<<endl;
    }
}
