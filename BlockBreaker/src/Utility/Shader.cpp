#include <iostream>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

Shader::Shader(const std::string& vs, const std::string& fs)
	:m_RendererID(0)
{
    m_RendererID = CreateShader(vs, fs);
}

void Shader::Clean()
{
    glDeleteProgram(m_RendererID);
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int  Shader::CompileShader(const std::string& source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    m_RendererID = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragement")
            << " shader\n";
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform1i(const std::string& name, float value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform2f(const std::string& name, glm::vec2 value)
{
    glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void Shader::SetUniform3f(const std::string& name, glm::vec3 value)
{
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void Shader::SetUniform4f(const std::string& name, glm::vec4 value)
{
    glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache.at(name);

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    m_UniformLocationCache[name] = location;

    return location; 
}
