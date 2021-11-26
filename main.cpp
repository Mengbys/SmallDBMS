#include"record.h"
#include"table.h"
#include"database.h"
#include"sql_driver.hpp"

#include<sys/stat.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/text_oarchive.hpp>

using namespace std;

int main()
{
    cout << "#############################\n";
    cout << "### Welcome to SMALLDBMS! ###\n";
    cout << "#############################\n";
    // string dirOfDatabase = "D:/workspace/codemagic/place-of-database/"; // must end with slash
    string dirOfDatabase = "./SmallDBMS-place-of-database/";
    mkdir(dirOfDatabase.c_str());
    cout<<"[INFO] Note that SmallDBMS store the data of database at ./SmallDBMS-place-of-database/, you can change it in the \"main.cpp\".\n";

    cout << "[INFO] Please input your sql sentence(press CTRL-C to quit):\n";

    string currentDatabase = "";
    string currentTable = "";
    string cmd = "";
    while(1)
    {

        /* parse a sql sentence */
        SQL::SQL_Driver driver;
        driver.parse(cin);


        cmd = driver.cmd;
        if(cmd == "create_database")
        {
            string databaseName = driver.name;

            if(checkDatabaseExist(databaseName, dirOfDatabase))
            {
                cout << "[ERROR] Database " << databaseName << " is already exist.\n";
                continue;
            }

            createDatabase(databaseName, dirOfDatabase);
            cout << "[INFO] Created a database!\n";
        }
        else if(cmd == "use_database")
        {
            string databaseName = driver.name;

            if(checkDatabaseExist(databaseName, dirOfDatabase))
            {
                currentDatabase = databaseName;
                cout << "[INFO] Selected a database!\n";
            }
            else
                cout << "[ERROR] Database " << databaseName << " is not exist.\n";
        }
        else if(cmd == "drop_database")
        {
            string databaseName = driver.name;

            if(checkDatabaseExist(databaseName, dirOfDatabase))
            {
                dropDatabase(databaseName, dirOfDatabase);
                cout << "[INFO] Dropped a database!\n";
            }
            else
                cout << "[ERROR] Database " << databaseName << " is not exist.\n";
        }
        else if(cmd == "show_databases")
        {
            showDatabases(dirOfDatabase);
            cout << "[INFO] Available databases list!\n";
        }
        else if(cmd == "create_table")
        {
            if(currentDatabase == "")
            {
                cout << "[INFO] Please select a database first.\n";
                continue;
            }

            string tableName = driver.name;
            int numOfColumn = driver.index;
            int primary_index = driver.primary_index;

            string file = dirOfDatabase + currentDatabase + "/" + currentDatabase + ".txt";
            if(checkTableExist(tableName, file))
            {
                cout << "[ERROR] Table " << tableName << " is already exist.\n";
                continue;
            }

            string nameOfColumnList[numOfColumn];
            string typeOfColumnList[numOfColumn];
            for (int i = 0; i < numOfColumn; i++)
            {
                nameOfColumnList[i] = driver.strList1[i];
                typeOfColumnList[i] = driver.strList2[i];
            }

            string path = dirOfDatabase + currentDatabase + "/";
            createTable(tableName, currentDatabase, numOfColumn, primary_index, nameOfColumnList, typeOfColumnList, path);
            cout << "[INFO] Created a table!\n";
        }
        else if(cmd == "drop_table")
        {
            if(currentDatabase == "")
            {
                cout << "[INFO] Please select a database first.\n";
                continue;
            }

            string tableName = driver.name;

            string file = dirOfDatabase + currentDatabase + "/" + currentDatabase + ".txt";
            if(checkTableExist(tableName, file))
            {
                dropTable(tableName, currentDatabase, dirOfDatabase);
                cout << "[INFO] Dropped a table!\n";
            }
            else
                cout << "[ERROR] Table " << tableName << " is not exist.\n";
        }
        else if(cmd == "show_tables")
        {
            if(currentDatabase == "")
            {
                cout << "[INFO] Please select a database first.\n";
                continue;
            }

            showTables(currentDatabase, dirOfDatabase);
            cout << "[INFO] Available tables in database \"" << currentDatabase << "\"!\n";

        }
        else if(cmd=="insert_into")
        {
            // params:
            // type 1(all columns): valueStrList
            // type 2(some columns): columnStrList, valueStrList

            if(currentDatabase == "")
            {
                cout << "[INFO] Please select a database first.\n";
                continue;
            }

            currentTable=driver.name;
            string filexx = dirOfDatabase + currentDatabase + "/" + currentDatabase + ".txt";
            if(!checkTableExist(currentTable, filexx))
            {
                cout << "[ERROR] Table " << currentTable << " is not exist.\n";
                continue;
            }

            string filex = dirOfDatabase + currentDatabase + "/" + currentDatabase + ".txt";
            Table t = getTable(currentTable, filex);
            int numOfColumn=t.getNumOfColumn();
            string columnStrList_all[numOfColumn];
            string typeStrList_all[numOfColumn]; // only use for type check
            string valueStrList_all[numOfColumn];
            for (int i=0; i<numOfColumn; i++)
            {
                columnStrList_all[i]=t.getNameOfColumnAtPos(i);
                typeStrList_all[i]=t.getTypeOfColumnAtPos(i);
            }

            int size=driver.index;
            int primary_index=t.getPrimaryIndex();
            string columnStrList[size];
            string valueStrList[size];
            if(size==numOfColumn) // have all value
            {
                for (int i=0; i<size; i++)
                    valueStrList_all[i]=driver.strList2[i];
            }
            else // have value of some column
            {
                bool check_have_primary_key=0;
                for (int i=0; i<size; i++)
                {
                    columnStrList[i]=driver.strList1[i];
                    // remove primary key check for no primary key table 2021-11-26
                    if(primary_index!=-1 && columnStrList[i]==columnStrList_all[primary_index])
                        check_have_primary_key=1;
                    valueStrList[i]=driver.strList2[i];
                }
                // remove primary key check for no primary key table 2021-11-26
                if(primary_index!=-1 && !check_have_primary_key)
                {
                    cout<<"[ERROR] Missing primary key's value.\n";
                    continue;
                }

                // add column name existence check 2021-11-26
                bool check_names_exist=1;
                for(int i=0; i<size; i++)
                {
                    bool check_this_name_exist=0;
                    for(int j=0; j<numOfColumn; j++)
                    {
                        if(columnStrList[i]==columnStrList_all[j])
                        {
                            check_this_name_exist=1;
                            break;
                        }
                    }
                    if(!check_this_name_exist)
                    {
                        check_names_exist=0;
                        cout<<"[ERROR] No column named "<<columnStrList[i]<<"!\n";
                        break;
                    }
                }
                if(!check_names_exist)
                    continue;

                for (int i=0; i<numOfColumn; i++)
                {
                    valueStrList_all[i]="0";
                    for (int j=0; j<size; j++)
                    {
                        if(columnStrList[j]==columnStrList_all[i])
                        {
                            valueStrList_all[i]=valueStrList[j];
                            break;
                        }
                    }
                }
            }

            if(!checkRecordTypeAndValue(numOfColumn,typeStrList_all,valueStrList_all))
            {
                cout<<"[ERROR] The value input doesn't match the data type!\n";
                continue;
            }

            string file = dirOfDatabase + currentDatabase + "/" + currentTable + ".txt";
            if(primary_index==-1) // no primary key
            {
                createRecord(numOfColumn, valueStrList_all, file);
                cout<<"[INFO] Inserted a record!\n";
            }
            else
            {
                string primary_value = valueStrList_all[primary_index];
                if(checkRecordPrimaryKeyExist(primary_value, primary_index, file))
                    cout << "[ERROR] Primary key " << primary_value << " is already exist.\n";
                else
                {
                    createRecord(numOfColumn, valueStrList_all, file);
                    cout<<"[INFO] Inserted a record!\n";
                }
            }
        }
        else if(cmd == "delete_record")
        {
            // param tableName, columnName, columnValue

            if(currentDatabase == "")
            {
                cout << "[INFO] Please select a database first.\n";
                continue;
            }

            currentTable=driver.name;
            string filexx = dirOfDatabase + currentDatabase + "/" + currentDatabase + ".txt";
            if(!checkTableExist(currentTable, filexx))
            {
                cout << "[ERROR] Table " << currentTable << " is not exist.\n";
                continue;
            }

            string filex = dirOfDatabase + currentDatabase + "/" + currentDatabase + ".txt";
            Table t = getTable(currentTable, filex);


            string columnName=driver.strList1[0];
            string columnValue=driver.strList2[0];
            int deleteall=driver.index2;

            if(deleteall == 1) // delete all record
            {
                string path = dirOfDatabase + currentDatabase + "/";
                deleteAllRecord(currentTable, path);
                cout<<"[INFO] Deleted all record!\n";
                continue;
            }

            int columnNum = t.getNumOfColumn();
            int index = -1;
            for (int i = 0; i < columnNum; i++)
            {
                if(columnName == t.getNameOfColumnAtPos(i))
                {
                    index = i;
                    break;
                }
            }
            if(index == -1)
            {
                cout << "[ERROR] No column named " << columnName << ".\n";
                continue;
            }

            string path = dirOfDatabase + currentDatabase + "/";
            if(deleteRecord(currentTable, path, index, columnValue))
                cout<<"[INFO] Deleted some record!\n";
            else
                cout<<"[ERROR] Can't find match data.\n";

        }
        else if(cmd=="update_record")
        {
            // param:
            // set_name_list, set_value_list
            // equal_name, equal_value

            if(currentDatabase == "")
            {
                cout << "[INFO] Please select a database first.\n";
                continue;
            }

            currentTable=driver.name;
            string filexx = dirOfDatabase + currentDatabase + "/" + currentDatabase + ".txt";
            if(!checkTableExist(currentTable, filexx))
            {
                cout << "[ERROR] Table " << currentTable << " is not exist.\n";
                continue;
            }

            string filex = dirOfDatabase + currentDatabase + "/" + currentDatabase + ".txt";
            Table t = getTable(currentTable, filex);
            
            int numOfColumn=t.getNumOfColumn();
            int primary_index=t.getPrimaryIndex();
            string nameStrList_all[numOfColumn];
            string typeStrList_all[numOfColumn];
            string valueStrList_all[numOfColumn];
            for (int i=0; i<numOfColumn; i++)
            {
                nameStrList_all[i]=t.getNameOfColumnAtPos(i);
                typeStrList_all[i]=t.getTypeOfColumnAtPos(i);
            }

            int numOfSet=driver.index;
            string nameOfSet[numOfSet];
            string valueOfSet[numOfSet];
            string typeOfSet[numOfSet];
            bool have_primary_value=0;
            string primary_value;
            for (int i=0; i<numOfSet; i++)
            {
                nameOfSet[i]=driver.strList1[i];
                valueOfSet[i]=driver.strList2[i];
            }


            // check column name exist
            bool check_names_exist=1;
            for (int i=0; i<numOfSet; i++)
            {
                bool check_this_name_exist=0;
                for (int j=0; j<numOfColumn; j++)
                {
                    if(nameOfSet[i]==nameStrList_all[j])
                    {
                        if(i<numOfSet-1 && j==primary_index)
                        {
                            typeOfSet[i]=typeStrList_all[j];
                            have_primary_value=1;
                            primary_value=valueOfSet[i];
                        }
                        check_this_name_exist=1;
                        break;
                    }
                }
                if(!check_this_name_exist)
                {
                    check_names_exist=0;
                    cout<<"[ERROR] No column named "<<nameOfSet[i]<<"!\n";
                    break;
                }
            }
            if(!check_names_exist)
                continue;

            // check value and data type
            if(!checkRecordTypeAndValue(numOfSet-1,typeOfSet,valueOfSet))
            {
                cout<<"[ERROR] The value input doesn't match the data type!\n";
                continue;
            }

            string file = dirOfDatabase + currentDatabase + "/" + currentTable + ".txt";
            if(primary_index==-1) // no primary key
            {
                if(updateRecord(numOfSet,numOfColumn,nameOfSet,valueOfSet,nameStrList_all,file))
                    cout<<"[INFO] Updated a record!\n";
                else
                    cout<<"[ERROR] Can't find match data!\n";
            }
            else
            {
                if(have_primary_value && checkRecordPrimaryKeyExist(primary_value, primary_index, file))
                    cout << "[ERROR] Primary key " << primary_value << " is already exist.\n";
                else
                {
                    if(updateRecord(numOfSet,numOfColumn,nameOfSet,valueOfSet,nameStrList_all,file))
                        cout<<"[INFO] Updated a record!\n";
                    else
                        cout<<"[ERROR] Can't find match data!\n";
                }
            }
        }
        else if(cmd=="select_from")
        {
            // param:
            // tableName, 
            // nameSelectList, equal_name, equal_value


            if(currentDatabase == "")
            {
                cout << "[INFO] Please select a database first.\n";
                continue;
            }

            currentTable=driver.name;
            string filexx = dirOfDatabase + currentDatabase + "/" + currentDatabase + ".txt";
            if(!checkTableExist(currentTable, filexx))
            {
                cout << "[ERROR] Table " << currentTable << " is not exist.\n";
                continue;
            }

            string filex = dirOfDatabase + currentDatabase + "/" + currentDatabase + ".txt";
            Table t = getTable(currentTable, filex);
            
            int numOfColumn=t.getNumOfColumn();
            int primary_index=t.getPrimaryIndex();
            string nameStrList_all[numOfColumn];
            string typeStrList_all[numOfColumn];
            string valueStrList_all[numOfColumn];
            for (int i=0; i<numOfColumn; i++)
            {
                nameStrList_all[i]=t.getNameOfColumnAtPos(i);
                typeStrList_all[i]=t.getTypeOfColumnAtPos(i);
            }

            string nameSelectList[numOfColumn];
            int numOfSelect=driver.index2;
            for (int i=0; i<numOfSelect; i++)
                nameSelectList[i]=driver.strList1[i];
            string equal_name;
            string equal_value;
            
            // check column name exist
            bool check_names_exist=1;
            for (int i=0; i<numOfSelect; i++)
            {
                bool check_this_name_exist=0;
                for (int j=0; j<numOfColumn; j++)
                {
                    if(nameSelectList[i]==nameStrList_all[j])
                    {
                        check_this_name_exist=1;
                        break;
                    }
                }
                if(!check_this_name_exist)
                {
                    check_names_exist=0;
                    cout<<"[ERROR] No column named "<<nameSelectList[i]<<"!\n";
                    break;
                }
            }
            if(!check_names_exist)
            {
                continue;
            }

            bool have_star=driver.have_star;
            bool have_equal=driver.have_equal;
            string file = dirOfDatabase + currentDatabase + "/" + currentTable + ".txt";
            if(have_star && !have_equal)
            {
                selectFrom_s(numOfColumn,nameStrList_all,primary_index,file);
                cout<<"[INFO] Result of select!\n";
            }
            else if(have_star && have_equal)
            {
                equal_name=driver.strList1[0];
                equal_value=driver.strList2[0];
                if(selectFrom_s_e(numOfColumn,nameStrList_all,primary_index,equal_name,equal_value,file))
                    cout<<"[INFO] Result of select!\n";
                else
                    cout<<"[ERROR] Can't find match data!\n";
            }
            else if(!have_star && !have_equal)
            {
                selectFrom(numOfSelect,numOfColumn,nameSelectList,nameStrList_all,primary_index,file);
                cout<<"[INFO] Result of select!\n";
            }
            else if(!have_star && have_equal)
            {
                equal_name=driver.strList1[numOfSelect-1];
                equal_value=driver.strList2[0];
                if(selectFrom_e(numOfSelect-1,numOfColumn,nameSelectList,nameStrList_all,primary_index,equal_name,equal_value,file))
                    cout<<"[INFO] Result of select!\n";
                else
                    cout<<"[ERROR] Can't find match data!\n";
            }


        }
    }

    return 0;
}
