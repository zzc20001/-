#include "crow.h"
#include "database.h"
#include "usr_auth.h"
#include "save_image.h"
#include "upload.h"
#include "display.h"
#include "heartbeat.h"
#include "dealmsg.h"
#include "SearchorUpdate.h"
#include <string>
#include <array>
#include <yaml-cpp/yaml.h>
#include <atomic>
#include <unordered_map>
#include <chrono>

std::unique_ptr<sql::Connection> con;
std::atomic<int> file_id_counter(1);

// 用于存储活跃的 WebSocket 连接
std::unordered_map<
            crow::websocket::connection*, \
            std::chrono::time_point<std::chrono::steady_clock>> conns;
std::mutex conns_mtx;

// 用于存储用户与连接
std::unordered_map<
            int, \
            crow::websocket::connection*> users;
std::mutex user_mtx;

int main() {

    // 读取yaml文件
    YAML::Node config;
    try {
        config = YAML::LoadFile("../config.yaml");
    } catch(YAML::BadFile &e) {
        CROW_LOG_ERROR << "YAML read failed!";
        return -1;
    }

    // 从配置文件中读取mysql相关信息
    auto cfgSql = config["mysql"];
    std::string sqlIp = cfgSql["ip"].as<std::string>();
    int sqlPort = cfgSql["port"].as<int>();
    std::string sqlUser = cfgSql["user"].as<std::string>();
    std::string sqlPasswd = cfgSql["password"].as<std::string>();
    std::string sqlDb = cfgSql["database"].as<std::string>();


    // 初始化数据库
    Database db(sqlIp, sqlPort, sqlUser, sqlPasswd, sqlDb);
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

    // 
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
                CROW_LOG_ERROR << "JWT verification failed: " << e.what();
                return crow::response(401, "Unauthorized: " + std::string(e.what()));
        }
        } catch (const std::exception& e) {
            CROW_LOG_ERROR << "JWT decoding failed: " << e.what();
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

    // 
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

    // 
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

    // 
    CROW_ROUTE(app, "/uploads/<string>").methods("GET"_method)(
        [](const crow::request& req, crow::response& res, const std::string& filename) {
            std::cout << "yes";
            std::string file_path = "../image/" + filename;
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

    CROW_ROUTE(app, "/display").methods("GET"_method) (
        [](crow::response& res) {
            auto products = getRecommendProducts(con);

            // 将商品列表转为json格式
            crow::json::wvalue rjson;
            std::vector<crow::json::wvalue> vjson;
            for(const auto& p : products) {
                vjson.emplace_back(p.to_json());
            }
            rjson["products"] = std::move(vjson);

            res.add_header("Content-Type", "application/json");
            res.write(rjson.dump());
            res.end();
        }
    );
  
    CROW_ROUTE(app,"/search").methods("GET"_method)([](const crow::request& req){
            // 获取查询参数
            std::string query = req.url_params.get("query");
            if (query.empty()) {
                return crow::response(400, "Query parameter is required");
            }
            return search_items(query, con);;
        });
    
    CROW_ROUTE(app, "/update-product/<int>").methods("PUT"_method)(
        [](const crow::request& req, int id) {
        try {
            // 提取 Authorization Token
            std::string token = req.get_header_value("Authorization").substr(7);

            if (token.empty()) {
                return crow::response(401, "Unauthorized: Token missing");
            }
            // 解析 FormData
            crow::multipart::message form(req);
            std::map<std::string, std::string> fields;
            std::string filepath;
            if (!form.get_part_by_name("name").body.empty()) {
                fields["name"] = form.get_part_by_name("name").body;
            }
            if (!form.get_part_by_name("price").body.empty()) {
                fields["price"] = form.get_part_by_name("price").body;
            }
            if (!form.get_part_by_name("description").body.empty()) {
                fields["description"] = form.get_part_by_name("description").body;
            }
            if (!form.get_part_by_name("category").body.empty()) {
                fields["category"] = form.get_part_by_name("category").body;
            }
            if (!form.get_part_by_name("image").body.empty()) {
                int current_id = file_id_counter.fetch_add(1);  // 获取当前值并递增
                std::string filename = "uploaded_image_" + std::to_string(current_id) + ".jpg";  
                filepath = save_image_to_file(filename,form.get_part_by_name("image").body);
            }
            if (fields.empty()) {
                return crow::response(400, "Bad Request: No fields to update");
            }


            // 调用更新函数
            update(id, con, fields, filepath);

            return crow::response(200, "Product updated successfully");
        } catch (const sql::SQLException& e) {
            // 数据库相关错误
            return crow::response(500, std::string("SQL Error: ") + e.what());
        } catch (const std::exception& e) {
            // 其他运行时错误
            return crow::response(500, std::string("Internal Server Error: ") + e.what());
        }
});

    CROW_ROUTE(app, "/buy").methods("POST"_method)(
        [](const crow::request& req) {
            auto json = crow::json::load(req.body);
            if(!json) {
                return crow::response(400);
            }

            std::string buyer_name = json["buyer_name"].s();
            int buyer_id = getUidFromUsername(con, buyer_name);
            int seller_id = json["seller_id"].i();
            int product_id = json["product_id"].i();

            // 新增订单为pending
            std::string sql{"INSERT INTO orders(buyer_id, seller_id, product_id, order_date, status) VALUES(?,?,?,NOW(),\"pending\")"};
            std::unique_ptr<sql::PreparedStatement> p;
            p.reset(con->prepareStatement(sql));
            p->setInt(1, buyer_id);
            p->setInt(2, seller_id);
            p->setInt(3, product_id);
            int res = p->execute();
            if(res) {
                return crow::response(400);
            }

            // 更新产品为pending
            p.reset(con->prepareStatement("UPDATE product SET status = \"pending\" WHERE product_id=?"));
            p->setInt(1, product_id);
            res = p->execute();
            if(res) {
                return crow::response(400);
            }

            // 正常逻辑应该是从第三方获取支付结果
            bool is_pay = json["is_pay"].b();
            p.reset(con->prepareStatement("SET @max_order_id = (SELECT MAX(order_id) FROM orders)"));
            p->execute();

            // 若付款, 则更新订单为completed, 更新产品为sold
            p.reset(con->prepareStatement("UPDATE orders SET status = ? WHERE order_id=@max_order_id"));
            if(is_pay) {
                p->setString(1, "completed");
                p->execute();
                p.reset(con->prepareStatement("UPDATE product SET status = ? WHERE product_id=?"));
                p->setString(1, "sold");
                p->setInt(2, product_id);
            }
            else {  // 否则更新订单为cancelled, 更新产品为available
                p->setString(1, "cancelled");
                p->execute();
                p.reset(con->prepareStatement("UPDATE product SET status = ? WHERE product_id=?"));
                p->setString(1, "available");
                p->setInt(2, product_id);
            }
            res = p->execute();
            if(res) {
                return crow::response(400);
            }

            return crow::response(200);
        }
    );

  CROW_WEBSOCKET_ROUTE(app, "/chat")
    // 连接建立时
.onaccept(
    [&](const crow::request& req, void** userdata) -> bool {
        // 解析请求中的用户数据
        auto query = req.url_params;
        std::string username = query.get("uid");    // 获取传递过来的username

        if(username.empty()) {
            CROW_LOG_ERROR << "Missing username in connection request.";
            return false;
        }
        // 如果你不需要uid，直接通过username获取user_id
        int uid = getUidFromUsername(con, username);  // 假设有一个函数根据username获取uid

        if(uid == -1) {
            CROW_LOG_ERROR << "Invalid username: " << username;
            return false;
        }

        *userdata = new connection_data{uid, username};   // 分配userdata
        CROW_LOG_INFO << "User " << uid << ":" << username << " connected.";
        return true;
    }
)
    // 连接成功时
    .onopen(
        [&](crow::websocket::connection& conn) {

            // connection <==> timestamp
            auto now = std::chrono::steady_clock::now();
            {
                std::lock_guard<std::mutex> lock(conns_mtx);
                conns[&conn] = now;
            }

            // uid <==> connection
            connection_data* userdata = (connection_data*)conn.userdata();
            {
                std::lock_guard<std::mutex> lock(user_mtx);
                users[userdata->user_id] = &conn;    
            }

            CROW_LOG_INFO << "new websocket connection from " << conn.get_remote_ip();
        }
    )
    // 收到消息时
    .onmessage(
        [&](crow::websocket::connection& conn, std::string message, bool is_binary) {
            CROW_LOG_INFO << "Received message: " << message;
 
            auto j = crow::json::load(message);  // 解析收到的消息

            std::string username = j["user"].s();
            std::string msg = j["text"].s();
            std::string timestamp = j["timestamp"].s();
            std::string user_to_string = j["user_to"].s();

            auto now = std::chrono::steady_clock::now();
            auto system_now = std::chrono::system_clock::now();
                
                // 在map中更新连接时间
                {
                    std::lock_guard<std::mutex> lock(conns_mtx);
                    auto it = std::find_if(conns.begin(), conns.end(), 
                                [&conn](const auto& pair) {
                                    return pair.first == &conn;
                                });
                    if (it != conns.end()) {
                        it->second = now;
                    }
                }

                // 不转发 pong 包
                if (!is_binary && message == "pong") {
                    CROW_LOG_INFO << "websocket connection: " << conn.get_remote_ip() 
                                    << " received pong";
                    return;
                }

                std::string date = steady_clock_to_timestamp(now, system_now);

                // 获取当前用户的信息
                connection_data* user = (connection_data*)conn.userdata();
                int user_from = user->user_id;

                // user_to为空就是群聊, 否则是私聊
                if(user_to_string.empty()) // 群聊
                {
                    // 将消息转发给其它客户端
                    {
                        std::lock_guard<std::mutex> lock(conns_mtx);
                        for (const auto& [c, _] : conns) {
                            if (c != &conn) {
                                c->send_text(message);
                            }
                        }
                    }

                    // 更新数据库
                    int ret = updateMessage(con, user_from, msg, date);
                    if (ret < 0) {
                        CROW_LOG_DEBUG << "updateMessage Error.";
                        return;
                    }
                }
                else    // 私聊
                {
                    int user_to = std::stoi(user_to_string);

                    // 将消息转发给uid_to
                    {
                        std::lock_guard<std::mutex> locK(conns_mtx);
                        auto it = users.find(user_to);
                        
                        // 若用户不在线
                        if(it == users.end()) {
                            CROW_LOG_ERROR << "User " << user_to << " is offlane.";
                            return;
                        }

                        // 用户在线
                        crow::websocket::connection* p = it->second;
                        p->send_text(message);
                        CROW_LOG_INFO << "Message sent to user_id: " << user_to;
                    }

                    // 更新数据库
                    int ret = updateMessage(con, user_from, user_to, msg, date);
                    if(ret < 0) {
                        CROW_LOG_DEBUG << "updateMessage Error.";
                        return;
                    }
                }
        }       
    )
    // 连接关闭时
    .onclose(
        [&](crow::websocket::connection& conn, const std::string& reason, uint16_t code) {
            
            // connection <==> timestamp
            {
                std::lock_guard<std::mutex> lock(conns_mtx);
                auto it = std::find_if(conns.begin(), conns.end(), 
                            [&conn](const auto& pair) {
                                return pair.first == &conn;
                            });
                
                // 这里若是找不到, 说明是该连接超时后由心跳线程删除的
                if (it != conns.end()) {
                    conns.erase(it);
                }
            }

            // user <==> connection
            connection_data* userdata = (connection_data*)conn.userdata();
            {
                std::lock_guard<std::mutex> lock(user_mtx);
                auto it = std::find_if(users.begin(), users.end(),
                            [&userdata](const auto& pair) {
                                return pair.first == userdata->user_id;
                            });
                
                // 心跳线程没有删除users, it不可能等于end
                if(it != users.end()) {
                    users.erase(it);
                }
            }
        }
    );

    CROW_ROUTE(app, "/get-message").methods("GET"_method)(
        [&](crow::response& res) {
            std::vector<message> msgs = getAllMessage(con);

            if(msgs.empty()) {
                CROW_LOG_INFO << "no msgs";
                return;
            }

            crow::json::wvalue rjson;
            std::vector<crow::json::wvalue> vjson;
            for(const auto& i: msgs) {
                crow::json::wvalue json;
                json["user_id"] = i.user_id;
                json["message"] = i.message_content;
                json["date"] = i.date;
                vjson.emplace_back(json);
            }

            rjson["info"] = std::move(vjson);

            res.add_header("Content-Type", "application/json");
            res.write(rjson.dump());
            res.end();
        }
    );

    // 启动心跳线程, 分离态
    std::thread(heartbeat_loop).detach();

    app.port(3000).run(); 
}