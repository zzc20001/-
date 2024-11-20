#include"crow.h"
#include"database.h"
#include"usr_auth.h"
#include"save_image.h"
#include"upload.h"
#include<atomic>
std::unique_ptr<sql::Connection> con;
std::atomic<int> file_id_counter(1);
int main(){
    Database  db;
    con = db.getConnection();
    // 处理图片文件
    int max_id = get_max_product_id_from_database(con);
    file_id_counter.store(max_id + 1);  // 初始化原子计数器
    crow::SimpleApp app;
    CROW_ROUTE(app,"/register").methods("POST"_method)([](const crow::request& req){
        auto x = crow::json::load(req.body);
        if(!x){
            return crow::response(400);
        }
        auto username = x["username"].s();
        auto password = x["password"].s();
        auto email = x["email"].s();
        bool res = handleRegister(username,password,email,con);
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
CROW_ROUTE(app, "/upload-product").methods("POST"_method)([](const crow::request& req) {
    crow::multipart::message msg(req);
    std::string name;
    std::string description;
    double price = 0.0;
    std::string imageFileData;
    int cnt = 0;
    std::string username;
    int userid;

    try {
        // 获取并解析 Token
        std::string token = req.get_header_value("Authorization").substr(7); // 去掉 "Bearer "
        auto decoded = jwt::decode(token);
        jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{"your_secret_key"}) // 确保密钥一致
            .with_issuer("your_app") // 验证发行者
            .verify(decoded);

        // 提取用户名
        username = decoded.get_subject();

        // 提取用户ID
        userid = std::stoi(decoded.get_payload_claim("user_id").as_string());
    } catch (const std::exception& e) {
        return crow::response(401, "Unauthorized: " + std::string(e.what()));
    }

    for (const auto& part : msg.parts) {
        if (cnt == 0) {
            name = part.body;
        } else if (cnt == 1) {
            description = part.body;
        } else if (cnt == 2) {
            price = std::stod(part.body);
        } else if (cnt == 3) {
            int current_id = file_id_counter.fetch_add(1);  // 获取当前值并递增
            std::string filename = "uploaded_image_" + std::to_string(current_id) + ".jpg";  // 可以为每个文件生成唯一的文件名
            save_image_to_file(filename, part.body);  // 调用 save_image 函数保存图片
        }
        cnt++;
        }
    // 调用上传处理函数
        handleUploadProduct(name, std::to_string(price), description, 1, userid, con);
        return crow::response(200);
});
    app.port(8080).run(); 
}