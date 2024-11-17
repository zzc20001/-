#include"crow.h"
#include"database.h"
#include"usr_auth.h"
std::unique_ptr<sql::Connection> con;
int main(){
    con = Database().getConnection();
    crow::SimpleApp app;
    CROW_ROUTE(app,"/register").methods("POST"_method)([](const crow::request& req){
        auto x = crow::json::load(req.body);
        if(!x){
            return crow::response(400);
        }
        auto username = x["username"].s();
        auto password = x["password"].s();
        bool res = handleRegister(username,password,con);
        if(!res){
            return crow::response(400);
        }
        return crow::response(200);
    });
    CROW_ROUTE(app,"/login").methods("POST"_method)([](const crow::request& req){
        auto x = crow::json::load(req.body);
        if(!x){
            return crow::response(400);
        }
        auto username = x["username"].s();
        auto password = x["password"].s();
        return handleLogin(username,password,con);
    });
    CROW_ROUTE(app,"/upload-product").methods("POST"_method)([](const crow::request& req){
        auto x = crow::json::load(req.body);
        if(!x){
            return crow::response(400);
        }
        auto name = x["name"].s();
        auto price = x["price"].s();
        auto description = x["description"].s();
        auto image = x["image"].s();
        return crow::response(200);
    });
}