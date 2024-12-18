#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include <SFML/Graphics.hpp>
#include <iostream>

//definitions:
#define WHITE sf::Color(255,255,255)


enum Substances {
	DEAD,
	WATER,
	SAND,
	FIRE,
	PLANT,
	STEEL,
	STEAM,
	CONWAY,
	CABLE,
	POWER
	
};

void ChangeBackgroundColor(sf::Color color);
sf::Color GetBgColor();


#pragma once
class Grid_Tiles {
public:
	Substances substance = DEAD;
	Substances next_substance = DEAD;
	sf::RectangleShape square;
	sf::Color default_color = GetBgColor();
	short int way = 1;
	bool isUpdated = false;
	bool growing = true;
	bool isFalling = false;
	std::pair<int, int> direction = { 1,0 };

	int x;
	int y;

	void Recolor(sf::Color color);
	void UpdateSubstanceColor();
};

void UpdateGridBackground(Grid_Tiles**& grid_list, int grid_num, sf::Color new_bg_color);


class UI_Element {
public:
	sf::Vector2f pos = sf::Vector2f(0,0);

	UI_Element(int x, int y);
};

class Button : public UI_Element {
public:
	sf::RectangleShape square;
	sf::Color default_color;
	sf::String button_name;

	Button(int x, int y, int size, sf::String name, sf::Color button_color);
	void DrawItself(sf::RenderWindow& window);
	void Recolor(sf::Color color);
};

class Labeled_Button : public Button {
public:
	sf::Text button_label;
	sf::Vector2f text_pos;

	Labeled_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font &default_font, sf::Color button_color);
	void DrawItself(sf::RenderWindow& window);
	void Recolor(sf::Color color);
};

class Paint_Button : public Labeled_Button {
public:
	Substances substance;

	Paint_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, Substances set_substance);
	void ChangePaintColor(sf::Color& paint_color, Substances &paint_substance);
};

class Save_Button : public Labeled_Button {
public:
	Save_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font &default_font, sf::Color button_color) : Labeled_Button(x, y, size, name, label, default_font, button_color) {};
	void Save_Image(sf::Image& image, sf::String filename);
	void Clicked(sf::RenderWindow& window, bool simulate, sf::Image& image, int grid_num, int grid_size, Grid_Tiles**& grid_list);
};

class Load_Button : public Labeled_Button {
public:
	Load_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font &default_font, sf::Color button_color) : Labeled_Button(x, y, size, name, label, default_font, button_color) {};
	void Load_Image(sf::Image& image, sf::String filename);
	void Clicked(sf::RenderWindow& window, bool simulate, sf::Image& image, int grid_num, int grid_size, Grid_Tiles**& grid_list);
};

class Simulate_Button : public Labeled_Button {
public:
	bool clicked = false;
	bool is_active = false;

	Simulate_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font &default_font, sf::Color button_color) :Labeled_Button(x, y, size, name, label, default_font, button_color) {};
	void Simulate(bool& simulate);
	void ToggleSimulate(bool& simulate);
	void Release();
};

class Plus_Button : public Labeled_Button {
public:
	bool clicked = false;
	Plus_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color);
	void Clicked(sf::RenderWindow& window, bool simulate, int &brush_size, sf::Text &counter, int grid_num);
	void Release();
	void ScrollUp(sf::RenderWindow& window, bool simulate, int& brush_size, sf::Text& counter);
};

class Minus_Button : public Labeled_Button {
public:
	bool clicked = false;
	Minus_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color);
	void Clicked(sf::RenderWindow& window, bool simulate, int &brush_size, sf::Text &counter);
	void Release();
	void ScrollDown(sf::RenderWindow& window, bool simulate, int& brush_size, sf::Text& counter);
};

class Plus_Time_Button : public Plus_Button {
public:
	Plus_Time_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color) : Plus_Button(x, y, size, name, label, default_font, button_color) {};
	void Clicked(sf::RenderWindow& window, bool simulate, int& time_speed, sf::Text& counter);
};

class Minus_Time_Button : public Minus_Button {
public:
	Minus_Time_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color) : Minus_Button(x, y, size, name, label, default_font, button_color) {};
	void Clicked(sf::RenderWindow& window, bool simulate, int& time_speed, sf::Text& counter);
};



class Menu_Popup {
public:

	bool clicked = false;

	void ToggleMenuPopup(sf::RenderWindow& main_window, sf::Font default_font);
	void Release();

	void MenuHandler(sf::RenderWindow& main_window, sf::Font default_font);

};


class Menu_Popup_Plus_Button : Plus_Button {
public:
	bool clicked = false;

	Menu_Popup_Plus_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color) : Plus_Button(x, y, size, name, label, default_font, button_color) {};
	void Clicked(sf::RenderWindow& window, sf::Color& bg_color, sf::Text& counter, sf::String option);
	void DrawItself(sf::RenderWindow& window);
	void Release();
};

class Menu_Popup_Minus_Button : Plus_Button {
public:
	bool clicked = false;

	Menu_Popup_Minus_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color) : Plus_Button(x, y, size, name, label, default_font, button_color) {};
	void Clicked(sf::RenderWindow& window, sf::Color& bg_color, sf::Text& counter, sf::String option);
	void DrawItself(sf::RenderWindow& window);
	void Release();
};

#endif