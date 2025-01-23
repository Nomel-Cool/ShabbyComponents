#pragma once
#ifndef TINYXML_IMPL_H
#define TINYXML_IMPL_H

#include <memory>

#include "IXMLStructure.h"

#include "tinyxml2.h"

class TinyXMLAttributeAdapter : public IXMLAttribute
{
public:
	TinyXMLAttributeAdapter(const tinyxml2::XMLAttribute* init_attribute);
	const char* Name() const override;
	const char* Value() const override;
	std::unique_ptr<IXMLAttribute> Next() const override;
private:
	tinyxml2::XMLAttribute* attribute;
};

class TinyXMLTextAdapter : public IXMLText
{
public:
	TinyXMLTextAdapter(const tinyxml2::XMLText* init_text);
	const char* Value() const override;
private:
	tinyxml2::XMLText* text;
};

class TinyXMLNodeAdapter : public IXMLNode
{
public:
	TinyXMLNodeAdapter(const tinyxml2::XMLNode* init_node);
	const char* Name() const override;
	std::unique_ptr<IXMLNode> FirstChild() override;
	std::unique_ptr<IXMLNode> NextSibling() override;
	std::unique_ptr<IXMLAttribute> FirstAttribute() override;
	std::unique_ptr<IXMLNode> InsertFirstChild(std::unique_ptr<IXMLNode> first_node) override;
	std::unique_ptr<IXMLNode> InsertAfterChild(std::unique_ptr<IXMLNode> former_node, std::unique_ptr<IXMLNode> later_node) override;
	void InsertEndChild(std::unique_ptr<IXMLNode> last_node) override;
	void SetAttribute(const char* key, const char* value) override;
	std::unique_ptr<IXMLText> GetText() override;
	bool NoChildren() const override;
private:
	tinyxml2::XMLNode* node;
};


class TinyXMLDocumentAdapter : public IXMLDocument
{
public:
	TinyXMLDocumentAdapter();
	TinyXMLDocumentAdapter(const tinyxml2::XMLDocument* init_doc);
	~TinyXMLDocumentAdapter() override;
	bool LoadFile(const char* path) override;
	bool SaveFile(const char* path) override;
	std::unique_ptr<IXMLNode> NewRoot(const char* node_name) override;
	std::unique_ptr<IXMLNode> NewNode(const char* node_name) override;
	std::unique_ptr<IXMLNode> GetRoot() override;
private:
	tinyxml2::XMLDocument* doc;
};
class TinyXMLDocumentFactory : public IXMLDocumentFactory 
{
public:
	std::unique_ptr<IXMLDocument> CreateDocument() override;
};
#endif // !TINYXML_IMPL_H
