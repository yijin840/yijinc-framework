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

file(GLOB VCPKG_INCLUDE_DIRS "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/share/*")
foreach(PKG ${VCPKG_INCLUDE_DIRS})
    # 获取库或文件夹的名称（不带路径和扩展名）
    if(IS_DIRECTORY ${PKG})
        get_filename_component(PKG_NAME ${PKG} NAME)
        list(APPEND CMAKE_PREFIX_PATH ${PKG})
    endif()
endforeach()

# message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
# 查找库
find_package(PkgConfig REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)
find_package(curl REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS)
find_package(boost_assert REQUIRED CONFIG)
find_package(boost_asio REQUIRED CONFIG)
find_package(boost_beast REQUIRED CONFIG)
find_package(OpenSSL REQUIRED)
find_package(log4cplus CONFIG REQUIRED)
find_package(unofficial-b64 CONFIG REQUIRED)
find_package(websocketpp REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(libuv CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)

# message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "PATH: ${CMAKE_HOME_DIRECTORY}")
# 验证当前模块版本
# pkg-config --modversion libcurl
# 查找库

# find_path(UWEBSOCKETS_INCLUDE_DIRS "uwebsockets/App.h")
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
        ${OpenCV_INCLUDE_DIRS}
        ${BOOST_INCLUDE_DIRS}
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

# 添加链接库
target_link_libraries(yijinc
        PRIVATE
        nlohmann_json::nlohmann_json
        unofficial::b64::b64
        OpenSSL::SSL
        OpenSSL::Crypto
        Crow::Crow asio::asio
        Boost::beast Boost::asio Boost::assert
        CURL::libcurl
        log4cplus::log4cplus
        ZLIB::ZLIB
        $<IF:$<TARGET_EXISTS:libuv::uv_a>,libuv::uv_a,libuv::uv>
        fmt::fmt
        ${Boost_LIBRARIES}
        ${OpenCV_LIBS}
        # unofficial::libmysql::libmysql
        # sqlpp11::mysql
        # sqlpp11::sqlpp11
)


# 添加 test 子目录
# add_subdirectory(test)