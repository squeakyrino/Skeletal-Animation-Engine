#pragma once
#include <Vectors\Vec2.h>
#include <Vectors\Vec3.h>
#include <Vectors\Vec4.h>
#include <Matrix\Mat2.h>
#include <Matrix\Mat3.h>
#include <Matrix\Mat4.h>

#include <string>

#include <GL\glew.h>

namespace Engine
{
    class UniformValue
    {
    protected:
        int location;
    public:
        UniformValue(int location) : location(location) {}
        virtual void setUniform() = 0;
    };

    class BoolUniformValue : public UniformValue
    {
    protected:
        bool boolean;
    public:
        BoolUniformValue(int location, bool boolean)
            : UniformValue(location), boolean(boolean) {}

        void setUniform() override 
        {
            
        }
    };

    class IntUniformValue : public UniformValue
    {
    protected:
        int integer;
    public:
        IntUniformValue(int location, int integer)
            : UniformValue(location), integer(integer) {}

        void setUniform() override
        {
            glUniform1i(location, integer);
        }
    };

    class floatUniformValue : public UniformValue
    {
    protected:
        float value;
    public:
        floatUniformValue(int location, float value)
            : UniformValue(location), value(value) {}

        void setUniform() override
        {
            glUniform1f(location, value);
        }
    };

    class vec3UniformValue : public UniformValue
    {
    protected:
        Vec3 value;
    public:
        vec3UniformValue(int location, Vec3 value)
            : UniformValue(location), value(value) {}

        void setUniform() override
        {
            glUniform3f(location, value.x, value.y, value.z);
        }
    };

    class Mat4UniformValue : public UniformValue
    {
    protected:
        //Already in opengl format
        std::vector<float> mat;
    public:
        Mat4UniformValue(int location, Mat4 mat)
            : UniformValue(location) 
        {
            this->mat = mat.flatten();
        }

        void setUniform() override
        {
            glUniformMatrix4fv(location, 1, GL_TRUE, mat.data());
        }
    };

    class ArrayMat4UniformValue : public UniformValue
    {
    protected:
        //Already in opengl format
        std::vector<float> mat;
        size_t numMats;
    public:
        ArrayMat4UniformValue(int location, std::vector<Mat4> matrices)
            : UniformValue(location), numMats(matrices.size())
        {
            for (auto const& tempMat : matrices)
            {
                std::vector<float> temp = tempMat.flatten();
                this->mat.insert(this->mat.end(), temp.begin(), temp.end());
            }
            
        }

        void setUniform() override
        {
            glUniformMatrix4fv(location, (GLsizei)numMats, GL_TRUE, mat.data());
        }
    };


}