#pragma once
#include <string>
#include <map>

#include <Vectors\Vec3.h>
#include <Matrix\Mat3.h>
#include <Matrix\Mat4.h>

namespace Engine
{
	class Shader
	{
		//TODO: Not everything should be public but wtv
	public:
		unsigned int id;
		std::string name;

		std::map<std::string, int> uniforms;


		Shader(std::string name, const std::string& vertexFilename, const std::string& fragFilename);
	
		void use();

		
	private:
		void createShader(const std::string& source, unsigned int shaderObject);

		std::string loadSrcFromFile(const std::string& filename);
	};



}