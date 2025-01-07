#ifndef MENU_POPUP_H
#define MENU_POPUP_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "UIElements.h"

class MenuPopup {
public:
	sf::RectangleShape background;
	sf::Texture bgTexture;
	sf::RenderWindow* main_window;
	sf::Font default_font;

	bool isVisible = false;

	std::vector<Labeled_Button> Menu_Buttons;

	MenuPopup(sf::RenderWindow* window, sf::Font default_font);
	void MenuBgInit();
	void MenuDraw();
};

#endif