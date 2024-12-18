#include "XMLParser.h"

TraverseResult XmlParser::DeSerialize(std::string xml_path)
{
    tinyxml2::XMLDocument xmlDocument;
    tinyxml2::XMLError error = xmlDocument.LoadFile(xml_path.c_str());
	TraverseResult traverse_result;
	if (error == tinyxml2::XML_SUCCESS)
		traverse_result = TraversingXML(&xmlDocument);
	return traverse_result;
}

bool XmlParser::Serialize(const std::string& xml_path, tinyxml2::XMLDocument& doc)
{
    // 尝试保存 XML 文档到指定路径
    tinyxml2::XMLError result = doc.SaveFile(xml_path.c_str());
    if (result != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to save XML file: " << xml_path << ", Error Code: " << result << std::endl;
        return false;
    }

    std::cout << "Successfully saved XML to: " << xml_path << std::endl;
    return true;
}


TraverseResult XmlParser::TraversingXML(tinyxml2::XMLNode* node)
{
    TraverseResult traverse_result;
    std::stringstream json_stream;

    if (node == nullptr)
        return traverse_result;

    if (node->ToElement())
    {
        auto element = dynamic_cast<tinyxml2::XMLElement*>(node);
        json_stream << "{ \"" << element->Name() << "\" : [ ";

        const tinyxml2::XMLAttribute* attribute = element->FirstAttribute();
        while (attribute != nullptr)
        {
            json_stream << "{ \"" << attribute->Name() << "\" : \"" << attribute->Value() << "\" }, ";
            attribute = attribute->Next();
        }

        if (node->ToText())
        {
            auto text = dynamic_cast<tinyxml2::XMLText*>(node);
            json_stream << "{ \"_text_\" : \"" << text->Value() << "\" } ";
        }

        // Remove the trailing comma and space if they exist
        std::string json_string = json_stream.str();
        if (json_string.back() == ' ')
        {
            json_string.pop_back();
            if (json_string.back() == ',')
            {
                json_string.pop_back();
            }
        }

        json_string += " ] }";
        traverse_result.push_back(json_string);
        json_stream.str("");  // Clear the stringstream for the next element
    }

    if (!node->NoChildren())
    {
        tinyxml2::XMLNode* child = node->FirstChild();
        while (child != nullptr)
        {
            TraverseResult child_result = TraversingXML(child);
            traverse_result.insert(traverse_result.end(), child_result.begin(), child_result.end());
            child = child->NextSibling();
        }
    }

    return traverse_result;
}