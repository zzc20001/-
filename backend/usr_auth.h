#ifndef AUTH_H
#define AUTH_H
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <jwt-cpp/jwt.h>
#include "database.h"
#include <memory>
#include <string>
#include <cppconn/connection.h> // 声明 std::unique_ptr<sql::Connection>
#include<crow.h>
// 声明函数
std::string hashPassword(const std::string& password);

bool handleRegister(const std::string& username, const std::string& password, std::unique_ptr<sql::Connection>& con);

crow::response handleLogin(const std::string & username, const std::string & password, std::unique_ptr<sql::Connection> &con) ;

std::string generateToken(const std::string& username);

#endif // AUTH_H
