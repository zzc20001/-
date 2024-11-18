#include "crow.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

// 保存图片到指定目录
void save_image_to_file(const std::string& filename, const std::string& file_data) {
    // 指定图片保存的文件夹路径
    std::string directory = "/home/zzc20001/database_system/backend/image/";  // 请确保这个目录存在
    std::string filepath = directory + filename;
    // 打开文件并保存二进制数据
    std::ofstream outfile(filepath, std::ios::binary);
    if (outfile.is_open()) {
        outfile.write(file_data.c_str(), file_data.size());
        outfile.close();
        std::cout << "Image saved as " << filepath << std::endl;
    } else {
        std::cerr << "Error opening file to write image" << std::endl;
    }
}
