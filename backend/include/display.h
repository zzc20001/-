/**
 * 
 * 首页显示推荐商品
 * 
 */
#ifndef DISPLAY_H
#define DISPLAY_H

#include <cppconn/prepared_statement.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <vector>
#include "common.h"
#include "crow.h"

class product {
public:

    product() {}

    product(int id, std::string name, std::string price, std::string desc, std::string img_url) :
        id(id), name(name), price(price), desc(desc), img_url(img_url) {}
    crow::json::wvalue to_json() const;

    void setId(int id);
    void setName(std::string name);
    void setPirce(std::string price);
    void setDesc(std::string desc);
    void setImgUrl(std::string img_url);
    void owner(int usr_id);
private:
    int id;
    std::string img_url;
    std::string name;
    std::string price;
    std::string desc;
    int usr_id;
};

/**
 * @brief 获取推荐商品列表
 */
std::vector<product> getRecommendProducts(std::unique_ptr<sql::Connection>& conn);

#endif // DISPLAY_Hp