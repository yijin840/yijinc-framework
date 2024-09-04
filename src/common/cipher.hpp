#ifndef CIPHER_HPP
#define CIPHER_HPP
#include <string>
#include <vector>

namespace yijinc {
    class Cipher {
    public:
        static std::vector<unsigned char> hmacSha256(const std::string &key,
                                                     const std::string &data);

        static std::string base64_encode(const std::vector<unsigned char> &input);

        static std::vector<unsigned char> hex_to_bytes(const std::string &hex);
    };
} // namespace yijinc

#endif
