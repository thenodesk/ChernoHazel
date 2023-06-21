#pragma once

#include "Hazel/Renderer/Texture.h"

namespace Hazel {

	struct MSDFData;

	class Font
	{
	public:
		Font(const std::filesystem::path& font);
		~Font();

		const MSDFData* GetMSDFData() const { return m_Data; }
		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static Ref<Font> GetDefault();

	private:
		MSDFData* m_Data = nullptr;
		Ref<Texture2D> m_AtlasTexture;
	};
}
