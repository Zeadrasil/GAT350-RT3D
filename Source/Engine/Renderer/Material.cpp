#include "Material.h"
#include "Program.h"
#include "Texture.h"
#include "Core/Core.h"
#include "Cubemap.h"
namespace nc
{
	bool Material::Create(std::string filename, ...)
	{
		// load program json document
		rapidjson::Document document;
		bool success = Json::Load(filename, document);
		if (!success)
		{
			INFO_LOG("Could not load program file (%s)." << filename);
			return false;
		}

		// read the program name
		std::string program;
		READ_DATA(document, program);
		// get program resource
		m_program = GET_RESOURCE(Program, program);

		// read the textures name
		// read the textures name
		std::vector<std::string> textures;
		READ_DATA(document, textures);
		for (auto texture : textures)
		{
			// get texture resource
			m_textures.push_back(GET_RESOURCE(Texture, texture));
		}


		std::string textureName;
		READ_NAME_DATA(document, "albedoTexture", textureName);
		if (!textureName.empty())
		{
			params = ALBEDO_TEXTURE_MASK;
			albedoTexture = GET_RESOURCE(Texture, textureName);
		}
		textureName = "";
		READ_NAME_DATA(document, "specularTexture", textureName);
		if (!textureName.empty())
		{
			params |= SPECULAR_TEXTURE_MASK;
			specularTexture = GET_RESOURCE(Texture, textureName);
		}
		textureName = "";
		READ_NAME_DATA(document, "emissiveTexture", textureName);
		if (!textureName.empty())
		{
			params |= EMISSIVE_TEXTURE_MASK;
			emissiveTexture = GET_RESOURCE(Texture, textureName);
		}
		textureName = "";
		READ_NAME_DATA(document, "normalTexture", textureName);
		if (!textureName.empty())
		{
			params |= NORMAL_TEXTURE_MASK;
			normalTexture = GET_RESOURCE(Texture, textureName);
		}
		textureName = "";
		READ_NAME_DATA(document, "cubemap", textureName);
		if (!textureName.empty())
		{
			params |= NORMAL_TEXTURE_MASK;
			std::vector<std::string> cubemaps;
			READ_DATA(document, cubemaps);

			cubemapTexture = GET_RESOURCE(Cubemap, textureName, cubemaps);
		}

		READ_DATA(document, albedo);
		READ_DATA(document, specular);
		READ_DATA(document, emissive);
		READ_DATA(document, shininess);
		READ_DATA(document, tiling);
		READ_DATA(document, offset);

		return true;
	}

	void Material::Bind()
	{
		m_program->Use();
		if (!cubemapTexture)
		{
			m_program->SetUniform("material.albedo", albedo);
			m_program->SetUniform("material.params", params);
			m_program->SetUniform("material.specular", specular);
			m_program->SetUniform("material.emissive", emissive);
			m_program->SetUniform("material.shininess", shininess);
			m_program->SetUniform("material.tiling", tiling);
			m_program->SetUniform("material.offset", offset);
		}
		if (albedoTexture)
		{
			albedoTexture->SetActive(GL_TEXTURE0);
			albedoTexture->Bind();
		}
		if (specularTexture)
		{
			specularTexture->SetActive(GL_TEXTURE1);
			specularTexture->Bind();
		}
		if (normalTexture)
		{
			normalTexture->SetActive(GL_TEXTURE2);
			normalTexture->Bind();
		}
		if (emissiveTexture)
		{
			emissiveTexture->SetActive(GL_TEXTURE3);
			emissiveTexture->Bind();
		}
		if (cubemapTexture)
		{
			cubemapTexture->SetActive(GL_TEXTURE0);
			cubemapTexture->Bind();
		}

	}
	void Material::ProcessGUI()
	{
		ImGui::Begin("Material");
		ImGui::ColorEdit3("Albedo", glm::value_ptr(albedo));
		ImGui::ColorEdit3("Specular", glm::value_ptr(specular));
		ImGui::DragFloat("Shininess", &shininess, 0.1f, 2.0f, 200.0f);
		ImGui::ColorEdit3("Emissive", glm::value_ptr(emissive));
		ImGui::DragFloat2("Tiling", glm::value_ptr(tiling), 0.1f);
		ImGui::DragFloat2("Offset", glm::value_ptr(offset), 0.1f);
		ImGui::End();
	}
}