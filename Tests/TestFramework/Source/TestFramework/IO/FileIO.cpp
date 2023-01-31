#include "FileIO.hpp"

#include <fstream>

namespace TestFramework {

	bool FileIO::ReadFile(const std::filesystem::path& InPath, ImmutableBuffer& OutBuffer)
	{
		std::ifstream stream(InPath, std::ios::ate | std::ios::binary);
		if (!stream.is_open())
			return false;

		uint32_t streamSize = uint32_t(stream.tellg());

		stream.seekg(std::ios::beg);

		if (streamSize == 0)
			return false;

		Byte* tempBuffer = new Byte[streamSize];
		stream.read(reinterpret_cast<char*>(tempBuffer), streamSize);
		OutBuffer = ImmutableBuffer(tempBuffer, streamSize);
		stream.close();
		return true;
	}

	bool FileIO::ReadFileText(const std::filesystem::path& InPath, std::string& OutText)
	{
		std::ifstream stream(InPath);
		if (!stream.is_open())
			return false;

		std::stringstream strStream;
		strStream << stream.rdbuf();
		OutText = strStream.str();
		return true;
	}

}
