#include "hzpch.h"
#include "ProjectSerializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace Hazel {

	ProjectSerializer::ProjectSerializer(Ref<Project> project)
		: m_Project(project)
	{

	}

	bool ProjectSerializer::Serialize(const std::filesystem::path& filepath)
	{
		const ProjectConfig& config = m_Project->GetConfig();

		YAML::Emitter out;
		out << YAML::BeginMap; // Root
		
		out << YAML::Key << "Project" << YAML::Value << YAML::BeginMap; // ProjectMap
		out << YAML::Key << "Name" << YAML::Value << config.Name;
		out << YAML::Key << "StartScene" << YAML::Value << config.StartScene.string();
		out << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetDirectory.string();
		out << YAML::Key << "ScriptModulePath" << YAML::Value << config.ScriptModulePath.string();
		out << YAML::EndMap; // ProjectMap
		
		out << YAML::EndMap; // Root

		std::ofstream fout(filepath);
		fout << out.c_str();

		return true;
	}

	bool ProjectSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		ProjectConfig& config = m_Project->GetConfig();

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath.string());
		}
		catch (const YAML::ParserException& ex)
		{
			HZ_CORE_ERROR("Failed to deserialize project file '{0}'\n  {1}", filepath, ex.what());

			return false;
		}

		YAML::Node projectNode = data["Project"];
		if (!projectNode)
			return false;

		config.Name = projectNode["Name"].as<std::string>();
		config.StartScene = projectNode["StartScene"].as<std::string>();
		config.AssetDirectory = projectNode["AssetDirectory"].as<std::string>();
		config.ScriptModulePath = projectNode["ScriptModulePath"].as<std::string>();

		return true;
	}
}
