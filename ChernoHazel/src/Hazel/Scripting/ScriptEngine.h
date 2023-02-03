#pragma once

#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/Entity.h"

#include <filesystem>
#include <string>

extern "C" {
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
    typedef struct _MonoImage MonoImage;
    typedef struct _MonoClassField MonoClassField;
}

namespace Hazel {

    enum class ScriptFieldType
    {
        None = 0,
        Float, Double,
        Bool, Char, Short, Int, Long,
        Byte, UShort, UInt, ULong,
        Vector2, Vector3, Vector4,
        Entity
    };

    struct ScriptField
    {
        ScriptFieldType Type;
        std::string Name;
        
        MonoClassField* ClassField;
    };

    // ScriptField + data storage
    struct ScriptFieldInstance
    {
        ScriptField Field;

        ScriptFieldInstance()
        {
            memset(m_Buffer, 0, sizeof(m_Buffer));
        }

        template<typename T>
        T GetValue()
        {
            static_assert(sizeof(T) <= 8, "Type too large!");
            return *(T*)m_Buffer;
        }

        template<typename T>
        void SetValue(const T& value)
        {
            static_assert(sizeof(T) <= 8, "Type too large!");
            memcpy(m_Buffer, &value, sizeof(T));
        }

    private:
        uint8_t m_Buffer[8];

        friend class ScriptEngine;
        friend class ScriptInstance;
    };

    using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

    class ScriptClass
    {
    public:
        ScriptClass() = default;
        ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

        MonoObject* Instantiate();
        MonoMethod* GetMethod(const std::string& name, int parameterCount);
        MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

        const std::unordered_map<std::string, ScriptField>& GetFields() const { return m_Fields; }

    private:
        std::string m_ClassNamespace;
        std::string m_ClassName;

        std::unordered_map<std::string, ScriptField> m_Fields;

        MonoClass* m_MonoClass = nullptr;

        friend class ScriptEngine;
    };

    class ScriptInstance
    {
    public:
        ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

        void InvokeOnCreate();
        void InvokeOnUpdate(float ts);

        Ref<ScriptClass> GetScriptClass() const { return m_ScriptClass; }

        template<typename T>
        T GetFieldValue(const std::string& name)
        {
            static_assert(sizeof(T) <= 8, "Type too large!");

            bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
            if (!success)
                return T();

            return *(T*)s_FieldValueBuffer;
        }

        template<typename T>
        void SetFieldValue(const std::string& name, T value)
        {
            static_assert(sizeof(T) <= 8, "Type too large!");
            SetFieldValueInternal(name, &value);
        }

    private:
        bool GetFieldValueInternal(const std::string& name, void* buffer);
        bool SetFieldValueInternal(const std::string& name, const void* value);

    private:
        Ref<ScriptClass> m_ScriptClass;

        MonoObject* m_Instance = nullptr;
        MonoMethod* m_Constructor = nullptr;
        MonoMethod* m_OnCreateMethod = nullptr;
        MonoMethod* m_OnUpdateMethod = nullptr;

        inline static char s_FieldValueBuffer[8];

        friend class ScriptEngine;
        friend struct ScriptFieldInstance;
    };

    class ScriptEngine
    {
    public:
        static void Init();
        static void Shutdown();

        static void LoadAssembly(const std::filesystem::path& filepath);
        static void LoadAppAssembly(const std::filesystem::path& filepath);
        
        static void OnRuntimeStart(Scene* scene);
        static void OnRuntimeStop();

        static bool EntityClassExists(const std::string& fullClassName);
        static void OnCreateEntity(Entity entity);
        static void OnUpdateEntity(Entity entity, Timestep ts);

        static Scene* GetSceneContext();
        static Ref<ScriptInstance> GetEntityScriptInstance(UUID entityID);

        static Ref<ScriptClass> GetEntityClass(const std::string& name);
        static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();
        static ScriptFieldMap& GetScriptFieldMap(Entity entity);

        static MonoImage* GetCoreAssemblyImage();

    private:
        static void InitMono();
        static void ShutdownMono();

        static MonoObject* InstantiateClass(MonoClass* monoClass);
        static void LoadAssemblyClasses();

        friend class ScriptClass;
        friend class ScriptGlue;
    };
}
