#include "beast_http.h"
#include "common/cipher.h"
#include "common/logger.h"
#include "i_http.h"
#include <curl/curl.h>
#include <vector>
#define DEBUG_ENABLE false
#define CONNECT_TIMEOUT 20

using cipher = yijinc::Cipher;

namespace yijinc {} // namespace yijinc
