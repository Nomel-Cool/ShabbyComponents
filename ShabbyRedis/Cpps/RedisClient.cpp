#include "RedisClient.h"

RedisClient::RedisClient()
{
	context = redisConnect("127.0.0.1", 6379);
    if (context == nullptr || context->err) {
        if (context) {
            std::cerr << "Connection error: " << context->errstr << std::endl;
            redisFree(context);
        }
        else {
            std::cerr << "Default connection error: can't allocate redis context" << std::endl;
        }
        exit(1);
    }
}

RedisClient::RedisClient(const std::string& host, int port)
{
    context = redisConnect(host.c_str(), port);
    if (context == nullptr || context->err) {
        if (context) {
            std::cerr << "Connection error: " << context->errstr << std::endl;
            redisFree(context);
        }
        else {
            std::cerr << "Specific connection error: can't allocate redis context" << std::endl;
        }
        exit(1);
    }
}

RedisClient::~RedisClient()
{
    if (context)
        redisFree(context);
}

void RedisClient::SelectDB(int dbIndex) 
{
    redisReply* reply = static_cast<redisReply*>(redisCommand(context, "SELECT %d", dbIndex));
    if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) {
        std::cerr << "SELECT command failed: " << (reply ? reply->str : "No reply") << std::endl;
    }
    freeReplyObject(reply);
}

std::string RedisClient::Get(const std::string& key)
{
    redisReply* reply = static_cast<redisReply*>(redisCommand(context, "GET %s", key.c_str()));
    if (reply == nullptr) {
        std::cerr << "GET command failed" << std::endl;
        return "";
    }
    std::string value = reply->str ? reply->str : "";
    freeReplyObject(reply);
    return value;
}

void RedisClient::Set(const std::string& key, const std::string& value)
{
    redisReply* reply = static_cast<redisReply*>(redisCommand(context, "SET %s %s", key.c_str(), value.c_str()));
    if (reply == nullptr) {
        std::cerr << "SET command failed" << std::endl;
    }
    freeReplyObject(reply);
}

void RedisClient::Del(const std::string& key) 
{
    redisReply* reply = static_cast<redisReply*>(redisCommand(context, "DEL %s", key.c_str()));
    if (reply == nullptr) {
        std::cerr << "DEL command failed" << std::endl;
    }
    freeReplyObject(reply);
}

void RedisClient::FlushDB() {
    redisReply* reply = static_cast<redisReply*>(redisCommand(context, "FLUSHDB"));
    if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) {
        std::cerr << "FLUSHDB command failed: " << (reply ? reply->str : "No reply") << std::endl;
    }
    freeReplyObject(reply);
}