#ifndef LOAD_POPUP_H
#define LOAD_POPUP_H
#include "UIElements.h"
#include "tinyxml2.h"

class LoadPopup {
public:
	sf::RectangleShape background;
	//sf::Texture bgTexture;
	sf::RenderWindow* main_window;
	sf::Font default_font;

	sf::RectangleShape grid_background;
	sf::RectangleShape button_background;

	Grid_Tiles** grid = nullptr;
	sf::Texture temp_texture;
	sf::Clock* clock;

	bool first_time = true;
	bool loaded_preset;

	tinyxml2::XMLDocument doc;

	const float margin = 10.0f;
	bool isVisible = false;
	bool* NewFrame;

	LoadPopup(sf::RenderWindow* window, sf::Font font, sf::Clock* clock, bool*NewFrame);
	void LoadBgInit();
	void LoadDraw(Grid_Tiles** main_grid);
	bool LoadPreset();

};

#endif
#pragma once
