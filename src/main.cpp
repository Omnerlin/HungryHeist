#define JSON_USE_IMPLICIT_CONVERSIONS 0

#include <SFML/Graphics.hpp>
#include <Game.h>

#include "Input.h"
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")


/*
Hand Layout            --|---|---|---|--
					   |               |
--- = Wall             |               |
| = Hand               |               |
					   --|---|---|---|--
*/

int main(int arc, char** argv) {
	// Create Window
	sf::RenderWindow window;  // Window Size
	Game game(&window);
	game.Initialize();

	sf::Image icon;
	icon.loadFromFile("assets/textures/SphynxIcon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	
	while (window.isOpen()) {
		Input::ClearPressedKeys();
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::Resized) {
				float targetAspect = 16.f / 9;
				float currentImageAspect = (float)event.size.width / event.size.height;
				game.guiCamera.setSize(sf::Vector2f(window.getSize()));
				game.guiCamera.setCenter(game.guiCamera.getSize() / 2.f);

				sf::FloatRect newViewportSettings(0, 0, 1, 1);
				if (currentImageAspect < targetAspect) { // Our window is 
					newViewportSettings = sf::FloatRect(0, (1.f - currentImageAspect / targetAspect) / 2, 1, currentImageAspect / targetAspect);
					game.gui.root->SetLocalPosition(0, game.guiCamera.getSize().y * ((1.f - currentImageAspect / targetAspect) / 2));
				}
				else if (currentImageAspect > targetAspect) { // Our window is wider than our game view
					newViewportSettings = sf::FloatRect((1.f - targetAspect / currentImageAspect) / 2, 0, targetAspect / currentImageAspect, 1);
					game.gui.root->SetLocalPosition(game.guiCamera.getSize().x * ((1.f - targetAspect / currentImageAspect) / 2), 0);
				}
				game.mainCamera.setViewport(newViewportSettings);
				game.gui.root->SetRectSize(game.guiCamera.getSize().x * newViewportSettings.width, game.guiCamera.getSize().y * newViewportSettings.height);
			}
			if (event.type == sf::Event::KeyPressed) {
				Input::AddKeyPress(event.key.code);
			}
			game.gui.ProcessEvent(event);
		}

		game.Tick();
		game.Render();
	}

	return 0;
}