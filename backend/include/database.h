#ifndef DATABASE_H
#define DATABASE_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <memory>
#include <string>

class Database {
    public: 
        Database();
        ~Database();
        std::unique_ptr<sql::Connection> getConnection();

    private:
        std::string host;
        int port;
        std::string user;
        std::string password;
        std::string database;
};
#endif