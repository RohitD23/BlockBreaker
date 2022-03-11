#pragma once
#include <unordered_map>
#include "../Utility/Shader.h"
#include "../Utility/Texture.h"

class ResourceManager
{
public:
	static std::unordered_map<std::string, Shader*> Shaders;
	static std::unordered_map<std::string, Texture*> Textures;

	static Shader LoadShader(const char* filepath, const std::string& name);
	static inline Shader GetShader(const std::string& name) { return *Shaders[name]; }

	static void LoadTexture(const char* filepath, bool alpha, const std::string& name);
	static inline Texture GetTexture(const std::string& name) { return *Textures[name]; }

	static void Clear();

private:
	// private constructor, to make resource manager singleton class. Its members and functions should be publicly available (static).
	ResourceManager() {};

	static Shader* LoadShaderFromFile(const char* filepath);
	static Texture* LoadTextureFromFile(const char* filepath, bool alpha);
};

