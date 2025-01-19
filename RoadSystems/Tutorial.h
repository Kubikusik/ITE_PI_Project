#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

class Tutorial_Button {
public:
    Tutorial_Button(float x, float y, const std::string& label, sf::Texture textur);
    void draw(sf::RenderWindow& window);
    bool isClicked(const sf::Vector2i& mousePos);

    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;
    sf::Texture texture;
};

class TutorialWindow {
public:
    TutorialWindow();
    void display(sf::RenderWindow& window);

    sf::Text text1;
    sf::Text text2;
    sf::Font font;
    
    bool tutorialActive = false;
};

#endif