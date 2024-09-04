cmake_minimum_required(VERSION 3.29.0)
project(yijinc VERSION 0.1.0 LANGUAGES C CXX)

#cmake_policy(SET CMP0167 NEW)

# 这里设置的是C标准
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(CMAKE_OSX_ARCHITECTURES "arm64")


set(LIBB64_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/b64)
set(LIBB64_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIBB64_LIBRARIES ${LIBB64_LIBRARY_DIR}/libb64.a)

#验证当前模块版本
#pkg-config --modversion libcurl

# 查找库
find_package(PkgConfig REQUIRED)
pkg_check_modules(LOG4CPLUS REQUIRED log4cplus>=2.1.1)
pkg_check_modules(FMT REQUIRED fmt>=10.2.1)
pkg_check_modules(CURL REQUIRED libcurl>=7.79.1)

# pkg_check_modules(BOOST REQUIRED IMPORTED_TARGET boost)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(Boost 1.82.0 REQUIRED COMPONENTS system filesystem url)
find_package(OpenSSL 3.3.1 REQUIRED)

# find_package(glog 0.6.0 REQUIRED)
# find_package(log4cplus 2.1.1 REQUIRED)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB SOURCES "src/*")
# 添加src
include_directories(src
        ${CMAKE_CURRENT_SOURCE_DIR}/test
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${LOG4CPLUS_INCLUDE_DIRS}
        ${FMT_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${LIBB64_INCLUDE_DIR}
        ${OPENSSL_INCLUDE_DIR}
        ${CURL_INCLUDE_DIRS}
)
link_directories(${LOG4CPLUS_LIBRARY_DIRS}
        ${FMT_LIBRARY_DIRS}
        ${Boost_LIBRARY_DIRS}
        ${LIBB64_INCLUDE_DIR}
        ${OPENSSL_LIBRARY_DIR}
        ${CURL_LIBRARY_DIRS}
)
# 添加可执行文件
add_executable(yijinc ${SOURCES}
        src/exception/yijinc_exception.cpp
        src/exception/yijinc_exception.h)

target_compile_definitions(yijinc PRIVATE BUFFERSIZE=16777216)
#开启DEBUG （按需开启）
target_compile_definitions(yijinc PRIVATE DEBUG)
# 添加链接库
target_link_libraries(yijinc
        PRIVATE
        nlohmann_json::nlohmann_json
        ssl
        crypto
        ${LIBB64_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${Boost_LIBRARIES}
        ${LOG4CPLUS_LIBRARIES}
        ${FMT_LIBRARIES}
        ${CURL_LIBRARIES}
)

# 添加 test 子目录
add_subdirectory(test)