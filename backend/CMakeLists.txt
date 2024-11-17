# 指定 CMake 最低版本
cmake_minimum_required(VERSION 3.15)

# 项目名称和语言
project(SecondHandTradingSystem LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含 vcpkg 的工具链文件路径（用户需要自行设置正确的路径）
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
else()
    message(FATAL_ERROR "VCPKG_ROOT 环境变量未设置，请确保 vcpkg 已安装")
endif()

# 添加可执行文件，假设主文件为 main.cpp
add_executable(${PROJECT_NAME} main.cpp)

# 查找并链接 jwt-cpp 和 OpenSSL
find_package(jwt-cpp CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE jwt-cpp OpenSSL::SSL OpenSSL::Crypto)
