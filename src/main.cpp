#define JSON_USE_IMPLICIT_CONVERSIONS 0

#include <filesystem>
#include <SFML/Graphics.hpp>
#include <Game.h>
#include "Input.h"
#if defined(_WIN64) || defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

/*
Hand Layout            --|---|---|---|--
					   |               |
--- = Wall             |               |
| = Hand               |               |
					   --|---|---|---|--
*/

int main(int arc, char** argv) {

	auto dir = std::filesystem::path(argv[0]).parent_path();
	Game::rootPath = dir;

	// Create Window
	sf::RenderWindow window;  // Window Size
	Game game(&window);
	game.Initialize();
	

	sf::Image icon;
	icon.loadFromFile(Game::GetAbsolutePath("assets/textures/SphynxIcon.png"));
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	
	while (window.isOpen()) {
		Input::ClearPressedKeys();
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close(); 
			else if (event.type == sf::Event::Resized) {
				sf::Vector2u newSize(event.size.width, event.size.height);
				game.HandleResize(newSize);
			}
			else if (event.type == sf::Event::KeyPressed) {
				Input::AddKeyPress(event.key.code);
			}
			else if(event.type == sf::Event::LostFocus)
			{
				game.TrySetPaused(true);
			}
			game.gui.ProcessEvent(event);
		}

		game.Tick();
		game.gui.root->UpdateTransforms();
		game.gui.RootUpdateHoveredElement();
		game.Render();
	}

	return 0;
}