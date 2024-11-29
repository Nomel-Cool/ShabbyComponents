#pragma once
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <filesystem>
#include <string>
#include "XMLParser.h"
#include "JSONParser.h"

/// <summary>
/// 封装filesystem库，用于
/// 1. 查询某目录下的指定文件数量，种类，以及访问权限
/// 2. 追加，插入文本到文件（可指定写入的字是Unicode还是utf-8）
/// 3. 读取指定范围内的文本内容到内存（可指定写入的字是Unicode还是utf-8）
/// </summary>
class FileManager
{
public:
	FileManager() {}
    JsonParser GetJsonParser()
    {
        return json_parser;
    }
	/// <summary>
	/// XML反序列化为类始终是会需要一些代价的，DeSerialize函数通过map存储关注的string字段（详见DeSerialize说明）
	/// 从而使得XML可以利用处理string字段来与其它类沟通
	/// 但是TransXml2Class并不知道转换的协议，也不应该知道，这部分的代价由可由XML反序列化的类实现来承担，即FillUp函数以及其它类内可能需要的信号量
	/// 也许有更优雅的处理方式，但这样实现在模块分离时至少不会影响FileManage类以及可反序列化类（因为FillUp函数和对应的信号量可以被弃用）
	/// </summary>
	/// <typeparam name="StructuringClass">定义了公开的FillUp的可序列化类</typeparam>
	/// <param name="xml_path">xml文件的路径</param>
	/// <param name="target_obj">可序列化类的实例</param>
	/// <returns>反序列化成功target_obj被填充为true，否则false</returns>
    template<typename StructuringClass>
    bool TransXml2Class(std::string xml_path, StructuringClass& target_obj)
    {
        auto deserialized_data = xml_parser.DeSerialize(xml_path);
        if (!deserialized_data.empty())
        {
            try {
                if constexpr (std::is_invocable_v<decltype(&StructuringClass::FillUp), StructuringClass, std::string>)
                    for (const auto& json_string : deserialized_data)
                        target_obj.FillUp(json_string);
                else
                    throw std::logic_error("The class does not implement the required FillUp(std::string) function.");
                return true;
            }
            catch (const std::exception& e) {
                std::cerr << "Exception occurred: " << e.what() << std::endl;
                return false;
            }
        }
        else
        {
            std::cerr << "DeSerialization failed or returned empty data." << std::endl;
            return false;
        }
    }

private:
	XmlParser xml_parser;
    JsonParser json_parser;
};

#endif