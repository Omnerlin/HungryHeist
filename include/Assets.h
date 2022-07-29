#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "unordered_map"
#include "Game.h"
#include <iostream>

template <typename T>
struct BasicAssetStore
{
	std::unordered_map<std::string, T> store;

	T& LoadResource(std::string path)
	{
		if (store.contains(path))
		{
			// std::cout << "Loading pre-existing resource from: " << path << std::endl;
			return store[path];
		}

		T newAsset;
		if (!newAsset.loadFromFile(Game::GetAbsolutePath(path)))
		{
			return _empty;
		}
		store.emplace(path, newAsset);
		return store[path];
	}

private:
	T _empty;
};

struct Assets
{
public:
	static sf::Texture& LoadTexture(const std::string& texturePath);
	static sf::Font& LoadFont(const std::string& fontPath);
	static sf::SoundBuffer& LoadSoundBuffer(const std::string& bufferPath);

private:
	static BasicAssetStore<sf::Texture> _textures;
	static BasicAssetStore<sf::Font> _fonts;
	static BasicAssetStore<sf::SoundBuffer> _soundBuffers;
};
