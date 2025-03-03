#pragma once

#ifndef SHABBY_REDIS_CLIENT_EXPORTS
#define SHABBY_CLASS __declspec(dllimport)
#define SHABBY_TEMPLATE
#else //EXPORT
#define SHABBY_CLASS __declspec(dllexport)
#define SHABBY_TEMPLATE __declspec(dllexport)
#endif

#ifndef REDIS_CLIENT
#define REDIS_CLIENT

#include <iostream>
#include <string>
#include <functional>
#include <hiredis.h>

class SHABBY_CLASS  RedisClient {
public:

    RedisClient();

    RedisClient(const std::string& host, int port);

    ~RedisClient();

    void SelectDB(int dbIndex);

    std::string Get(const std::string& key);

    template<typename T>
    void Set(const std::string& key, const T& value, std::function<std::string(const T&)> toStringFunc) {
        std::string valueStr = toStringFunc(value); // ʹ�ô����ת������
        redisReply* reply = static_cast<redisReply*>(redisCommand(context, "SET %s %s", key.c_str(), valueStr.c_str()));
        if (reply == nullptr) {
            std::cerr << "SET command failed" << std::endl;
        }
        freeReplyObject(reply);
    }

    void Set(const std::string& key, const std::string& value);

    void Del(const std::string& key);

    void FlushDB();

private:
    redisContext* context;
};


#endif // !REDIS_CLIENT
