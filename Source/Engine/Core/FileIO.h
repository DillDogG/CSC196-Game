#pragma once
#include <filesystem>
#include <string>

namespace kiko
{
	std::string getFilePath();

	bool fileExists(const std::filesystem::path& path);
	bool getFileSize(const std::filesystem::path& path, size_t& size);
	bool readFile(const std::filesystem::path& pathname, std::string& buffer);
}
