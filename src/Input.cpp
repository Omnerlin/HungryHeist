#include <Input.h>
#include <cstring>

int Input::PressedKeyDictionary[sf::Keyboard::KeyCount] {0};

bool Input::KeyIsDown(KeyCode key) {
    return sf::Keyboard::isKeyPressed(key);
}

bool Input::KeyWasPressed(KeyCode key) {
    return Input::GetKeyPressCount(key) > 0;
}

int Input::GetKeyPressCount(KeyCode key) {
    return PressedKeyDictionary[key];
}

void Input::AddKeyPress(KeyCode key) {
    PressedKeyDictionary[key] += 1;
}

void Input::ClearPressedKeys() {
    memset(PressedKeyDictionary, 0, sf::Keyboard::KeyCount * sizeof(int));
}