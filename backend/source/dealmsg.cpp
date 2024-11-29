#include "dealmsg.h"

int updateMessage(std::unique_ptr<sql::Connection>& con, int uid, const std::string& msg, const std::string& date)
{
    try {
        std::string sql{"INSERT INTO message (user_id, message_content, date) VALUES(?, ?, ?)"};
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(sql));
        pstmt->setInt(1, uid);
        pstmt->setString(2, msg);
        pstmt->setString(3, date);

        int res = pstmt->executeUpdate();
        return res;

    } catch(sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
        return -1;
    }

    return -1;
}

std::string steady_clock_to_timestamp(std::chrono::steady_clock::time_point now, 
                                     std::chrono::system_clock::time_point system_now)
{
    auto diff = system_now.time_since_epoch() - now.time_since_epoch();
    auto adjust = std::chrono::system_clock::time_point(now.time_since_epoch() + diff);

    // 转换为time_t格式
    auto fmt = std::chrono::system_clock::to_time_t(adjust);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&fmt), "%Y-%m-%d %H-%M:%S");
    return oss.str();
}