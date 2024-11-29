#pragma once
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <filesystem>
#include <string>
#include "XMLParser.h"
#include "JSONParser.h"

/// <summary>
/// ��װfilesystem�⣬����
/// 1. ��ѯĳĿ¼�µ�ָ���ļ����������࣬�Լ�����Ȩ��
/// 2. ׷�ӣ������ı����ļ�����ָ��д�������Unicode����utf-8��
/// 3. ��ȡָ����Χ�ڵ��ı����ݵ��ڴ棨��ָ��д�������Unicode����utf-8��
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
	/// XML�����л�Ϊ��ʼ���ǻ���ҪһЩ���۵ģ�DeSerialize����ͨ��map�洢��ע��string�ֶΣ����DeSerialize˵����
	/// �Ӷ�ʹ��XML�������ô���string�ֶ����������๵ͨ
	/// ����TransXml2Class����֪��ת����Э�飬Ҳ��Ӧ��֪�����ⲿ�ֵĴ����ɿ���XML�����л�����ʵ�����е�����FillUp�����Լ��������ڿ�����Ҫ���ź���
	/// Ҳ���и����ŵĴ���ʽ��������ʵ����ģ�����ʱ���ٲ���Ӱ��FileManage���Լ��ɷ����л��ࣨ��ΪFillUp�����Ͷ�Ӧ���ź������Ա����ã�
	/// </summary>
	/// <typeparam name="StructuringClass">�����˹�����FillUp�Ŀ����л���</typeparam>
	/// <param name="xml_path">xml�ļ���·��</param>
	/// <param name="target_obj">�����л����ʵ��</param>
	/// <returns>�����л��ɹ�target_obj�����Ϊtrue������false</returns>
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