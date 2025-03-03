#pragma once

#ifndef SHABBY_FILE_MANAGER_EXPORTS
#define SHABBY_CLASS __declspec(dllimport)
#define SHABBY_TEMPLATE
#else //EXPORT
#define SHABBY_CLASS __declspec(dllexport)
#define SHABBY_TEMPLATE __declspec(dllexport)
#endif

#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "TinyXMLImpl.h"

class SHABBY_CLASS XMLTraverser
{
public:
	/// <summary>
	/// ����ʱֻ����xml�ĵ��е� <tag attribute="value1">text</tag> �ṹ
	/// ��ɢ�б���ȼ۵�Json�ṹ��ͼ��{ "tag" : [ { "attribute" : value } ] }
	/// </summary>
	/// <param name="node">���ڱ���XML�ļ��Ľڵ㣬ģ����DOM����Ψһ�ĸ��ڵ㿪ʼ</param>
	std::vector<std::string> TraversingXML(shabby::IXMLNode& node);
};

class SHABBY_CLASS XMLSerializer
{
public:
	/// <summary>
	/// ���ṹ�������XML���ݳ־û���ָ���ļ�
	/// </summary>
	/// <param name="xml_path">�ļ�Ŀ¼</param>
	/// <param name="doc">�ṹ����XML����</param>
	/// <returns>�ɹ�����true������false</returns>
	bool Serialize(const std::string& xml_path, shabby::IXMLDocument& doc);
};

class SHABBY_CLASS XMLDeserializer
{
public:
	XMLDeserializer(std::unique_ptr<shabby::IXMLDocumentFactory> factory);
	/// <summary>
	/// ���ظ����� XML �ļ� URL
	/// </summary>
	/// <param name="xml_path"> xml �ļ�·�� </param>
	/// <returns>�ɹ��򷵻��ļ�ָ�룬�����ָ��</returns>
	std::unique_ptr<shabby::IXMLDocument> DeSerialize(std::string xml_path);
private:
	std::unique_ptr<shabby::IXMLDocumentFactory> factory_;
};

class SHABBY_CLASS  XmlParser
{
public:
	XmlParser(std::unique_ptr<shabby::IXMLDocumentFactory> factory);
	bool DoSerialization(const std::string& xml_path, shabby::IXMLDocument& doc);
	std::vector<std::string> DoDeserialization(const std::string& xml_path);
private:
	std::unique_ptr<XMLTraverser> up_xml_traverser;
	std::unique_ptr<XMLSerializer> up_xml_serializer;
	std::unique_ptr<XMLDeserializer> up_xml_deserializer;
};

#endif // !XML_PARSER_H
