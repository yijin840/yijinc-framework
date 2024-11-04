#include "BeastHttp.h"
#include "IHttp.h"
#include "common/Cipher.h"
#include "common/Logger.h"
#include <curl/curl.h>
#include <vector>
#define DEBUG_ENABLE false
#define CONNECT_TIMEOUT 20

using cipher = yijinc::Cipher;

namespace yijinc {} // namespace yijinc
