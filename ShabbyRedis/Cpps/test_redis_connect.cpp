#include "RedisClient.h"

int main()
{
    RedisClient client("127.0.0.1", 6379);
    client.selectDB(0); // 选择数据库1
    // 测试模板 set 方法
    auto intToString = [](const int& value) { return std::to_string(value); };
    auto floatToString = [](const float& value) { return std::to_string(value); };
    auto customToString = [](const std::pair<int, std::string>& value) {
        return std::to_string(value.first) + ":" + value.second;
        };

    client.set<int>("int_key", 42, intToString);
    client.set<float>("float_key", 3.14f, floatToString);
    client.set<std::pair<int,std::string>>("custom_key", std::make_pair(1, "value"), customToString);

    // 测试 get 方法
    std::string value = client.get("custom_key");
    std::cout << "custom_key: " << value << std::endl;

    // 测试 del 方法
    client.del("custom_key");
    value = client.get("custom_key");
    std::cout << "custom_key after delete: " << value << std::endl;

    return 0;
}