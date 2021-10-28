#include "record.h"
#include<iostream>
#include<vector>
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

Record::Record() {}

void Record::addValue(string value)
{
    valueList.push_back(value);
}

string Record::getValueAtPos(int pos)
{
    return valueList.at(pos);
}

void Record::setValueAtPos(int pos, string value)
{
    valueList[pos]=value;
}

template <typename Archive>
void serialize(Archive &ar, Record &r, const unsigned int version)
{
    ar & r.valueList;
}

bool checkRecordPrimaryKeyExist(string primary_value, int primary_index, string file)
{
    ifstream infile(file);
    string serialData;
    Record r;
    while(getline(infile, serialData))
    {
        stringstream ss;
        ss << serialData;
        text_iarchive ia(ss);
        ia >> r;
        if(primary_value == r.getValueAtPos(primary_index))
            return 1;
    }

    return 0;
}

bool checkRecordTypeAndValue(int num, string typeList[], string valueList[])
{
    for (int i=0; i<num; i++)
    {
        if(typeList[i]=="int")
        {
            int size=valueList[i].size();
            for (int j=0; j<size; j++)
            {
                if(valueList[i][j]<'0' || valueList[i][j]>'9')
                    return 0;
            }
        }
    }

    return 1;
}

void createRecord(int numOfColumn, string valueStrList[], string file)
{
    Record record0;
    for (int i = 0; i < numOfColumn; i++)
    {
        record0.addValue(valueStrList[i]);
    }

    ofstream ofile(file, ios::app);
    text_oarchive oa(ofile);
    oa << record0;
    ofile << "\n";
    ofile.close();
}

int deleteRecord(string tableName, string path, int index, string value)
{
    string tableFile=path+tableName+".txt";
    string tableFileTmp=path+"tmp.txt";

    ifstream ifile(tableFile);
    ofstream ofile(tableFileTmp);
    string serialData;
    Record r;

    int get_match=0;
    while(getline(ifile,serialData))
    {
        stringstream ss;
        ss<<serialData;
        text_iarchive ia(ss);
        ia>>r;

        if(r.getValueAtPos(index)!=value)
        {
            ofile<<serialData<<"\n";
        }
        else
            get_match=1;
    }
    ifile.close();
    ofile.close();

    remove(tableFile.c_str());
    rename(tableFileTmp.c_str(), tableFile.c_str());

    return get_match;
}

void deleteAllRecord(string tableName, string path)
{
    string tableFile=path+tableName+".txt";
    string tableFileTmp=path+"tmp.txt";

    ofstream ofile(tableFileTmp);
    ofile.close();

    remove(tableFile.c_str());
    rename(tableFileTmp.c_str(), tableFile.c_str());
}

int updateRecord(int numOfSet,int numOfColumn,string nameOfSet[],string valueOfSet[],string nameStrList_all[],string file)
{
    string tmp_file=file+".tmp";

    string equal_name=nameOfSet[numOfSet-1];
    string equal_value=valueOfSet[numOfSet-1];
    int equal_index;
    for (int i=0; i<numOfColumn; i++)
    {
        if(equal_name==nameStrList_all[i])
        {
            equal_index=i;
            break;
        }
    }

    ifstream ifile(file);
    ofstream ofile(tmp_file);
    string serialData;
    Record r;
    int get_match=0;
    while(getline(ifile,serialData))
    {
        stringstream ss;
        ss<<serialData;
        text_iarchive ia(ss);
        ia>>r;

        if(r.getValueAtPos(equal_index)==equal_value)
        {
            get_match=1;
            for (int i=0; i<numOfSet-1; i++)
            {
                for (int j=0; j<numOfColumn; j++)
                {
                    if(nameOfSet[i]==nameStrList_all[j])
                    {
                        r.setValueAtPos(j,valueOfSet[i]);
                    }
                }
            }
        }

        text_oarchive oa(ofile);
        oa<<r;
        // ofile<<"\n"; // what happen? insert into another eol
    }
    ifile.close();
    ofile.close();

    remove(file.c_str());
    rename(tmp_file.c_str(), file.c_str());

    return get_match;
}

