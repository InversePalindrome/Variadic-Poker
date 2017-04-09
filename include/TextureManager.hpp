/*
Copyright (c) 2017 InversePalindrome
VariadicPoker - TextureManager.hpp
InversePalindrome.com
*/


#pragma once
#include <unordered_map>
#include <string>
#include <SFML/Graphics/Texture.hpp>


class TextureManager
{
public:
	sf::Texture& getTexture(const std::string& name);

	void loadTexture(const std::string& name, const std::string& fileName);

private:
	std::unordered_map<std::string, sf::Texture> textures;
};