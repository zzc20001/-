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
    if(!x) return crow::response(400);

    auto username = x["username"].s();
    auto password = x["password"].s();
    auto email = x["email"].s();
    bool result = handleRegister(username, password, email, con);

    crow::response res;
    
    if(!result) {
        res.code = 400;
        return res;
    }

    res.code = 200;
    return res;
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
    std::string category;
    int userid;
    try {
        // 获取并解析 Token
        std::string token = req.get_header_value("Authorization").substr(7);
        auto decoded = jwt::decode(token);
        try {
            jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{"your_secret_key"}) // 确保密钥一致
            .with_issuer("your_app") // 验证发行者
            .verify(decoded);  // 这里如果失败会抛出异常

            std::string username = decoded.get_subject();
            userid = std::stoi(decoded.get_payload_claim("user_id").as_string());
        // 继续处理...
        } catch (const std::exception& e) {
            std::cerr << "JWT verification failed: " << e.what() << std::endl;
            return crow::response(401, "Unauthorized: " + std::string(e.what()));
    }
    } catch (const std::exception& e) {
        std::cerr << "JWT decoding failed: " << e.what() << std::endl;
        return crow::response(401, "Unauthorized: " + std::string(e.what()));
    }
    std::string filepath;
    for (const auto& part : msg.parts) {
        if (cnt == 0) {
            name = part.body;
        } else if (cnt == 1) {
            description = part.body;
        } else if (cnt == 2) {
            price = std::stod(part.body);
        } else if (cnt == 3) {
            category = part.body;
        }   else if (cnt == 4) {
            int current_id = file_id_counter.fetch_add(1);  // 获取当前值并递增
            std::string filename = "uploaded_image_" + std::to_string(current_id) + ".jpg";  // 可以为每个文件生成唯一的文件名
            filepath = save_image_to_file(filename, part.body);  // 调用 save_image 函数保存图片
        }
        cnt++;
        }
    // 调用上传处理函数
        handleUploadProduct(name, std::to_string(price), description, category, userid,filepath, con);
        return crow::response(200);
});
CROW_ROUTE(app,"/user").methods("GET"_method)([](const crow::request& req){
    std::string token = req.get_header_value("Authorization").substr(7);
    auto decoded = jwt::decode(token);
    jwt::verify()
        .allow_algorithm(jwt::algorithm::hs256{"your_secret_key"})
        .with_issuer("your_app")
        .verify(decoded);
    std::string username = decoded.get_subject();
    return find_user_profile(username,con);
});
CROW_ROUTE(app,"/get-products").methods("GET"_method)([](const crow::request& req){
    std::string token = req.get_header_value("Authorization").substr(7);
    auto decoded = jwt::decode(token);
    jwt::verify()
        .allow_algorithm(jwt::algorithm::hs256{"your_secret_key"})
        .with_issuer("your_app")
        .verify(decoded);
    std::string username = decoded.get_subject();
    int user_id = std::stoi(decoded.get_payload_claim("user_id").as_string());
    return handleLoadProductResponse(user_id,con);
});
CROW_ROUTE(app, "/uploads/<string>").methods("GET"_method)(
    [](const crow::request& req, crow::response& res, const std::string& filename) {
          std::cout << "yes" << std::endl;
        std::string file_path = "/home/zzc20001/database_system/backend/image/" + filename;
        // 打开文件
        std::ifstream file(file_path, std::ios::binary);
        if (file) {
            std::ostringstream content;
            content << file.rdbuf(); // 读取文件内容

            // 设置响应
            // 设置 Content-Type 为图片类型，根据后缀名判断
            if (filename.find(".jpg") != std::string::npos) {
                res.add_header("Content-Type", "image/jpeg");
            } else if (filename.find(".png") != std::string::npos) {
                res.add_header("Content-Type", "image/png");
            } else {
                res.add_header("Content-Type", "application/octet-stream");
            }
            res.write(content.str());
        } else {
            // 文件未找到
            res.code = 404;
            res.write("File not found");
        }
        res.end();
});   
    app.port(3000).run(); 
}
