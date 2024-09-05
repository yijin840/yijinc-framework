#include "cipher.h"
#include "logger.h"
#include <b64/encode.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <string>
#include <vector>

std::vector<unsigned char> yijinc::Cipher::hmacSha256(const std::string& key, const std::string& message) {
    std::vector<unsigned char> result(EVP_MD_size(EVP_sha256()));
    unsigned int result_len = 0;
    HMAC(
        EVP_sha256(),
        key.data(),
        key.size(),
        reinterpret_cast<const unsigned char*>(message.data()),
        message.size(),
        result.data(),
        &result_len);

    result.resize(result_len);
    return result;
}

std::string yijinc::Cipher::base64_encode(const std::vector<unsigned char>& input) {
    base64::base64_encodestate state;
    base64_init_encodestate(&state);
    // Calculate the buffer size needed for the encoded string
    int bufferSize = 4 * ((input.size() + 2) / 3);
    std::string encodedData(bufferSize, '\0');
    // Encode the input data
    int encodedSize =
        base64_encode_block(reinterpret_cast<const char*>(input.data()), input.size(), &encodedData[0], &state);
    encodedSize += base64_encode_blockend(&encodedData[0] + encodedSize, &state);
    // Remove trailing null character if present
    if (!encodedData.empty() && encodedData.back() == '\0') {
        encodedData.pop_back();
    }
    // Resize the string to the actual encoded size
    encodedData.resize(encodedSize);

    return encodedData;
}

std::vector<unsigned char> yijinc::Cipher::hex_to_bytes(const std::string& hex) {
    std::vector<unsigned char> bytes(hex.size() / 2);
    for (size_t i = 0; i < hex.size(); i += 2) {
        unsigned char highByte = std::toupper(hex[i]);
        unsigned char lowByte = std::toupper(hex[i + 1]);

        highByte = (highByte > '9') ? (highByte - 'A' + 10) : (highByte - '0');
        lowByte = (lowByte > '9') ? (lowByte - 'A' + 10) : (lowByte - '0');

        bytes[i / 2] = (highByte << 4) | lowByte;
    }
    return bytes;
}
