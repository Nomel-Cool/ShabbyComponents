#pragma once
#ifndef REDIS_CLIENT
#define REDIS_CLIENT

#include <iostream>
#include <string>
#include <functional>
#include <hiredis.h>

class RedisClient {
public:
    RedisClient(const std::string& host, int port) {
        context = redisConnect(host.c_str(), port);
        if (context == nullptr || context->err) {
            if (context) {
                std::cerr << "Connection error: " << context->errstr << std::endl;
                redisFree(context);
            }
            else {
                std::cerr << "Connection error: can't allocate redis context" << std::endl;
            }
            exit(1);
        }
    }

    ~RedisClient() {
        if (context) {
            redisFree(context);
        }
    }

    void selectDB(int dbIndex) { 
        redisReply* reply = static_cast<redisReply*>(redisCommand(context, "SELECT %d", dbIndex));
        if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) { 
            std::cerr << "SELECT command failed: " << (reply ? reply->str : "No reply") << std::endl;
        } 
        freeReplyObject(reply);
    }

    std::string get(const std::string& key) {
        redisReply* reply = static_cast<redisReply*>(redisCommand(context, "GET %s", key.c_str()));
        if (reply == nullptr) {
            std::cerr << "GET command failed" << std::endl;
            return "";
        }
        std::string value = reply->str ? reply->str : "";
        freeReplyObject(reply);
        return value;
    }

    template<typename T>
    void set(const std::string& key, const T& value, std::function<std::string(const T&)> toStringFunc) {
        std::string valueStr = toStringFunc(value); // 使用传入的转换函数
        redisReply* reply = static_cast<redisReply*>(redisCommand(context, "SET %s %s", key.c_str(), valueStr.c_str()));
        if (reply == nullptr) {
            std::cerr << "SET command failed" << std::endl;
        }
        freeReplyObject(reply);
    }

    void set(const std::string& key, const std::string& value) {
        redisReply* reply = static_cast<redisReply*>(redisCommand(context, "SET %s %s", key.c_str(), value.c_str()));
        if (reply == nullptr) {
            std::cerr << "SET command failed" << std::endl;
        }
        freeReplyObject(reply);
    }

    void del(const std::string& key) {
        redisReply* reply = static_cast<redisReply*>(redisCommand(context, "DEL %s", key.c_str()));
        if (reply == nullptr) {
            std::cerr << "DEL command failed" << std::endl;
        }
        freeReplyObject(reply);
    }

    void flushDB() {
        redisReply* reply = static_cast<redisReply*>(redisCommand(context, "FLUSHDB"));
        if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) {
            std::cerr << "FLUSHDB command failed: " << (reply ? reply->str : "No reply") << std::endl;
        }
        freeReplyObject(reply);
    }

private:
    redisContext* context;
};


#endif // !REDIS_CLIENT
