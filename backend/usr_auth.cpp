#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <jwt-cpp/jwt.h>
#include "database.h"
//hash函数加密密码
std::string hashPassword(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), password.size(), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

void handleRegister(const std::string & username, const std::string & password, std::unique_ptr<sql::Connection> &con) {
    try {
        //检测用户名是否已经存在
        std::unique_ptr<sql::PreparedStatement> pstmt;
        pstmt.reset(con->prepareStatement("SELECT * FROM user WHERE username = ?"));
        pstmt->setString(1, username);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            std::cerr << "Username already exists!" << std::endl;
            return;
        }
        //插入新用户
        pstmt.reset(con->prepareStatement("INSERT INTO user (username, password) VALUES (?, ?)"));
        pstmt->setString(1, username);
        pstmt->setString(2, hashPassword(password));
        pstmt->execute();
        std::cout << "User registered successfully!" << std::endl;
    } catch (sql::SQLException &e) {
        std::cerr << "Error while registering user: " << e.what() << std::endl;
    }
}

void handleLogin(const std::string & username, const std::string & password, std::unique_ptr<sql::Connection> &con) {
    try {
        //检测用户名是否存在
        std::unique_ptr<sql::PreparedStatement> pstmt;
        pstmt.reset(con->prepareStatement("SELECT * FROM user WHERE username = ?"));
        pstmt->setString(1, username);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (!res->next()) {
            std::cerr << "Username does not exist!" << std::endl;
            return;
        }
        //检测密码是否正确
        if (res->getString("password") != hashPassword(password)) {
            std::cerr << "Incorrect password!" << std::endl;
            return;
        }
        std::cout << "Login successful!" << std::endl;
    } catch (sql::SQLException &e) {
        std::cerr << "Error while logging in: " << e.what() << std::endl;
    }
}

std::string generateToken(const std::string& username) {
    auto token = jwt::create()
        .set_issuer("your_app")
        .set_subject(username)
        .set_issued_at(std::chrono::system_clock::now())
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::minutes(60))
        .sign(jwt::algorithm::hs256{"your_secret_key"});
    return token;
}