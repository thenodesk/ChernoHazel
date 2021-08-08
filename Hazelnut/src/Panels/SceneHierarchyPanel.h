#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/Entity.h"

namespace Hazel {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		template<typename T, typename Functor>
		void DrawComponent(const char* name, Entity ent, Functor func)
		{
			if (ent.HasComponent<T>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, name))
				{
					func();

					ImGui::TreePop();
				}
				ImGui::Separator();
			}
		}
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}
