#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace Hazel {

	class Hazelnut: public Application
	{
	public:
		Hazelnut()
			: Application("Hazelnut")
		{
			//PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}

		~Hazelnut()
		{

		}
	};

	Application* CreateApplication()
	{
		return new Hazelnut();
	}

}
