#!/bin/bash

# 生成当前时间的毫秒级时间戳
timestamp=$(date +%s%3N)

# 设置其他请求头部的值
access_key="bg_f343ed5e8f9766697392e096f9c837f7"
access_passphrase="12345678"
access_sign="9L6c6dkmL7fN5eFRo9wrGeNXlhFfvPoLRa0G3dMvTFY="
content_type="application/json"
locale="zh-CN"

# 执行 curl 请求
curl -X GET "https://api.bitget.com/api/v2/p2p/merchantList?online=yes" \
  -H "ACCESS-KEY: $access_key" \
  -H "ACCESS-PASSPHRASE: $access_passphrase" \
  -H "ACCESS-SIGN: $access_sign" \
  -H "ACCESS-TIMESTAMP: $timestamp" \
  -H "Content-Type: $content_type" \
  -H "locale: $locale"
