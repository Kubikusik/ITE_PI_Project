#include <iostream>
#include <SFML/Graphics.hpp>
#include "UIElements.h"
#include "Menu_Popup.h"

MenuPopup::MenuPopup(sf::RenderWindow *window, sf::Font default_font) {
	this->main_window = window;
    this->default_font = default_font;

	MenuBgInit();
}

void MenuPopup::MenuBgInit() {
	background.setSize(sf::Vector2f(
		(this->main_window)->getSize().x / 1.3f,
		(this->main_window)->getSize().y / 1.3f
	));
	background.setPosition(sf::Vector2f(
		(this->main_window->getSize().x / 2.0f - background.getSize().x / 2.0f),
		(this->main_window->getSize().y / 2.0f - background.getSize().y / 2.0f)
	));

	//int x = this->main_window->getSize().x; //reading size of menu
	//int y = this->main_window->getSize().y;
	background.setFillColor(sf::Color(200, 200, 200));
	bgTexture.loadFromFile("Background.png");
	background.setTexture(&bgTexture);

}

void MenuPopup::MenuDraw() {
	(this->main_window)->draw(background);

    sf::Color bg_color = GetBgColor(); // Get current background color

    // Red color adjustment buttons
    Menu_Popup_Plus_Button increase_R(300, 200, 50, "plus_r", "+", default_font, sf::Color(200, 0, 0));
    Menu_Popup_Minus_Button decrease_R(350, 200, 50, "minus_r", "-", default_font, sf::Color(200, 0, 0));
    

    // Green color adjustment buttons
    Menu_Popup_Plus_Button increase_G(300, 300, 50, "plus_g", "+", default_font, sf::Color(0, 200, 0));
    Menu_Popup_Minus_Button decrease_G(350, 300, 50, "minus_g", "-", default_font, sf::Color(0, 200, 0));

    // Blue color adjustment buttons
    Menu_Popup_Plus_Button increase_B(300, 400, 50, "plus_b", "+", default_font, sf::Color(0, 0, 200));
    Menu_Popup_Minus_Button decrease_B(350, 400, 50, "minus_b", "-", default_font, sf::Color(0, 0, 200));

    Labeled_Button color_sample(600, 300, 50, "color_sample", "", default_font, GetBgColor());

    sf::Text color_display;
    color_display.setFont(default_font);
    color_display.setCharacterSize(20);
    color_display.setPosition(550, 250);
    color_display.setFillColor(sf::Color::Black);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        increase_R.Clicked(*(this->main_window), bg_color, color_display, "r");
        decrease_R.Clicked(*(this->main_window), bg_color, color_display, "r");

        increase_G.Clicked(*(this->main_window), bg_color, color_display, "g");
        decrease_G.Clicked(*(this->main_window), bg_color, color_display, "g");

        increase_B.Clicked(*(this->main_window), bg_color, color_display, "b");
        decrease_B.Clicked(*(this->main_window), bg_color, color_display, "b");

        ChangeBackgroundColor(bg_color); // Update the global background color
    }
    else {
        increase_R.Release(); // Reset clicked state
        decrease_R.Release();

        increase_G.Release(); // Reset clicked state
        decrease_G.Release();

        increase_B.Release(); // Reset clicked state
        decrease_B.Release();
    }

    color_display.setString("R: " + std::to_string(bg_color.r) +
        " G: " + std::to_string(bg_color.g) +
        " B: " + std::to_string(bg_color.b));


    increase_R.DrawItself(*(this->main_window));
    decrease_R.DrawItself(*(this->main_window));
    increase_G.DrawItself(*(this->main_window));
    decrease_G.DrawItself(*(this->main_window));
    increase_B.DrawItself(*(this->main_window));
    decrease_B.DrawItself(*(this->main_window));
    color_sample.DrawItself(*(this->main_window));

    this->main_window->draw(color_display);
}