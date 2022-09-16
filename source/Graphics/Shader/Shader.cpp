#include <Graphics/Shader/Shader.h>
#include <EngineHelperFunctions.h>

#include <GL/glew.h>

#include <fstream>
#include <sstream>
#include <iostream>

namespace Engine
{
	Shader::Shader(std::string name, const std::string& vertexFilename, const std::string& fragFilename)
		: name(name)
	{
		unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);

		id = glCreateProgram();

		std::string vsSrc = loadSrcFromFile(vertexFilename);
		std::string fsSrc = loadSrcFromFile(fragFilename);

		createShader(vsSrc, vs);
		createShader(fsSrc, fs);

		//Link shaders
		glAttachShader(id, vs);
		glAttachShader(id, fs);
		

		//TESTING:
		// Link the program.
		glLinkProgram(id);

		GLint isLinked = 0;
		glGetProgramiv(id, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(id, maxLength, &maxLength, &infoLog[0]);

			std::cout << infoLog.data() << std::endl;

			// The program is useless now. So delete it.
			glDeleteProgram(id);

			// Provide the infolog in whatever manner you deem best.
			// Exit with failure.
			return;
		}

		//Delete cause we don't need
		glDeleteShader(vs);
		glDeleteShader(fs);

		//Go through attrib/uniforms here

		 // query the number of active uniforms
		int nrUniforms;
		glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &nrUniforms);

		char buffer[128];

		// iterate over all active uniforms
		for (GLuint  i = 0; i < (GLuint)nrUniforms; ++i)
		{
			//Note: We don't care about the size of the uniform right now. Maybe it's needed sometime?
			//And we don't care about the type either
			GLenum glType;
			GLint size;
			glGetActiveUniform(id, i, sizeof(buffer), 0, &size, &glType, buffer);
			
			std::string tname = std::string(buffer);
			GLint tlocation = glGetUniformLocation(id, buffer);

			/* From the spec:
			* - This function returns -1 if name does not correspond to an active uniform variable in program,
			* - if name starts with the reserved prefix "gl_", 
			* - or if name is associated with an atomic counter or a named uniform block.
			*
			* So basically we can cull out the global uniforms by just checking if the location is -1
			*/
			if(tlocation != (GLint) -1)
				uniforms[tname] = tlocation;
			
		}

		//For the global uniforms we need to bind the uniform block to the global binding point
		unsigned int globalUniformTest = glGetUniformBlockIndex(id, "GlobalMatrices");
		glUniformBlockBinding(id, globalUniformTest, 0);

	}

	void Shader::use()
	{
		glUseProgram(id);
	}

	void Shader::createShader(const std::string& source, unsigned int shaderObject)
	{
		const GLchar* shaderSourceStrings[1];
		GLint shaderSourceStringsLength[1];

		shaderSourceStrings[0] = source.c_str();
		shaderSourceStringsLength[0] = (GLint)source.length();

		glShaderSource(shaderObject, 1, shaderSourceStrings, shaderSourceStringsLength);
		glCompileShader(shaderObject);

		//Check for errors
		GLint success = 0;
		GLchar error[1024] = { 0 };

		glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			glGetShaderInfoLog(shaderObject, sizeof(error), NULL, error);

			std::stringstream ss;
			ss << "Error in shader: " << source << std::endl;

			ss << error << std::endl;
			throw std::runtime_error(ss.str());
		}

	}

	std::string Shader::loadSrcFromFile(const std::string& filename)
	{
		std::ifstream file;
		file.open((filename).c_str());

		std::string output;
		std::string line;

		if (file.is_open())
		{
			while (file.good())
			{
				getline(file, line);

				//Recursively read any includes
				if (line.substr(0, 8) == "#include")
				{					

					//Since I'm using a specific plugin for GLSL highlight in VS Studio
					
					std::string includePath = getDirectoryFromPath(filename) + "/" + line.substr(9);
					
					std::size_t delimiterPos = includePath.find("//");

					includePath = includePath.substr(0, delimiterPos);

					output += loadSrcFromFile(includePath);
				}
				else
				{
					output.append(line + "\n");
				}
			}
		}
		else
		{
			//TODO: Replace with logging LOOOOOOOOOOL
			throw std::runtime_error("Error: Unable to load shader at path: " + filename);
		}

		return output;
	}
}