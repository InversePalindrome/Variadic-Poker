/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - TextureManager.cpp
InversePalindrome.com
*/


#include "TextureManager.hpp" 


sf::Texture& TextureManager::getTexture(const std::string& name)
{
    return this->textures.at(name);
}

void TextureManager::loadTexture(const std::string& name, const std::string& fileName)
{
    sf::Texture texture;

    texture.loadFromFile(fileName);

    this->textures[name] = texture;
}

bool TextureManager::hasTexture(const std::string& name) const
{
    return this->textures.count(name) > 0;
}