void selectFrom_s(int numOfColumn,string nameStrList_all[],int primary_index,string file)
{
    // print column names
    string primary_key=nameStrList_all[primary_index];
    for (int i=0; i<numOfColumn; i++)
    {
        if(nameStrList_all[i]==primary_key)
            cout<<"*"<<primary_key<<"\t";
        else
            cout<<nameStrList_all[i]<<"\t";
    }
    cout<<"\n";

    ifstream ifile(file);
    string serialData;
    Record r;
    while(getline(ifile,serialData))
    {
        stringstream ss;
        ss<<serialData;
        text_iarchive ia(ss);
        ia>>r;

        for (int i=0; i<numOfColumn; i++)
            cout<<r.getValueAtPos(i)<<"\t";
        cout<<"\n";
    }
}

int selectFrom_s_e(int numOfColumn,string nameStrList_all[],int primary_index,string equal_name,string equal_value,string file)
{
    // print column names
    string primary_key=nameStrList_all[primary_index];
    int equal_index;
    for (int i=0; i<numOfColumn; i++)
    {
        if(nameStrList_all[i]==primary_key)
            cout<<"*"<<primary_key<<"\t";
        else
            cout<<nameStrList_all[i]<<"\t";

        if(nameStrList_all[i]==equal_name)
            equal_index=i;
    }
    cout<<"\n";

    ifstream ifile(file);
    string serialData;
    Record r;
    int get_match=0;
    while(getline(ifile,serialData))
    {
        stringstream ss;
        ss<<serialData;
        text_iarchive ia(ss);
        ia>>r;

        if(r.getValueAtPos(equal_index)==equal_value)
        {
            for (int i=0; i<numOfColumn; i++)
                cout<<r.getValueAtPos(i)<<"\t";
            cout<<"\n";
            get_match=1;
        }
    }
    
    return get_match;
}

void selectFrom(int numOfSelect, int numOfColumn, string nameSelectList[], string nameStrList_all[], int primary_index, string file)
{
    // print column names
    string primary_key=nameStrList_all[primary_index];
    for (int i=0; i<numOfSelect; i++)
    {
        if(nameSelectList[i]==primary_key)
            cout<<"*"<<primary_key<<"\t";
        else
            cout<<nameSelectList[i]<<"\t";
    }
    cout<<"\n";

    int indexSelectList[numOfSelect];
    for (int i=0; i<numOfColumn; i++)
    {
        for (int j=0; j<numOfSelect; j++)
        {
            if(nameSelectList[j]==nameStrList_all[i])
                indexSelectList[j]=i;
        }
    }

    ifstream ifile(file);
    string serialData;
    Record r;
    while(getline(ifile,serialData))
    {
        stringstream ss;
        ss<<serialData;
        text_iarchive ia(ss);
        ia>>r;

        for (int i=0; i<numOfSelect; i++)
            cout<<r.getValueAtPos(indexSelectList[i])<<"\t";
        cout<<"\n";
    }

}

int selectFrom_e(int numOfSelect, int numOfColumn, string nameSelectList[], string nameStrList_all[], int primary_index, string equal_name, string equal_value, string file)
{
    // print column names
    string primary_key=nameStrList_all[primary_index];
    for (int i=0; i<numOfSelect; i++)
    {
        if(nameSelectList[i]==primary_key)
            cout<<"*"<<primary_key<<"\t";
        else
            cout<<nameSelectList[i]<<"\t";
    }
    cout<<"\n";

    int equal_index;
    int indexSelectList[numOfSelect];
    for (int i=0; i<numOfColumn; i++)
    {
        if(equal_name==nameStrList_all[i])
            equal_index=i;
    }
    for (int i=0; i<numOfColumn; i++)
    {
        for (int j=0; j<numOfSelect; j++)
        {
            if(nameSelectList[j]==nameStrList_all[i])
                indexSelectList[j]=i;
        }
    }

    ifstream ifile(file);
    string serialData;
    Record r;
    int get_match=0;
    while(getline(ifile,serialData))
    {
        stringstream ss;
        ss<<serialData;
        text_iarchive ia(ss);
        ia>>r;

        if(r.getValueAtPos(equal_index)==equal_value)
        {
            for (int i=0; i<numOfSelect; i++)
                cout<<r.getValueAtPos(indexSelectList[i])<<"\t";
            cout<<"\n";
            get_match=1;
        }
    }
    
    return get_match;
}
