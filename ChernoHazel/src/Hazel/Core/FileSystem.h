#pragma once

#include "Hazel/Core/Buffer.h"

namespace Hazel {

	// TODO: platforms
	class FileSystem
	{
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};
}
