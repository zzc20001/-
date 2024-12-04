#ifndef DEALMSG_H
#define DEALMSG_H

#include "common.h"
#include "crow.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <array>

struct message {
    int user_id;
    std::string message_content;
    std::string date;
};

/**
 * @brief 通过username获取uid
 */
int getUidFromUsername(std::unique_ptr<sql::Connection>& con, std::string& username);

/**
 * @brief 通过uid获取username
 */
std::string getUsernameFromUid(std::unique_ptr<sql::Connection>& con, int uid);

/**
 * @brief 将消息发到数据库
 */
int updateMessage(std::unique_ptr<sql::Connection>& con, int uid, const std::string& msg, const std::string& date);

/**
 * @brief 同上, 用于私聊
 */
int updateMessage(std::unique_ptr<sql::Connection>& con, int uid_from, int uid_to, const std::string& msg, const std::string& date);

/**
 * @brief 处理chrono库的时间与数据库timestamp对应
 */
std::string steady_clock_to_timestamp(std::chrono::steady_clock::time_point now, 
                                     std::chrono::system_clock::time_point system_now);

/**
 * @brief 获得所有信息
 */
std::vector<message> getAllMessage(std::unique_ptr<sql::Connection>& con);

#endif // DEALMSG_H