#pragma once
#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include "crc.h"
class Encryption
{
	std::string CRCCheckSum(std::string str);
};

#endif // !ENCRYPTION_H
