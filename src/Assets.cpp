#include "Assets.h"
#include <iostream>

BasicAssetStore<sf::Texture> Assets::_textures;
BasicAssetStore<sf::Font> Assets::_fonts;
BasicAssetStore<sf::SoundBuffer> Assets::_soundBuffers;

sf::Texture& Assets::LoadTexture(const std::string& texturePath)
{
	return _textures.LoadResource(texturePath);
}

sf::Font& Assets::LoadFont(const std::string& fontPath)
{
	return _fonts.LoadResource(fontPath);
}


sf::SoundBuffer& Assets::LoadSoundBuffer(const std::string& bufferPath)
{
	return _soundBuffers.LoadResource(bufferPath);
}
