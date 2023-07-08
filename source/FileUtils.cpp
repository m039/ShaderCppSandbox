#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "FileUtils.hpp"

namespace fs = std::filesystem;

std::string ReadFileAsString(fs::path path)
{
    std::ifstream f(path.c_str());

    if (f)
    {
        std::ostringstream ss;
        ss << f.rdbuf();
        return ss.str();
    }

    return std::string();
}