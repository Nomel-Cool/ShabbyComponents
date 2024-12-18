#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "tinyxml2.h"

using namespace tinyxml2;

using TraverseResult = std::vector<std::string>;
class XmlParser
{
public:
    TraverseResult DeSerialize(std::string xml_path);

	bool Serialize(const std::string& xml_path, XMLDocument& doc);
protected:
	/// <summary>
	/// ����ʱֻ����xml�ĵ��е� <tag attribute="value1">text</tag> �ṹ
	/// ��ɢ�б���ȼ۵�Json�ṹ��ͼ��{ "tag" : [ { "attribute" : value } ] }
	/// </summary>
	/// <param name="node">���ڱ���XML�ļ��Ľڵ㣬ģ����DOM����Ψһ�ĸ��ڵ㿪ʼ</param>
	TraverseResult TraversingXML(XMLNode* node);

private:

};

#endif // !XML_PARSER_H
