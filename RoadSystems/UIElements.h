#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include <SFML/Graphics.hpp>
#include <iostream>

//definitions:
#define WHITE sf::Color(255,255,255)

#pragma once
class Grid_Tiles {
public:
	sf::RectangleShape square;
	sf::Color default_color = WHITE;
	sf::Color temp_color = WHITE;

	void Recolor(sf::Color color);
};

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
	Paint_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font &default_font, sf::Color button_color) : Labeled_Button(x, y, size, name, label, default_font, button_color) {};
	void ChangePaintColor(sf::Color& paint_color);
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




#endif