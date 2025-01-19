#include "Tutorial.h"

Tutorial_Button::Tutorial_Button(float x, float y, const std::string& label, sf::Texture texture) {
    shape.setSize(sf::Vector2f(50, 50));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::White);
    this->texture = texture;
    shape.setTexture(&this->texture);
}

void Tutorial_Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Tutorial_Button::isClicked(const sf::Vector2i& mousePos) {
    return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

TutorialWindow::TutorialWindow() {
    if (!font.loadFromFile("OpenSansRegular.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    text1.setFont(font);
    text1.setString("TUTORIAL");
    text1.setCharacterSize(48);
    text1.setFillColor(sf::Color::White);
    text1.setScale(sf::Vector2f(1, 1));
    text1.setPosition(300, 150);

    text2.setFont(font);
    text2.setString("This is an introduction to GRIDBOUND.\n"
        "Press the buttons on the left to draw on the grid.\n"
        "When you're done, click RUN.\n"
        "REMEMBER: Press 'M' for the menu to change colors or adjust sounds.\n\n"
        "Press 'ESC' to close this window.");
    text2.setCharacterSize(24);
    text2.setFillColor(sf::Color::White);
    text2.setScale(sf::Vector2f(1, 1));
    text2.setPosition(100, 250);
}

void TutorialWindow::display(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    window.draw(text1);
    window.draw(text2);
    //window.display();
}