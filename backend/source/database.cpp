#include "database.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <bits/stdc++.h>

Database::Database(std::string host, int port, std::string user, std::string passwd, std::string db) {
    this->host = "tcp://" + host;
    this->port = port;
    this->user = user;
    this->password = passwd;
    this->database = db;
    std::cout << "Database object created!" << std::endl;
}

Database::~Database() {
    std::cout << "Database object destroyed!" << std::endl;
}

std::unique_ptr<sql::Connection> Database::getConnection() {
    try {
        // 获取 MySQL 驱动程序
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        
        // 创建连接
        std::unique_ptr<sql::Connection> con(driver->connect(host, user, password));

        // 选择数据库
        con->setSchema(database);

        std::cout << "Connection to database established successfully!" << std::endl;
        return con;

    } catch (sql::SQLException &e) {
        std::cerr << "Error while connecting to the database: " << e.what() << std::endl;
        throw;  // 如果连接失败，抛出异常
    }
}