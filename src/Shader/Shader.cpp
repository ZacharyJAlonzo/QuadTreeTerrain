
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

Shader::Shader(const string& filepath)
{
    source = ParseShader(filepath);
    CompileShader();
}

void Shader::Use()
{
    glUseProgram(ID);
}

void Shader::SetMat4(const string& uniformName, bool transpose, glm::mat4 value)
{
    unsigned int loc = glGetUniformLocation(ID, uniformName.c_str());
    glUniformMatrix4fv(loc, 1, transpose, glm::value_ptr(value));
}

ShaderProgramSource Shader::ParseShader(const string& filepath)
{
    enum ShaderType
    {
        vertex,
        fragment,
        NONE
    };

    std::fstream stream;
    stream.open(filepath);
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    ss[0].str("");
    ss[1].str("");
    if (stream.is_open())
    {
        string line;
        while (getline(stream, line))
        {
            //std::cout << line << "\n";
            if (line.find("shader") != string::npos)
            {
                if (line.find("vertex") != string::npos)
                {
                    type = ShaderType::vertex;
                }
                else if (line.find("fragment") != string::npos)
                {
                    type = ShaderType::fragment;
                }
            }
            else
            {
                //string stream array contains both shaders
                ss[(int)type] << line << "\n";
            }
        }
    }
    stream.close();

    ShaderProgramSource source{ ss[0].str(), ss[1].str() };
    return source;
}



unsigned int Shader::CreateShader(unsigned int type, const string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];

        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile shader" << std::endl;
        std::cout << message << std::endl;

        delete[] message;
        return 0;
    }

    return id;
}

void Shader::CompileShader()
{
    ID = glCreateProgram();
    VS = CreateShader(GL_VERTEX_SHADER, source.VertexSource);
    FS = CreateShader(GL_FRAGMENT_SHADER, source.FragmentSource);

    glAttachShader(ID, VS);
    glAttachShader(ID, FS);
    glLinkProgram(ID);
    glValidateProgram(ID);
}
