#ifndef AUTOINDEXGENERATOR_HPP
#define AUTOINDEXGENERATOR_HPP

#include <Logger.hpp>

#include <fstream>
#include <string>

namespace AutoIndexGenerator
{
std::fstream OpenAutoIndex(std::string directory, const std::string uri);
std::string AutoIndexGenerator(const std::string dir);
} // namespace AutoIndexGenerator

#endif // !AUTOINDEXGENERATOR_HPP
