#ifndef DEALMSG_H
#define DEALMSG_H

#include "common.h"
#include "crow.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>

/**
 * @brief 将消息发到数据库
 */
int updateMessage(std::unique_ptr<sql::Connection>& con, int uid, const std::string& msg, const std::string& date);

/**
 * @brief 处理chrono库的时间与数据库timestamp对应
 */
std::string steady_clock_to_timestamp(std::chrono::steady_clock::time_point now, 
                                     std::chrono::system_clock::time_point system_now);

#endif // DEALMSG_H