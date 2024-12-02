#include "FileManager.h"

bool FileManager::TransStr2JsonObject(const std::string& json_string, json& json_obj)
{
	if (json::accept(json_string))
	{
		json_obj = json::parse(json_string);
		return true;
	}
	else
	{
		std::cerr << "Invalid Json string formation" << std::endl;
		return false;
	}
	return false;
}

