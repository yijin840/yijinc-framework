# test/CMakeLists.txt

# 添加测试文件
add_executable(yijinc_test test.cpp)

target_compile_definitions(yijinc_test PRIVATE BUFFERSIZE=16777216)
enable_testing()
add_test(NAME test/test.cpp COMMAND yijinc_test)
target_link_libraries(yijinc_test
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