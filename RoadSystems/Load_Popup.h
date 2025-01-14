#ifndef LOAD_POPUP_H
#define LOAD_POPUP_H
#include "UIElements.h"

class LoadPopup {
public:
	sf::RectangleShape background;
	sf::Texture bgTexture;
	sf::RenderWindow* main_window;
	sf::Font default_font;

	Grid_Tiles** grid = nullptr;


	LoadPopup(sf::RenderWindow* window, std::string file);
	void LoadBgInit();
	void LoadDraw();
};

#endif
#pragma once
