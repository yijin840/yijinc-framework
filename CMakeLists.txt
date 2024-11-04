cmake_minimum_required(VERSION 3.29.0)
project(yijinc VERSION 0.1.0 LANGUAGES C CXX)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(VCPKG_TARGET_TRIPLET "arm64-osx")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(VCPKG_TARGET_TRIPLET "x64-linux")
endif ()

# cmake_policy(SET CMP0167 NEW)
# 设置vcpkg chain brew install vcpkg
# 这里设置的是C标准
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#开启性能分析
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")


set(CMAKE_BUILD_TYPE Debug)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# 设置vcpkg目录
set(VCPKG_ROOT $ENV{VCPKG_ROOT})
message(STATUS "VCPKG_ROOT: ${VCPKG_ROOT}")

# 设置工具链
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

# 设置crow
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message(STATUS "current system is linux")
    set(CMAKE_PREFIX_PATH ${VCPKG_ROOT}/installed/x64-linux/share/crow ${CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH ${VCPKG_ROOT}/installed/x64-linux/share/curl ${CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH ${VCPKG_ROOT}/installed/x64-linux/share/log4cplus ${CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH ${VCPKG_ROOT}/installed/x64-linux/share/unofficial-b64 ${CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH ${VCPKG_ROOT}/installed/x64-linux/share/libuv ${CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH ${VCPKG_ROOT}/installed/x64-linux/share/usockets ${CMAKE_PREFIX_PATH})
    set(CURL_INCLUDE_DIR ${VCPKG_ROOT}/installed/x64-linux/share/curl)
    set(ASIO_INCLUDE_DIR ${VCPKG_ROOT}/installed/x64-linux/share/asio)
    set(LOG4CPLUS_INCLUDE_DIR ${VCPKG_ROOT}/installed/x64-linux/share/log4cplus)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    message(STATUS "current system is macOS")
    set(CMAKE_PREFIX_PATH ${VCPKG_ROOT}/installed/arm64-osx/share/crow ${CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH ${VCPKG_ROOT}/installed/arm64-osx/share/curl ${CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH ${VCPKG_ROOT}/installed/arm64-osx/share/log4cplus ${CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH ${VCPKG_ROOT}/installed/arm64-osx/share/unofficial-b64 ${CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH ${VCPKG_ROOT}/installed/arm64-osx/share/libuv ${CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH ${VCPKG_ROOT}/installed/arm64-osx/share/usockets ${CMAKE_PREFIX_PATH})
    set(CURL_INCLUDE_DIR ${VCPKG_ROOT}/installed/arm64-osx/share/curl)
    set(ASIO_INCLUDE_DIR ${VCPKG_ROOT}/installed/arm64-osx/share/asio)
    set(LOG4CPLUS_INCLUDE_DIR ${VCPKG_ROOT}/installed/arm64-osx/share/log4cplus)
endif ()

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "PATH: ${CMAKE_HOME_DIRECTORY}")
set(USOCKETS_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/include/uSockets.a)
# 验证当前模块版本
# pkg-config --modversion libcurl

# 查找库
find_package(PkgConfig REQUIRED)
pkg_check_modules(FMT REQUIRED fmt>=10.2.1)
pkg_check_modules(CURL REQUIRED libcurl)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(Boost 1.86.0 REQUIRED COMPONENTS system filesystem url)
find_package(OpenSSL 3.3.2 REQUIRED)
find_package(log4cplus CONFIG REQUIRED)
find_package(unofficial-b64 CONFIG REQUIRED)
find_package(websocketpp REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(libuv CONFIG REQUIRED)
find_package(OpenCV REQUIRED)
#find_package(uSockets REQUIRED)

find_path(UWEBSOCKETS_INCLUDE_DIRS "uwebsockets/App.h")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB SOURCES "src/*"
        "src/common/*"
        "src/http/*"
        "src/wss/*"
        "src/processor/*"
        "src/model/*"
        "src/mapper/*"
        "src/opencv/*")

# 添加src
include_directories(src
        ${CMAKE_CURRENT_SOURCE_DIR}/test
        ${CMAKE_CURRENT_SOURCE_DIR}/include

        # ${MYSQL_SOURCE_PATH}/include/mysql
        ${LOG4CPLUS_INCLUDE_DIRS}
        ${FMT_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${OPENSSL_INCLUDE_DIR}
        ${CURL_INCLUDE_DIRS}
        ${WEBSOCKETPP_INCLUDE_DIRS}

        # ${VCPKG_ROOT}/installed/arm64-osx/include/mysql
)
link_directories(${LOG4CPLUS_LIBRARY_DIRS}
        ${FMT_LIBRARY_DIRS}
        ${Boost_LIBRARY_DIRS}
        ${OPENSSL_LIBRARY_DIR}
        ${CURL_LIBRARY_DIRS}
        ${WEBSOCKETPP_LIBRARY_DIRS}
)

# 添加可执行文件
add_executable(yijinc ${SOURCES} src/main.cpp)

target_compile_definitions(yijinc PRIVATE BUFFERSIZE=16777216)

# 开启DEBUG （按需开启）
target_compile_definitions(yijinc PRIVATE DEBUG)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_compile_definitions(yijinc PRIVATE LINUX_SYSTEM)
endif ()
if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    target_compile_definitions(yijinc PRIVATE DARWIN_SYSTEM)
endif ()

target_link_directories(yijinc PRIVATE ${UWEBSOCKETS_INCLUDE_DIRS})

# 添加链接库
target_link_libraries(yijinc
        PRIVATE
        nlohmann_json::nlohmann_json
        unofficial::b64::b64
        ssl
        crypto
        # unofficial::libmysql::libmysql
        # sqlpp11::mysql
        # sqlpp11::sqlpp11
        Crow::Crow
        asio::asio
        log4cplus::log4cplus
        ${OPENSSL_LIBRARIES}
        ${Boost_LIBRARIES}
        # ${LOG4CPLUS_LIBRARIES}
        ${FMT_LIBRARIES}
        ${CURL_LIBRARIES}
        ${WEBSOCKETPP_LIBRARIES}
        ${UWEBSOCKETS_LIB_PATH}
        ${USOCKETS_INCLUDE_DIR}
        ZLIB::ZLIB
        ${OpenCV_LIBS}
        $<IF:$<TARGET_EXISTS:libuv::uv_a>,libuv::uv_a,libuv::uv>
)

# 添加 test 子目录
# add_subdirectory(test)