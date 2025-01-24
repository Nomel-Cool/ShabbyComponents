#include "TinyXMLImpl.h"

TinyXMLDocumentAdapter::TinyXMLDocumentAdapter()
{
	doc = new tinyxml2::XMLDocument();
}

TinyXMLDocumentAdapter::TinyXMLDocumentAdapter(const tinyxml2::XMLDocument* init_doc)
{
	doc = const_cast<tinyxml2::XMLDocument*>(init_doc);
}

TinyXMLDocumentAdapter::~TinyXMLDocumentAdapter()
{
	if (doc != nullptr)
		delete doc;
}

bool TinyXMLDocumentAdapter::LoadFile(const char* path)
{
	return doc->LoadFile(path) == tinyxml2::XML_SUCCESS;
}

bool TinyXMLDocumentAdapter::SaveFile(const char* path)
{
	return doc->SaveFile(path) == tinyxml2::XML_SUCCESS;
}

std::unique_ptr<shabby::IXMLNode> TinyXMLDocumentAdapter::NewRoot(const char* node_name)
{
	auto root = doc->NewElement(node_name);
	doc->InsertFirstChild(root);
	return std::make_unique<TinyXMLNodeAdapter>(root);
}

std::unique_ptr<shabby::IXMLNode> TinyXMLDocumentAdapter::GetRoot()
{
	return std::make_unique<TinyXMLNodeAdapter>(doc->RootElement());
}

tinyxml2::XMLDocument* TinyXMLDocumentAdapter::GetDocHandler()
{
	return doc;
}

TinyXMLNodeAdapter::TinyXMLNodeAdapter(const tinyxml2::XMLNode* init_node)
{
	node = const_cast<tinyxml2::XMLNode*>(init_node);
}

const char* TinyXMLNodeAdapter::Name() const
{
	auto element = dynamic_cast<tinyxml2::XMLElement*>(node);
	return element->Name();
}

std::unique_ptr<shabby::IXMLNode> TinyXMLNodeAdapter::FirstChild()
{
	return std::make_unique<TinyXMLNodeAdapter>(node->FirstChild());
}

std::unique_ptr<shabby::IXMLNode> TinyXMLNodeAdapter::NextSibling()
{
	auto next_sibling = node->NextSibling();
	if (next_sibling != nullptr)
		return std::make_unique<TinyXMLNodeAdapter>(next_sibling);
	else
		return nullptr;
}

std::unique_ptr<shabby::IXMLAttribute> TinyXMLNodeAdapter::FirstAttribute()
{
	auto element = dynamic_cast<tinyxml2::XMLElement*>(node);
	return std::make_unique<TinyXMLAttributeAdapter>(element->FirstAttribute());
}

std::unique_ptr<shabby::IXMLNode> TinyXMLNodeAdapter::InsertFirstChild(std::unique_ptr<shabby::IXMLNode> first_node)
{
	auto* adapter = dynamic_cast<TinyXMLNodeAdapter*>(first_node.get());
	node->InsertFirstChild(adapter->node);
	return std::move(first_node);
}

std::unique_ptr<shabby::IXMLNode> TinyXMLNodeAdapter::InsertAfterChild(std::unique_ptr<shabby::IXMLNode> former_node, std::unique_ptr<shabby::IXMLNode> later_node)
{
	auto* adapter_lhs = dynamic_cast<TinyXMLNodeAdapter*>(former_node.get());
	auto* adapter_rhs = dynamic_cast<TinyXMLNodeAdapter*>(later_node.get());
	node->InsertAfterChild(adapter_lhs->node, adapter_rhs->node);
	return std::move(later_node);
}

void TinyXMLNodeAdapter::InsertEndChild(std::unique_ptr<shabby::IXMLNode> last_node)
{
	auto* adapter = dynamic_cast<TinyXMLNodeAdapter*>(last_node.get());
	node->InsertEndChild(adapter->node);
}

std::unique_ptr<shabby::IXMLNode> TinyXMLNodeAdapter::NewNode(std::shared_ptr<shabby::IXMLDocument> doc, const char* node_name)
{
	auto upcast_doc = dynamic_cast<TinyXMLDocumentAdapter*>(doc.get());
	auto tinyxml_doc = upcast_doc->GetDocHandler();
	auto element_node = tinyxml_doc->NewElement(node_name);
	return std::make_unique<TinyXMLNodeAdapter>(element_node);
}

void TinyXMLNodeAdapter::SetAttribute(const char* key, const char* value)
{
	auto element = dynamic_cast<tinyxml2::XMLElement*>(node);
	element->SetAttribute(key, value);
}

std::unique_ptr<shabby::IXMLText> TinyXMLNodeAdapter::GetText()
{
	auto text = dynamic_cast<tinyxml2::XMLText*>(node);
	return std::make_unique<TinyXMLTextAdapter>(text);
}

bool TinyXMLNodeAdapter::NoChildren() const
{
	return node->NoChildren();
}

TinyXMLAttributeAdapter::TinyXMLAttributeAdapter(const tinyxml2::XMLAttribute* init_attribute)
{
	attribute = const_cast<tinyxml2::XMLAttribute*>(init_attribute);
}

const char* TinyXMLAttributeAdapter::Name() const
{
	return attribute->Name();
}

const char* TinyXMLAttributeAdapter::Value() const
{
	return attribute->Value();
}

std::unique_ptr<shabby::IXMLAttribute> TinyXMLAttributeAdapter::Next() const
{
	auto next_attr = attribute->Next();
	if(next_attr != nullptr)
		return std::make_unique<TinyXMLAttributeAdapter>(next_attr);
	return
		nullptr;
}

TinyXMLTextAdapter::TinyXMLTextAdapter(const tinyxml2::XMLText* init_text)
{
	text = const_cast<tinyxml2::XMLText*>(init_text);
}

const char* TinyXMLTextAdapter::Value() const
{
	return text->Value();
}

std::unique_ptr<shabby::IXMLDocument> TinyXMLDocumentFactory::CreateDocument()
{
	return std::make_unique<TinyXMLDocumentAdapter>();
}
