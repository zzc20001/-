#ifndef UPLOAD_H
#define UPLOAD_H
#include <cppconn/prepared_statement.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <fstream>
#include <iostream>

int get_max_product_id_from_database(std::unique_ptr<sql::Connection> &con) ;

void handleUploadProduct(const std::string& name, 
                         const std::string& price, 
                         const std::string& description, 
                         int category_id, 
                         int userid, 
                         std::unique_ptr<sql::Connection>& con) ;
#endif // UPLOAD_H