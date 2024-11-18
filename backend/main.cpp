#include"crow.h"
#include"database.h"
#include"usr_auth.h"
#include"save_image.h"
std::unique_ptr<sql::Connection> con;
int main(){
    Database  db;
    con = db.getConnection();
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
        crow::multipart::message msg(req);
        std::string name;
        std::string description;
        double price = 0.0;
        std::string imageFileData;
        int cnt = 0;
        for (const auto& part : msg.parts) {
            if(cnt == 0)   name = part.body;
            if(cnt == 1)   description = part.body;
            if(cnt == 2)   price = std::stod(part.body);
            if (cnt == 3) {  
                // 处理图片文件
                std::string filename = "uploaded_image.jpg";  // 可以为每个文件生成唯一的文件名
                save_image_to_file(filename, part.body);  // 调用 save_image 函数保存图片
            }
            cnt++;
        }
        return crow::response(200);
    });
    app.port(8080).run(); 
}