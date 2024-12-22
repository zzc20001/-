#include <SearchorUpdate.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>

crow::response search_items(const std::string& query,std::unique_ptr<sql::Connection>& con){
    try{
            std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement("SELECT * FROM Product WHERE category = ?"));
            pstmt->setString(1, query);
            std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
            crow::json::wvalue response;
            crow::json::wvalue::list data; 
            crow::json::wvalue product;
            while (res->next()) {
                product["product_id"] = res->getInt("product_id");
                product["name"] = res->getString("name");
                product["price"] = res->getInt("price");
                product["description"] = res->getString("description");
                product["category"] = res->getString("category");
                product["upload_date"] = res->getString("upload_date");
                product["status"] = res->getString("status");
                std::unique_ptr<sql::PreparedStatement> img_stmt(
                    con->prepareStatement("SELECT image_path FROM product_images WHERE productid = ?"));
                    img_stmt->setInt(1, res->getInt("product_id"));
                std::unique_ptr<sql::ResultSet> img_res(img_stmt->executeQuery());
                if (img_res->next()) {
                    product["image_path"] = img_res->getString("image_path");
                }
                data.push_back(product);
            }
            response["status"] = "success";
            response["message"] = "Products searched successfully";
            response["data"] = std::move(data);
            return crow::response(response);
        } catch (sql::SQLException& e) {
            CROW_LOG_ERROR << "Error while searching products: " << e.what();
            return crow::response(500);
        }
}
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <iostream>
#include <map>
#include <string>

void update(const int& id, std::unique_ptr<sql::Connection>& con, const std::map<std::string, std::string>& fields, const std::string& imagePath) {
    try {
        CROW_LOG_INFO << "New image path: " << imagePath;

        // 开始事务处理
        con->setAutoCommit(false);  // 禁用自动提交

        // 1. 删除旧的图片记录
        std::unique_ptr<sql::PreparedStatement> delete_stmt(con->prepareStatement(
            "DELETE FROM product_images WHERE productid = ?"
        ));
        delete_stmt->setInt(1, id);
        delete_stmt->execute();  // 执行删除操作

        CROW_LOG_INFO << "Old image(s) deleted for product ID " << id;

        // 2. 插入新的图片路径记录
        std::unique_ptr<sql::PreparedStatement> insert_stmt(con->prepareStatement(
            "INSERT INTO product_images (productid, image_path, created_at) VALUES (?, ?, NOW())"
        ));
        insert_stmt->setInt(1, id);       // 设置产品 ID
        insert_stmt->setString(2, imagePath);      // 设置新的图片路径
        insert_stmt->execute();                   // 执行插入操作

        // 提交事务
        con->commit();  
        con->setAutoCommit(true);  // 恢复自动提交

        CROW_LOG_INFO << "New image for product ID " << id << " uploaded successfully. Image path: " << imagePath;

    } catch (sql::SQLException& e) {
        // 出现错误时回滚事务
        con->rollback();  // 回滚事务
        CROW_LOG_ERROR << "SQL Error: " << e.what() << "\n"
                  << "Error Code: " << e.getErrorCode() << "\n"
                  << "SQL State: " << e.getSQLState() << "\n";
    } catch (std::exception& e) {
        // 处理其他异常
        CROW_LOG_ERROR << "Error: " << e.what();
    }
    try{
        // 更新其他字段的逻辑
        if (!fields.empty()) {
            // 动态生成 SQL 更新语句
            std::string query = "UPDATE Product SET ";
            bool first = true;
            for (const auto& [key, value] : fields) {
                if (!first) {
                    query += ", ";
                }
                query += key + " = ?";
                first = false;
            }
            query += " WHERE product_id = ?";

            // 创建 PreparedStatement
            std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));

            // 设置字段参数值
            int paramIndex = 1;
            for (const auto& [key, value] : fields) {
                pstmt->setString(paramIndex++, value);
            }
            pstmt->setInt(paramIndex, id);

            // 执行更新
            int affectedRows = pstmt->executeUpdate();
            if (affectedRows > 0) {
                CROW_LOG_INFO << "Product with ID " << id << " updated successfully. Rows affected: " << affectedRows;
            } else {
                CROW_LOG_ERROR << "No rows were updated. Check if the product ID exists.";
            }
        }
    } catch (sql::SQLException& e) {
        CROW_LOG_ERROR << "SQL Error: " << e.what() << "\n"
                  << "Error Code: " << e.getErrorCode() << "\n"
                  << "SQL State: " << e.getSQLState() << "\n";
    } catch (std::exception& e) {
        CROW_LOG_ERROR << "Error: " << e.what();
    }
}
