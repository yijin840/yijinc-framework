#ifndef WSS_HPP
#define WSS_HPP
#include <string>

namespace yijinc {

class Wss {
public:
    static std::string get(const std::string& url);
    static std::string post(const std::string& url, const std::string& body);
};

} // namespace yijinc

#endif
