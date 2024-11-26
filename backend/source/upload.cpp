#include "upload.h"
#include "crow.h"
int get_max_product_id_from_database(std::unique_ptr<sql::Connection> &con) {
    try {
        // 准备 SQL 查询语句
        std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement("SELECT MAX(product_id) AS max_id FROM Product"));
        
        // 执行查询
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        
        // 检查结果
        if (res->next()) {
            int max_id = res->getInt("max_id");
            if (res->isNull(1)) { // 如果表中没有数据，max_id 可能为 NULL
                return 0;
            }
            return max_id;
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Error while fetching max product_id: " << e.what() << std::endl;
        return 0; // 出现异常时，返回默认值
    }

    return 0; // 未获取到结果时，返回默认值
}
void handleUploadProduct(const std::string& name, 
                         const std::string& price, 
                         const std::string& description, 
                         const std::string& category, 
                         int userid, 
                         const std::string& filepath, 
                         std::unique_ptr<sql::Connection>& con) {
    try {
        // 开启事务
        con->setAutoCommit(false);

        // 插入新商品
        std::unique_ptr<sql::PreparedStatement> pstmt;
        pstmt.reset(con->prepareStatement(
            "INSERT INTO Product (name, description, price, category, upload_date, status, user_id) "
            "VALUES (?, ?, ?, ?, NOW(), 'available', ?)"
        ));

        pstmt->setString(1, name);
        pstmt->setString(2, description);
        pstmt->setString(3, price);
        pstmt->setString(4, category);
        pstmt->setInt(5, userid);

        pstmt->execute();

        // 获取插入的商品 ID
        int product_id = -1;
        std::unique_ptr<sql::PreparedStatement> id_stmt;
        id_stmt.reset(con->prepareStatement("SELECT LAST_INSERT_ID() AS product_id"));//获取最后插入的id
        std::unique_ptr<sql::ResultSet> res(id_stmt->executeQuery());
        if (res->next()) {
            product_id = res->getInt("product_id");
        }

        if (product_id == -1) {
            throw std::runtime_error("Failed to retrieve product ID after insertion.");
        }

        // 插入图片路径
        std::unique_ptr<sql::PreparedStatement> img_stmt;
        img_stmt.reset(con->prepareStatement(
            "INSERT INTO product_images (productid, image_path, created_at) VALUES (?, ?, NOW())"
        ));
        img_stmt->setInt(1, product_id);
        img_stmt->setString(2, filepath);

        img_stmt->execute();
        // 提交事务
        con->commit();
        con->setAutoCommit(true);  // 恢复自动提交
        std::cout << "Product and image uploaded successfully! Product ID: " << product_id << std::endl;

    } catch (const sql::SQLException& e) {
        // 出现异常时回滚事务
        con->rollback();
        con->setAutoCommit(true);  // 恢复自动提交
        std::cerr << "Error while uploading product: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        // 处理其他可能的异常
        con->rollback();
        con->setAutoCommit(true);  // 恢复自动提交
        std::cerr << "General error: " << e.what() << std::endl;
    }
}

crow::response handleLoadProductResponse(const int user_id, 
                       std::unique_ptr<sql::Connection>& con) {
    try {
        // 查询用户上传的商品
        std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement("SELECT * FROM Product WHERE user_id = ?"));
        pstmt->setInt(1, user_id);
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
        response["message"] = "Products loaded successfully";
        response["data"] = std::move(data);
        return crow::response(response);
    } catch (sql::SQLException& e) {
        std::cerr << "Error while loading products: " << e.what() << std::endl;
        return crow::response(500);
    }
}