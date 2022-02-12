#pragma once
#include <SFML/Window.hpp>
#include <map>

typedef sf::Keyboard::Key KeyCode;

struct Input  {
    static bool KeyIsDown(KeyCode);
    static bool KeyWasPressed(KeyCode);
    static int GetKeyPressCount(KeyCode key);
    static void ClearPressedKeys();
    static void AddKeyPress(KeyCode key);

    private: 
    static int PressedKeyDictionary[];
};