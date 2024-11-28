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

std::vector<product> getRecommendProducts(std::unique_ptr<sql::Connection>& conn) {
    try {    
        std::vector<product> products;

        std::unique_ptr<sql::PreparedStatement> pstmt;

        std::string sql{"SELECT product.product_id, product.name, product.description, product.price, product_images.image_path FROM product, product_images WHERE product.product_id=product_images.productid"};

        pstmt.reset(conn->prepareStatement(sql));        
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while(res->next()) {
            product p;
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