/**
 * 
 * 
 * 
 */
#include "display.h"

crow::json::wvalue product::to_json() const {
    crow::json::wvalue json;
    json["id"] = this->id;
    json["name"] = this->name;
    json["price"] = this->price;
    json["usr_id"] = this->usr_id;
    json["desc"] = this->desc;
    json["img_url"] = this->img_url;
    return json;
}

void product::setId(int id) {
    this->id = id;
}

void product::setName(std::string name) {
    this->name = name;
}

void product::setPirce(std::string price) {
    this->price = price;
}

void product::setDesc(std::string desc) {
    this->desc = desc;
}

void product::setImgUrl(std::string img_url) {
    this->img_url = img_url;
}

void product::owner(int usr_id) {
    this->usr_id = usr_id;
}
std::vector<product> getRecommendProducts(std::unique_ptr<sql::Connection>& conn) {
    try {    
        std::vector<product> products;

        std::unique_ptr<sql::PreparedStatement> pstmt;

        std::string sql{"SELECT Product.product_id, Product.user_id, Product.name, Product.description, Product.price, product_images.image_path FROM Product, product_images WHERE Product.product_id=product_images.productid"};

        pstmt.reset(conn->prepareStatement(sql));        
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while(res->next()) {
            product p;
            p.owner(res->getInt("user_id"));
            p.setId(res->getInt("product_id"));
            p.setName(res->getString("name"));
            p.setPirce(res->getString("price"));
            p.setDesc(res->getString("description"));
            std::string img = res->getString("image_path");
            p.setImgUrl(img.substr(9, img.size() - 9));
            products.emplace_back(p);
        }

        return products;
    } catch(sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }

    return std::vector<product>{};
}