#include <cppconn/prepared_statement.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <fstream>
#include <iostream>

void handleUploadproduct(const std::string & name, const std::string & price, const std::string & description, const std::string & image, std::unique_ptr<sql::Connection> &con) {
    try {
        //插入新商品
        std::unique_ptr<sql::PreparedStatement> pstmt;
        pstmt.reset(con->prepareStatement("INSERT INTO product (name, price, description, image) VALUES (?, ?, ?, ?)"));
        pstmt->setString(1, name);
        pstmt->setString(2, price);
        pstmt->setString(3, description);
        pstmt->setString(4, image);
        pstmt->execute();
        std::cout << "Product uploaded successfully!" << std::endl;
    } catch (sql::SQLException &e) {
        std::cerr << "Error while uploading product: " << e.what() << std::endl;
    }
}