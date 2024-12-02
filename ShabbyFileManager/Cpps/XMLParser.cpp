#include "XMLParser.h"

TraverseResult XmlParser::DeSerialize(std::string xml_path)
{
	XMLDocument xmlDocument;
	XMLError error = xmlDocument.LoadFile(xml_path.c_str());
	TraverseResult traverse_result;
	if (error == XML_SUCCESS)
		traverse_result = TraversingXML(&xmlDocument);
	return traverse_result;
}

bool XmlParser::Serialize(std::string xml_path)
{
    XMLDocument xmlDocument;
    XMLNode* root = xmlDocument.NewElement("Root");
    xmlDocument.InsertFirstChild(root);

    XMLElement* element = xmlDocument.NewElement("Example");
    element->SetAttribute("attribute", "value");
    element->SetText("This is a test");
    root->InsertEndChild(element);

    XMLError error = xmlDocument.SaveFile(xml_path.c_str());
    return error == XML_SUCCESS;
}

TraverseResult XmlParser::TraversingXML(XMLNode* node)
{
    TraverseResult traverse_result;
    std::stringstream json_stream;

    if (node == nullptr)
        return traverse_result;

    if (node->ToElement())
    {
        auto element = dynamic_cast<XMLElement*>(node);
        json_stream << "{ \"" << element->Name() << "\" : [ ";

        const XMLAttribute* attribute = element->FirstAttribute();
        while (attribute != nullptr)
        {
            json_stream << "{ \"" << attribute->Name() << "\" : \"" << attribute->Value() << "\" }, ";
            attribute = attribute->Next();
        }

        if (node->ToText())
        {
            auto text = dynamic_cast<XMLText*>(node);
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
        XMLNode* child = node->FirstChild();
        while (child != nullptr)
        {
            TraverseResult child_result = TraversingXML(child);
            traverse_result.insert(traverse_result.end(), child_result.begin(), child_result.end());
            child = child->NextSibling();
        }
    }

    return traverse_result;
}