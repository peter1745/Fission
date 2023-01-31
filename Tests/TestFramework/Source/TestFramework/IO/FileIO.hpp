#pragma once

#include "ImmutableBuffer.hpp"

#include <filesystem>

namespace TestFramework {

	class FileIO
	{
	public:
		static bool ReadFile(const std::filesystem::path& InPath, ImmutableBuffer& OutBuffer);
		static bool ReadFileText(const std::filesystem::path& InPath, std::string& OutText);

	};

}
