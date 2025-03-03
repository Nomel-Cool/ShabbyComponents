#include "XMLParser.h"

bool XMLSerializer::Serialize(const std::string& xml_path, shabby::IXMLDocument& doc)
{
    // 尝试保存 XML 文档到指定路径
    bool result = doc.SaveFile(xml_path.c_str());
    if (!result) {
        std::cerr << "Failed to save XML file: " << xml_path << std::endl;
        return false;
    }

    std::cout << "Successfully saved XML to: " << xml_path << std::endl;
    return true;
}

std::vector<std::string> XMLTraverser::TraversingXML(shabby::IXMLNode& node)
{
    std::vector<std::string> traverse_result;
    std::stringstream json_stream;

    json_stream << "{ \"" << node.Name() << "\" : [ ";

    std::unique_ptr<shabby::IXMLAttribute> attribute = node.FirstAttribute();
    while (attribute != nullptr)
    {
        json_stream << "{ \"" << attribute->Name() << "\" : \"" << attribute->Value() << "\" }, ";
        attribute = std::move(attribute->Next());
    }

    json_stream << "{ \"_text_\" : \"" << node.GetText() << "\" } ";

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

    if (!node.NoChildren())
    {
        auto child = node.FirstChild();
        while (child != nullptr)
        {
            std::vector<std::string> child_result = TraversingXML(*child);
            traverse_result.insert(traverse_result.end(), child_result.begin(), child_result.end());
            child = child->NextSibling();
        }
    }

    return traverse_result;
}

XMLDeserializer::XMLDeserializer(std::unique_ptr<shabby::IXMLDocumentFactory> factory)
{
    factory_ = std::move(factory);
}

std::unique_ptr<shabby::IXMLDocument> XMLDeserializer::DeSerialize(std::string xml_path)
{
    auto xmlDocument = factory_->CreateDocument();
    bool result = xmlDocument->LoadFile(xml_path.c_str());
    if (result)
        return xmlDocument;
    return nullptr;
}

XmlParser::XmlParser(std::unique_ptr<shabby::IXMLDocumentFactory> factory)
{
    up_xml_traverser = std::make_unique<XMLTraverser>();
    up_xml_serializer = std::make_unique<XMLSerializer>();
    up_xml_deserializer = std::make_unique<XMLDeserializer>(std::move(factory));
}

bool XmlParser::DoSerialization(const std::string& xml_path, shabby::IXMLDocument& doc)
{
    return up_xml_serializer->Serialize(xml_path, doc);
}

std::vector<std::string> XmlParser::DoDeserialization(const std::string& xml_path)
{
    auto abstract_node = up_xml_deserializer->DeSerialize(xml_path);
    if (abstract_node == nullptr)
        return std::vector<std::string>();
    return up_xml_traverser->TraversingXML(*abstract_node->GetRoot());
}
