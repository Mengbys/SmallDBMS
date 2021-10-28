#ifndef DATABASE_H
#define DATABASE_H
#include<string>
#include<boost/serialization/access.hpp>

class Database
{
private:
    std::string name;
    friend class boost::serialization::access;
    template <typename Archive>
    friend void serialize(Archive &ar, Database &d, const unsigned int version);
public:
    Database();
    Database(std::string _name);
    Database(const Database&_database);
    std::string getName();
};

bool checkDatabaseExist(std::string name, std::string path);
void createDatabase(std::string name, std::string path);
void dropDatabase(std::string name, std::string path);
void showDatabases(std::string path);


#endif
