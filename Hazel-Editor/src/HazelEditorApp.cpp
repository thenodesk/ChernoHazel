#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace Hazel {

	class HazelEditor : public Application
	{
	public:
		HazelEditor()
			: Application("Hazel Editor")
		{
			//PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}

		~HazelEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new HazelEditor();
	}

}
