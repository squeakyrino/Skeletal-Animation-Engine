#pragma once

#include <Graphics\Shader\Shader.h>
#include <Graphics\Shader\UniformValue.h>
#include <Graphics\Material\Texture.h>
#include <Graphics\Shader\UniformSamplerValue.h>

#include <memory>

#include <GL\glew.h>


namespace Engine
{
	/*
	* Material: responsible for representing all the required render state for rendering a mesh and/or a shader
	* 
	*/
	class Material
	{
    private:
        std::shared_ptr<Shader> shader;
        
        std::map<std::string, std::unique_ptr<UniformValue>> uniformValues;
        std::map<std::string, std::unique_ptr<UniformSamplerValue>> uniformSamplerValues;

    public:
        // depth state
        bool   DepthTest = true;
        bool   DepthWrite = true;
        GLenum DepthCompare = GL_LESS;

        // face culling state
        bool   Cull = true;
        GLenum CullFace = GL_BACK;
        GLenum CullWindingOrder = GL_CCW;

        // blending state
        bool   Blend = false;
        GLenum BlendSrc = GL_ONE; // pre-multiplied alpha
        GLenum BlendDst = GL_ONE_MINUS_SRC_ALPHA;
        GLenum BlendEquation = GL_FUNC_ADD;

    public:
        Material(std::shared_ptr<Shader> shader);

        std::shared_ptr<Shader> getShader();

        void setUniformInt(std::string uniformName, int integer);

        void setUniformFloat(std::string uniformName, float f);

        void setUniformVec3(std::string uniformName, Vec3 v);

        void setUniformMatrix4(std::string uniformName, Mat4 matrix);

        void setUniformArrayMatrix4(std::string uniformName, std::vector<Mat4> matrixArr);

        void setTexture(std::string uniformName, Texture* tex, unsigned int unit);

        void sendUniforms();
	};
}