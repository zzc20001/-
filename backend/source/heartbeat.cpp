/**
 * 
 * 
 * 
 */
#include "heartbeat.h"
#include "crow.h"
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>

static const int HEARTBEAT_INTERVAL_MS = 500000;  // 每5s发送一次心跳包
static const int HEARTBEAT_TIMEOUT_MS = 15000;  // 超过15s未响应视为超时

extern std::unordered_map<
            crow::websocket::connection*, \
            std::chrono::time_point<std::chrono::steady_clock>> conns;
extern std::mutex mtx;

void heartbeat_loop() {
    
    // 一直处理心跳
    while(true) {
        // 休眠5s, 醒来后发送心跳包
        std::this_thread::sleep_for(std::chrono::milliseconds(HEARTBEAT_INTERVAL_MS));

        auto now = std::chrono::steady_clock::now();

        {
            std::lock_guard<std::mutex> lock(mtx);

            // 遍历所有连接
            for(auto it = conns.begin(); it != conns.end();) {
                auto& [conn, last] = *it;
                
                // 判断当前连接是否超时
                if(std::chrono::duration_cast<std::chrono::milliseconds> \
                            (now - last).count() > HEARTBEAT_TIMEOUT_MS) {
                    // 若超时, 则关闭连接
                    conn->close("Heartbeat timeout");
                    it = conns.erase(it);   // 还要赋值回去, 防止迭代器失效
                }
                else {
                    // 否则发送ping包
                    conn->send_ping("ping");
                    ++it;
                }
            }
        }
    }
}