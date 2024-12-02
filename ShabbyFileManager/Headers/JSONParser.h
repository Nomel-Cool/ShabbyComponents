#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <iostream>
#include <string>

#include "json.hpp"
using json = nlohmann::json;

class JsonParser
{
public:
	virtual bool DeSerialize(std::string json_string, json& json_obj);

	virtual std::string Serialize(const json& json_obj);

	template<typename T>
	T GetValue(const json& json_obj, const std::string& key);

	template<typename T> 
	void SetValue(json& json_obj, const std::string& key, const T& value);
protected:

private:

};

#endif // !JSON_PARSER_H
