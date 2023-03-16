#include "hzpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"
#include "Hazel/Scripting/ScriptEngine.h"
#include "Hazel/Project/Project.h"

#include <fstream>

#include <yaml-cpp/yaml.h>

namespace YAML {

    template<>
    struct convert<glm::vec2>
    {
        static Node encode(const glm::vec2& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Hazel::UUID>
    {
        static Node encode(const Hazel::UUID& uuid)
        {
            Node node;
            node.push_back((uint64_t)uuid);
            return node;
        }

        static bool decode(const Node& node, Hazel::UUID& uuid)
        {
            uuid = node.as<uint64_t>();
            return true;
        }
    };
}

namespace Hazel {

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    static std::string RigidBody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType)
    {
        switch (bodyType)
        {
            case Rigidbody2DComponent::BodyType::Static:    return "Static";
            case Rigidbody2DComponent::BodyType::Dynamic:   return "Dynamic";
            case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
        }
        HZ_CORE_ASSERT(false, "Unknown body type");
        return {};
    }

    static Rigidbody2DComponent::BodyType RigidBody2DBodyTypeFromString(std::string& bodyTypeStr)
    {
        if (bodyTypeStr == "Static")    return Rigidbody2DComponent::BodyType::Static;
        if (bodyTypeStr == "Dynamic")   return Rigidbody2DComponent::BodyType::Dynamic;
        if (bodyTypeStr == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;

        HZ_CORE_ASSERT(false, "Unknown body type");
        return Rigidbody2DComponent::BodyType::Static;
    }

    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
        : m_Scene(scene)
    {
    }

    static void SerializeEntity(YAML::Emitter& out, Entity entity)
    {
        HZ_CORE_ASSERT(entity.HasComponent<IDComponent>(), "Entity has no ID!");

        out << YAML::BeginMap; // Entity
        out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

        if (entity.HasComponent<TagComponent>())
        {
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap; // TagComponent

            auto& tag = entity.GetComponent<TagComponent>().Tag;
            out << YAML::Key << "Tag" << YAML::Value << tag;

            out << YAML::EndMap; // TagComponent
        }

        if (entity.HasComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap; // TransformComponent

            auto& tc = entity.GetComponent<TransformComponent>();
            out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
            out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
            out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

            out << YAML::EndMap; // TransformComponent
        }

        if (entity.HasComponent<CameraComponent>())
        {
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap; // CameraComponent

            auto& cameraComponent = entity.GetComponent<CameraComponent>();
            auto& camera = cameraComponent.Camera;

            out << YAML::Key << "Camera" << YAML::Value;
            out << YAML::BeginMap; // Camera
            out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
            out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
            out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
            out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
            out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
            out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
            out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
            out << YAML::EndMap; // Camera

            out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
            out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

            out << YAML::EndMap; // CameraComponent
        }

        if (entity.HasComponent<ScriptComponent>())
        {
            out << YAML::Key << "ScriptComponent";
            out << YAML::BeginMap; // ScriptComponent

            auto& sc = entity.GetComponent<ScriptComponent>();
            out << YAML::Key << "ClassName" << YAML::Value << sc.ClassName;

            // Fields
            Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(sc.ClassName);
            const auto& fields = entityClass->GetFields();
            if (fields.size() > 0)
            {
                out << YAML::Key << "ScriptFields" << YAML::Value;
                out << YAML::BeginSeq; // ScriptFields
                auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
                for (const auto& [name, field] : fields)
                {
                    if (entityFields.find(name) == entityFields.end())
                        continue;

                    // - Name: ExampleField
                    //   Type: Int
                    //   Data: 5

                    out << YAML::BeginMap; // ScriptField
                    out << YAML::Key << "Name" << YAML::Value << name;
                    out << YAML::Key << "Type" << YAML::Value << Utils::ScriptFieldTypeToString(field.Type);

                    out << YAML::Key << "Data" << YAML::Value;
                    ScriptFieldInstance& scriptField = entityFields.at(name);
                    switch (field.Type)
                    {
                        case ScriptFieldType::Float:   out << scriptField.GetValue<float>(); break;
                        case ScriptFieldType::Double:  out << scriptField.GetValue<double>(); break;
                        case ScriptFieldType::Bool:    out << scriptField.GetValue<bool>(); break;
                        case ScriptFieldType::Char:    out << scriptField.GetValue<int8_t>(); break;
                        case ScriptFieldType::Short:   out << scriptField.GetValue<int16_t>(); break;
                        case ScriptFieldType::Int:     out << scriptField.GetValue<int32_t>(); break;
                        case ScriptFieldType::Long:    out << scriptField.GetValue<int64_t>(); break;
                        case ScriptFieldType::Byte:    out << scriptField.GetValue<uint8_t>(); break;
                        case ScriptFieldType::UShort:  out << scriptField.GetValue<uint16_t>(); break;
                        case ScriptFieldType::UInt:    out << scriptField.GetValue<uint32_t>(); break;
                        case ScriptFieldType::ULong:   out << scriptField.GetValue<uint64_t>(); break;
                        case ScriptFieldType::Vector2: out << scriptField.GetValue<glm::vec2>(); break;
                        case ScriptFieldType::Vector3: out << scriptField.GetValue<glm::vec3>(); break;
                        case ScriptFieldType::Vector4: out << scriptField.GetValue<glm::vec4>(); break;
                        case ScriptFieldType::Entity:  out << scriptField.GetValue<UUID>(); break;

                    }
                    out << YAML::EndMap; // ScriptField
                }
                out << YAML::EndSeq; // ScriptFields
            }

            out << YAML::EndMap; // ScriptComponent
        }

        if (entity.HasComponent<SpriteRendererComponent>())
        {
            out << YAML::Key << "SpriteRendererComponent";
            out << YAML::BeginMap; // SpriteRendererComponent

            auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
            out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
            out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;
            
            if (spriteRendererComponent.Texture)
                out << YAML::Key << "TexturePath" << YAML::Value << spriteRendererComponent.Texture->GetPath();

            out << YAML::EndMap; // SpriteRendererComponent
        }

        if (entity.HasComponent<CircleRendererComponent>())
        {
            out << YAML::Key << "CircleRendererComponent";
            out << YAML::BeginMap; // CircleRendererComponent

            auto& circleRendererComponent = entity.GetComponent<CircleRendererComponent>();
            out << YAML::Key << "Color" << YAML::Value << circleRendererComponent.Color;
            out << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.Thickness;
            out << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.Fade;

            out << YAML::EndMap; // CircleRendererComponent
        }

        if (entity.HasComponent<Rigidbody2DComponent>())
        {
            out << YAML::Key << "RigidBody2DComponent";
            out << YAML::BeginMap; // RigidBody2DComponent

            auto& rb2dComponent = entity.GetComponent<Rigidbody2DComponent>();
            out << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(rb2dComponent.Type);
            out << YAML::Key << "FixedRotation" << YAML::Value << rb2dComponent.FixedRotation;

            out << YAML::EndMap; // RigidBody2DComponent
        }

        if (entity.HasComponent<BoxCollider2DComponent>())
        {
            out << YAML::Key << "BoxCollider2DComponent";
            out << YAML::BeginMap; // BoxCollider2DComponent

            auto& bc2dComponent = entity.GetComponent<BoxCollider2DComponent>();
            out << YAML::Key << "Offset" << YAML::Value << bc2dComponent.Offset;
            out << YAML::Key << "Size" << YAML::Value << bc2dComponent.Size;
            out << YAML::Key << "Density" << YAML::Value << bc2dComponent.Density;
            out << YAML::Key << "Friction" << YAML::Value << bc2dComponent.Friction;
            out << YAML::Key << "Restitution" << YAML::Value << bc2dComponent.Restitution;
            out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2dComponent.RestitutionThreshold;

            out << YAML::EndMap; // BoxCollider2DComponent
        }

        if (entity.HasComponent<CircleCollider2DComponent>())
        {
            out << YAML::Key << "CircleCollider2DComponent";
            out << YAML::BeginMap; // CircleCollider2DComponent

            auto& cc2dComponent = entity.GetComponent<CircleCollider2DComponent>();
            out << YAML::Key << "Offset" << YAML::Value << cc2dComponent.Offset;
            out << YAML::Key << "Radius" << YAML::Value << cc2dComponent.Radius;
            out << YAML::Key << "Density" << YAML::Value << cc2dComponent.Density;
            out << YAML::Key << "Friction" << YAML::Value << cc2dComponent.Friction;
            out << YAML::Key << "Restitution" << YAML::Value << cc2dComponent.Restitution;
            out << YAML::Key << "RestitutionThreshold" << YAML::Value << cc2dComponent.RestitutionThreshold;

            out << YAML::EndMap; // CircleCollider2DComponent
        }

        out << YAML::EndMap; // Entity
    }

    void SceneSerializer::Serialize(const std::string& filepath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
        m_Scene->m_Registry.each([&](auto entityID)
        {
            Entity entity = { entityID, m_Scene.get() };
            if (!entity)
                return;

            SerializeEntity(out, entity);
        });
        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    void SceneSerializer::SerializeRuntime(const std::string& filepath)
    {
        // Not implemented
        HZ_CORE_ASSERT(false, "Not implemented!");
    }

    bool SceneSerializer::Deserialize(const std::string& filepath)
    {
        YAML::Node data;
        try
        {
            data = YAML::LoadFile(filepath);
        }
        catch (const YAML::ParserException& ex)
        {
            HZ_CORE_ERROR("Failed to deserialize .hazel file '{0}'\n  {1}", filepath, ex.what());

            return false;
        }

        if (!data["Scene"])
            return false;

        std::string sceneName = data["Scene"].as<std::string>();
        HZ_CORE_TRACE("Deserializing scene '{0}'", sceneName);

        YAML::Node entities = data["Entities"];
        if (entities)
        {
            for (auto entity : entities)
            {
                uint64_t uuid = entity["Entity"].as<uint64_t>();

                std::string name;
                auto tagComponent = entity["TagComponent"];
                if (tagComponent)
                    name = tagComponent["Tag"].as<std::string>();

                HZ_CORE_TRACE("Deserialized entity width ID = {0}, name = {1}", uuid, name);

                Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

                auto transformComponent = entity["TransformComponent"];
                if (transformComponent)
                {
                    // Entities always have transforms
                    auto& tc = deserializedEntity.GetComponent<TransformComponent>();
                    tc.Translation = transformComponent["Translation"].as<glm::vec3>();
                    tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
                    tc.Scale = transformComponent["Scale"].as<glm::vec3>();
                }

                auto cameraComponent = entity["CameraComponent"];
                if (cameraComponent)
                {
                    auto& cc = deserializedEntity.AddComponent<CameraComponent>();

                    auto& cameraProps = cameraComponent["Camera"];
                    cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

                    cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
                    cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
                    cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

                    cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
                    cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
                    cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

                    cc.Primary = cameraComponent["Primary"].as<bool>();
                    cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
                }

                auto scriptComponent = entity["ScriptComponent"];
                if (scriptComponent)
                {
                    auto& sc = deserializedEntity.AddComponent<ScriptComponent>();
                    sc.ClassName = scriptComponent["ClassName"].as<std::string>();
                    
                    // Fields
                    auto& scriptFields = scriptComponent["ScriptFields"];
                    if (scriptFields)
                    {
                        Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(sc.ClassName);
                        if (entityClass)
                        {
                            const auto& fields = entityClass->GetFields();
                            auto& entityFields = ScriptEngine::GetScriptFieldMap(deserializedEntity);
                            for (auto& scriptField : scriptFields)
                            {
                                std::string name = scriptField["Name"].as<std::string>();

                                std::string typeString = scriptField["Type"].as<std::string>();
                                ScriptFieldType type = Utils::StringToScriptFieldType(typeString);

                                ScriptFieldInstance& fieldInstance = entityFields[name];
                                HZ_CORE_ASSERT(fields.find(name) != fields.end());
                                if (fields.find(name) == fields.end())
                                    continue;
                                fieldInstance.Field = fields.at(name);

                                switch (type)
                                {
                                    case ScriptFieldType::Float:    fieldInstance.SetValue(scriptField["Data"].as<float>()); break;
                                    case ScriptFieldType::Double:   fieldInstance.SetValue(scriptField["Data"].as<double>()); break;
                                    case ScriptFieldType::Bool:     fieldInstance.SetValue(scriptField["Data"].as<bool>()); break;
                                    case ScriptFieldType::Char:     fieldInstance.SetValue(scriptField["Data"].as<int8_t>()); break;
                                    case ScriptFieldType::Short:    fieldInstance.SetValue(scriptField["Data"].as<int16_t>()); break;
                                    case ScriptFieldType::Int:      fieldInstance.SetValue(scriptField["Data"].as<int32_t>()); break;
                                    case ScriptFieldType::Long:     fieldInstance.SetValue(scriptField["Data"].as<int64_t>()); break;
                                    case ScriptFieldType::Byte:     fieldInstance.SetValue(scriptField["Data"].as<uint8_t>()); break;
                                    case ScriptFieldType::UShort:   fieldInstance.SetValue(scriptField["Data"].as<uint16_t>()); break;
                                    case ScriptFieldType::UInt:     fieldInstance.SetValue(scriptField["Data"].as<uint32_t>()); break;
                                    case ScriptFieldType::ULong:    fieldInstance.SetValue(scriptField["Data"].as<uint64_t>()); break;
                                    case ScriptFieldType::Vector2:  fieldInstance.SetValue(scriptField["Data"].as<glm::vec2>()); break;
                                    case ScriptFieldType::Vector3:  fieldInstance.SetValue(scriptField["Data"].as<glm::vec3>()); break;
                                    case ScriptFieldType::Vector4:  fieldInstance.SetValue(scriptField["Data"].as<glm::vec4>()); break;
                                    case ScriptFieldType::Entity:   fieldInstance.SetValue(scriptField["Data"].as<UUID>()); break;
                                }
                            }
                        }
                    }
                }

                auto spriteRendererComponent = entity["SpriteRendererComponent"];
                if (spriteRendererComponent)
                {
                    auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
                    src.Color = spriteRendererComponent["Color"].as<glm::vec4>();

                    if (spriteRendererComponent["TexturePath"])
                    {
                        //src.Texture = Texture2D::Create(spriteRendererComponent["TexturePath"].as<std::string>());

                        std::string texturePath = spriteRendererComponent["TexturePath"].as<std::string>();
                        auto path = Project::GetAssetFileSystemPath(texturePath);
                        src.Texture = Texture2D::Create(path.string());

                    }
                    
                    if (spriteRendererComponent["TilingFactor"])
                        src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
                }

                auto circleRendererComponent = entity["CircleRendererComponent"];
                if (circleRendererComponent)
                {
                    auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>();
                    crc.Color = circleRendererComponent["Color"].as<glm::vec4>();
                    crc.Thickness = circleRendererComponent["Thickness"].as<float>();
                    crc.Fade = circleRendererComponent["Fade"].as<float>();
                }

                auto rb2dComponent = entity["RigidBody2DComponent"];
                if (rb2dComponent)
                {
                    auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>();
                    rb2d.Type = RigidBody2DBodyTypeFromString(rb2dComponent["BodyType"].as<std::string>());
                    rb2d.FixedRotation = rb2dComponent["FixedRotation"].as<bool>();
                }

                auto bc2dComponent = entity["BoxCollider2DComponent"];
                if (bc2dComponent)
                {
                    auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
                    bc2d.Offset = bc2dComponent["Offset"].as<glm::vec2>();
                    bc2d.Size = bc2dComponent["Size"].as<glm::vec2>();
                    bc2d.Density = bc2dComponent["Density"].as<float>();
                    bc2d.Friction = bc2dComponent["Friction"].as<float>();
                    bc2d.Restitution = bc2dComponent["Restitution"].as<float>();
                    bc2d.RestitutionThreshold = bc2dComponent["RestitutionThreshold"].as<float>();

                }

                auto cc2dComponent = entity["CircleCollider2DComponent"];
                if (cc2dComponent)
                {
                    auto& cc2d = deserializedEntity.AddComponent<CircleCollider2DComponent>();
                    cc2d.Offset = cc2dComponent["Offset"].as<glm::vec2>();
                    cc2d.Radius = cc2dComponent["Radius"].as<float>();
                    cc2d.Density = cc2dComponent["Density"].as<float>();
                    cc2d.Friction = cc2dComponent["Friction"].as<float>();
                    cc2d.Restitution = cc2dComponent["Restitution"].as<float>();
                    cc2d.RestitutionThreshold = cc2dComponent["RestitutionThreshold"].as<float>();

                }
            }
        }

        return true;
    }

    bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
    {
        // Not implemented
        HZ_CORE_ASSERT(false, "Not implemented!");
        return false;
    }

}
