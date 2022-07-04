#include "Assets.h"
#include <iostream>

std::unordered_map<std::string, sf::Texture> Assets::_textures;

sf::Texture& Assets::LoadTexture(const std::string& texturePath)
{
	if(_textures.find(texturePath) != _textures.end())
	{
		return _textures[texturePath];
	}
	else
	{
		sf::Texture texture;
		if(!texture.loadFromFile(texturePath))
		{
			return _emptyTexture;
		}
		_textures.emplace(texturePath, texture);
		return _textures[texturePath];
	}
}

sf::Font& Assets::LoadFont(const std::string& fontPath)
{
	if (_fonts.find(fontPath) != _fonts.end())
	{
		return _fonts[fontPath];
	}
	else
	{
		sf::Font font;
		if (!font.loadFromFile(fontPath))
		{
			return _emptyFont;
		}
		_fonts.emplace(fontPath, font);
		return _fonts[fontPath];
	}
}
