#pragma once
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

class Shader
{
private:
	unsigned int m_RendererID;
	std::unordered_map <std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& vs, const std::string& fs);

	void Bind() const;
	void Unbind() const;

	//Set Uniforms
	void SetUniform1f(const std::string& name, float value);
	void SetUniform1i(const std::string& name, float value);
	void SetUniform2f(const std::string& name, glm::vec2 value);
	void SetUniform3f(const std::string& name, glm::vec3 value);
	void SetUniform4f(const std::string& name, glm::vec4 value);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	void Clean();

private:
	unsigned int GetUniformLocation(const std::string& name);

	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(const std::string& source, unsigned int type);
 };

