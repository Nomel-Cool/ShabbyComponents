#pragma once
#ifndef REDIS_CLIENT_H
#define REDIS_CLIENT_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include "hiredis.h"

#ifdef SHABBY_REDIS_CLIENT_EXPORTS
#define REDIS_API __declspec(dllexport)
#else
#define REDIS_API __declspec(dllimport)
#endif

/*
*   From Now on it is fucking important to apply this lib apparently in your program.
*   #pragma comment(lib, "path/to/hiredis.lib")
*/

class REDIS_API RedisClient {
public:
    RedisClient();
    RedisClient(const std::string& host, int port);
    ~RedisClient();

    void SelectDB(int dbIndex);
    std::string Get(const std::string& key);
    bool GetBinary(const std::string& key, std::vector<uint8_t>& output);

    template<typename T>
    void Set(const std::string& key, const T& value, std::function<std::string(const T&)> toStringFunc) {
        std::string valueStr = toStringFunc(value);
        redisReply* reply = reinterpret_cast<redisReply*>(redisCommand(context, "SET %s %s", key.c_str(), valueStr.c_str()));
        if (!reply) std::cerr << "SET command failed" << std::endl;
        freeReplyObject(reply);
    }

    template<typename T>
    void SetEx(const std::string& key, const T& value, int expire_ms,
        std::function<std::string(const T&)> toStringFunc) {
        std::string valueStr = toStringFunc(value);
        redisReply* reply = reinterpret_cast<redisReply*>(redisCommand(context, "SET %s %s PX %d",
            key.c_str(), valueStr.c_str(), expire_ms));
        if (!reply) std::cerr << "SETEX command failed" << std::endl;
        freeReplyObject(reply);
    }

    template<typename T>
    void SetBinEx(const std::string& key, const T& value, int expire_ms,
        std::function<std::vector<uint8_t>(const T&)> toBytesFunc) {
        std::vector<uint8_t> valueBytes = toBytesFunc(value);
        // ÐÞÕý²ÎÊýË³Ðò£ºkey, data, size, expire
        redisReply* reply = reinterpret_cast<redisReply*>(redisCommand(context, "SET %s %b PX %d",
            key.c_str(),
            valueBytes.data(),
            valueBytes.size(),
            expire_ms));
        if (!reply) std::cerr << "SET Binary PX command failed" << std::endl;
        freeReplyObject(reply);
    }

    void Set(const std::string& key, const std::string& value);
    void SetEx(const std::string& key, const std::string& value, int expire_ms);
    void Del(const std::string& key);
    void FlushDB();

private:
    redisContext* context;
};

#endif // REDIS_CLIENT_H