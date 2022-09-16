#include <Graphics/Material/Material.h>

namespace Engine
{
	Material::Material(std::shared_ptr<Shader> shader) : shader(shader)
	{
	}

	std::shared_ptr<Shader> Material::getShader()
	{
		return shader;
	}

	void Material::setUniformInt(std::string uniformName, int integer)
	{
		auto it = shader->uniforms.find(uniformName);

		//If key exists
		if (it != shader->uniforms.end())
		{
			//Create uniformValue and add it to the map
			uniformValues[uniformName] = std::make_unique<IntUniformValue>(it->second, integer);
		}
	}

	void Material::setUniformFloat(std::string uniformName, float f)
	{
		auto it = shader->uniforms.find(uniformName);

		//If key exists
		if (it != shader->uniforms.end())
		{
			//Create uniformValue and add it to the map
			uniformValues[uniformName] = std::make_unique<floatUniformValue>(it->second, f);
		}
	}

	void Material::setUniformVec3(std::string uniformName, Vec3 v)
	{
		auto it = shader->uniforms.find(uniformName);

		//If key exists
		if (it != shader->uniforms.end())
		{
			//Create uniformValue and add it to the map
			uniformValues[uniformName] = std::make_unique<vec3UniformValue>(it->second, v);
		}
	}

	void Material::setUniformMatrix4(std::string uniformName, Mat4 matrix)
	{
		auto it = shader->uniforms.find(uniformName);

		//If key exists
		if (it != shader->uniforms.end())
		{
			//Create uniformValue and add it to the map
			uniformValues[uniformName] = std::make_unique<Mat4UniformValue>(it->second, matrix);
		}
	}

	void Material::setUniformArrayMatrix4(std::string uniformName, std::vector<Mat4> matrixArr)
	{
		//TODO: Accessing shader directly. Should be read-only
		//We need to append [0] on arrays because that's how OpenGL gets those names
		std::string uniformNameArray = uniformName.append("[0]");

		auto it = shader->uniforms.find(uniformNameArray);

		//If key exists
		if (it != shader->uniforms.end())
		{
			//Create uniformValue and add it to the map
			uniformValues[uniformName] = std::make_unique<ArrayMat4UniformValue>(it->second, matrixArr);
		}

	}

	void Material::setTexture(std::string uniformName, Texture* tex, unsigned int unit)
	{
		auto it = shader->uniforms.find(uniformName);

		//If key exists
		if (it != shader->uniforms.end())
		{
			//Create uniformValue and add it to the map
			uniformSamplerValues[uniformName] = std::make_unique<UniformSamplerValue2D>(tex->getTexObjID(), unit);
		}

		//Also assign the texture unit to the sampler
		setUniformInt(uniformName, unit);
	}

	void Material::sendUniforms()
	{
		for (auto const& key : uniformValues)
		{
			key.second->setUniform();
		}

		for (auto const& key : uniformSamplerValues)
		{
			key.second->setUniformSampler();
		}
	}
}