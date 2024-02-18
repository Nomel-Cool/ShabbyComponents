#pragma once
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <filesystem>
/// <summary>
/// 封装filesystem库，用于
/// 1. 查询某目录下的指定文件数量，种类，以及访问权限
/// 2. 追加，插入文本到文件（可指定写入的字是Unicode还是utf-8）
/// 3. 读取指定范围内的文本内容到内存（可指定写入的字是Unicode还是utf-8）
/// </summary>
class FileManager
{
public:
	FileManager() {}
private:

};

#endif