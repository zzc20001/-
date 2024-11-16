#include "database.h"
#include <iostream>

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
