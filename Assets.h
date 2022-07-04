#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "unordered_map"

enum class ResourceType
{
	Texture,
	Shader
};

struct Assets
{
public:
	static sf::Texture& LoadTexture(const std::string& texturePath);
	static sf::Font& LoadFont(const std::string& fontPath);

private:
	static std::unordered_map<std::string, sf::Texture> _textures;
	static std::unordered_map<std::string, sf::Font> _fonts;
	static sf::Texture _emptyTexture;
	static sf::Font _emptyFont;
};
