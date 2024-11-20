#include "upload.h"
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
                         int category_id, 
                         int userid, 
                         std::unique_ptr<sql::Connection>& con) {
    try {
        // 插入新商品
        std::unique_ptr<sql::PreparedStatement> pstmt;
        pstmt.reset(con->prepareStatement(
            "INSERT INTO Product (name, price, description, category_id, upload_date, status, username) "
            "VALUES (?, ?, ?, ?, NOW(), 'available', ?)"
        ));
        
        pstmt->setString(1, name);
        pstmt->setString(2, price);
        pstmt->setString(3, description);
        pstmt->setInt(4, category_id); // 分类ID
        pstmt->setInt(5, userid);  // 卖家ID

        pstmt->execute();
        std::cout << "Product uploaded successfully!" << std::endl;
    } catch (sql::SQLException& e) {
        std::cerr << "Error while uploading product: " << e.what() << std::endl;
    }
}
