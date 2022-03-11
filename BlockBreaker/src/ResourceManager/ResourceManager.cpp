#include "ResourceManager.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include "GLEW/glew.h"
#include "stb/stb_image.h"

// Instantiate static variables
std::unordered_map<std::string, Texture*> ResourceManager::Textures;
std::unordered_map<std::string, Shader*> ResourceManager::Shaders;


Shader ResourceManager::LoadShader(const char* filepath, const std::string& name)
{
	Shaders[name] = LoadShaderFromFile(filepath);
    return *Shaders[name];
}

void ResourceManager::LoadTexture(const char* filepath, bool alpha, const std::string& name)
{
    Textures[name] = LoadTextureFromFile(filepath, alpha);
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders) {
        iter.second->Clean();
        delete (iter.second);
    }
    // (properly) delete all textures
    for (auto iter : Textures) {
        iter.second->Clean();
        delete (iter.second);
    }

    Shaders.clear();
    Textures.clear();
}

Shader* ResourceManager::LoadShaderFromFile(const char* filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType {
        None = -1,
        Vertex = 0,
        Fragment = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::None;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::Vertex;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::Fragment;
        }
        else {
            ss[int(type)] << line << '\n';
        }
    }

    Shader* sh = new Shader(ss[0].str(), ss[1].str());
    return sh;
}

Texture* ResourceManager::LoadTextureFromFile(const char* filepath, bool alpha)
{
    int width, height, nrChannels;
    unsigned char* localBuffer = stbi_load(filepath, &width, &height, &nrChannels, 4);

    Texture* texture = new Texture(width ,height);
    if (alpha)
    {
        texture->SetInternalFormat(GL_RGBA);
        texture->SetImageFormat(GL_RGBA);
    }
    texture->Generate(localBuffer);

    if (localBuffer)
        stbi_image_free(localBuffer);

    return texture;
}
