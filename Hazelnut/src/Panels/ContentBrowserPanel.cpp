#include "hzpch.h"
#include "ContentBrowserPanel.h"

#include "Hazel/Project/Project.h"

#include <imgui/imgui.h>

namespace Hazel {

	ContentBrowserPanel::ContentBrowserPanel()
		: m_BaseDir(Project::GetAssetDirectory()), m_CurrentDir(m_BaseDir)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDir != m_BaseDir)
		{
			if (ImGui::Button("<-"))
				m_CurrentDir = m_CurrentDir.parent_path();
		}

		static float padding = 6.0f;
		static float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);

		ImGui::Columns(columnCount >= 1 ? columnCount : 1, (const char*)0, false);

		ImGuiIO& io = ImGui::GetIO();
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDir))
		{

			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();

			if (ImGui::BeginDragDropSource())
			{
				//auto relativePath = Project::GetAssetFileSystemPath(path);
				const wchar_t* itemPath = path.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && io.MouseDownWasDoubleClick[ImGuiMouseButton_Left])
			{
				if (directoryEntry.is_directory())
					m_CurrentDir /= path.filename();
			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 8.0f, 128.0f);
		ImGui::SliderFloat("Padding", &padding, 0.0f, 16.0f);

		ImGui::End();
	}

}
