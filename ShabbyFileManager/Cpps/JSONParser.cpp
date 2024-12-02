#include "JSONParser.h"

bool JsonParser::DeSerialize(std::string json_string, json& json_obj)
{
	bool isLegal = json::accept(json_string);
	if (!isLegal)
		return false;
	json_obj = json::parse(json_string.c_str());
	return true;
}

std::string JsonParser::Serialize(const json& json_obj)
{
	try {
		return json_obj.dump();
	}
	catch (json::type_error& e) {
		std::cerr << "JSON serialization error: " << e.what() << std::endl;
		return "";
	}
}