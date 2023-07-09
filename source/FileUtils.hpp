#ifndef _FILE_UTILS_
#define _FILE_UTILS_

#include <filesystem>
#include <string>

std::string ReadFileAsString(std::filesystem::path path);

#endif