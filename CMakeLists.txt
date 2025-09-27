#CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

project(LogSystem)

# 手动设置MySQL路径（根据你的实际安装路径修改）
set(MYSQL_CONNECTOR_CXX_DIR "/usr/include/mysql")
include_directories(${MYSQL_CONNECTOR_CXX_DIR}/include)
link_directories(${MYSQL_CONNECTOR_CXX_DIR}/lib)

# # 查找 MySQL Connector/C++
# find_package(MySQL REQUIRED)

add_executable(LogSystem main.cpp Logger.cpp TaskManager.cpp)



# 将 MySQL 库链接到你的可执行文件
target_link_libraries(LogSystem ${MYSQLCPPCONN_LIBRARIES})
# 包含必要的目录
target_include_directories(LogSystem PRIVATE ${CONNECTORCPP_INCLUDE_DIRS})

# 如果pkg-config找不到，手动指定路径
if(NOT MYSQLCPPCONN_FOUND)
    message(WARNING "使用手动指定的MySQL路径")
    target_include_directories(LogSystem PRIVATE /usr/include/mysql)
    target_link_libraries(LogSystem mysqlcppconn)
endif()