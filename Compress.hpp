#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <stdexcept>
#include <sstream>
#include <zlib.h>
#include "Define.h"

std::string Compress(std::string str);
std::string Decompress(std::string str);
