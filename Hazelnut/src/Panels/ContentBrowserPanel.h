#pragma once

#include <filesystem>

#include "Hazel/Renderer/Texture.h"

namespace Hazel {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_BaseDir;
		std::filesystem::path m_CurrentDir;
		Ref<Texture2D> m_DirectoryIcon, m_FileIcon;
	};

}
