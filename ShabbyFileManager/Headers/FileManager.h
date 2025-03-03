#pragma once

#ifndef SHABBY_FILE_MANAGER_EXPORTS
#define SHABBY_CLASS __declspec(dllimport)
#define SHABBY_TEMPLATE
#else //EXPORT
#define SHABBY_CLASS __declspec(dllexport)
#define SHABBY_TEMPLATE __declspec(dllexport)
#endif

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <filesystem>
#include <string>
#include <functional>
#include <iostream>

#include "XMLParser.h"
#include "JSONParser.h"

class  SHABBY_CLASS FileManager
{
public:
    FileManager() = default;
    FileManager(std::unique_ptr<shabby::IXMLDocumentFactory> factory) 
    { 
        xml_parser = std::make_unique<XmlParser>(std::move(factory));
    }
    bool TransStr2JsonObject(const std::string& json_string, json& json_obj);

    template<typename StructuringClass>
    bool TransXml2Class(const std::string& xml_path, StructuringClass& target_obj, std::function<bool(const std::string&, StructuringClass&)> FillUp)
    {
        auto deserialized_data = xml_parser->DoDeserialization(xml_path);
        if (!deserialized_data.empty())
        {
            try {
                for (const auto& json_string : deserialized_data)
                    FillUp(json_string, target_obj);
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

    template<typename StructuringClass>
    bool TransClass2Xml(StructuringClass& target_obj, const std::string& xml_path, const std::function<bool(std::shared_ptr<shabby::IXMLDocument>, StructuringClass&)>& Depart)
    {
        try {
            // ����һ���յ� XML �ĵ�
            std::shared_ptr<shabby::IXMLDocument> doc = std::make_shared<TinyXMLDocumentAdapter>();

            // ͨ�� Depart �ص�������� XML �ĵ�
            if (!Depart(doc, target_obj)) {
                std::cerr << "Depart function failed to generate XML data." << std::endl;
                return false;
            }

            // ���� Serialize ���浽ָ��·��
            if (!xml_parser->DoSerialization(xml_path, *doc)) {
                std::cerr << "Failed to serialize XML document to path: " << xml_path << std::endl;
                return false;
            }

            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Exception occurred during TransClass2Xml: " << e.what() << std::endl;
            return false;
        }
    }

private:
    std::unique_ptr<XmlParser> xml_parser;
    JsonParser json_parser;
};

#endif
