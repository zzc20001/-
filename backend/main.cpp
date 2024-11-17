#include "database.h"
#include <iostream>
//测试语句
//g++ -std=c++17 -o test_database database.cpp main.cpp -lmysqlcppconn  -I./vcpkg/installed/x64-linux/include
//./test_database
int main() {
    try {
        Database db;
        auto connection = db.getConnection();
        std::cout << "Database connection successful!" << std::endl;
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
