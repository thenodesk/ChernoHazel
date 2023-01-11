#include "hzpch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include "Hazel/Core/UUID.h"
#include "Hazel/Scene/Scene.h"

#include "mono/metadata/object.h"

namespace Hazel {

#define HZ_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Hazel.InternalCalls::" #Name, Name)

    static void NativeLog(MonoString* string, int value)
    {
        char* cStr = mono_string_to_utf8(string);
        std::string str(cStr);
        mono_free(cStr);

        std::cout << str << ", " << value << "\n";
    }

    static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
    {
        HZ_CORE_WARN("Value: {0}", *parameter);
        *outResult = glm::normalize(*parameter);
    }

    static float NativeLog_VectorDot(glm::vec3* parameter)
    {
        HZ_CORE_WARN("Value: {0}", *parameter);
        return glm::dot(*parameter, *parameter);
    }
    
    static void Entity_GetTranslation(UUID entityID, glm::vec3* outTranslation)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        
        Entity entity = scene->GetEntityByUUID(entityID);
        *outTranslation = entity.GetComponent<TransformComponent>().Translation;
    }
    
    static void Entity_SetTranslation(UUID entityID, glm::vec3* translation)
    {
        Scene* scene = ScriptEngine::GetSceneContext();

        Entity entity = scene->GetEntityByUUID(entityID);
        entity.GetComponent<TransformComponent>().Translation = *translation;
    }

    static bool Input_IsKeyDown(KeyCode keycode)
    {
        return Input::IsKeyPressed(keycode);
    }

    void ScriptGlue::RegisterFunctions()
    {
        // C++ functions to C#
        HZ_ADD_INTERNAL_CALL(NativeLog);
        HZ_ADD_INTERNAL_CALL(NativeLog_Vector);
        HZ_ADD_INTERNAL_CALL(NativeLog_VectorDot);
        
        HZ_ADD_INTERNAL_CALL(Entity_GetTranslation);
        HZ_ADD_INTERNAL_CALL(Entity_SetTranslation);

        HZ_ADD_INTERNAL_CALL(Input_IsKeyDown);
    }
}
