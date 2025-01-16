#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "tinyxml2.h"

// UI constants
extern const int grid_size;
extern const int button_size;
extern const int ui_size;
extern const int grid_num;
extern const int map_x;
extern const int map_y;
extern const int margin;

// Colors:
extern sf::Color DEAD_COLOR;
extern const sf::Color SAND_COLOR;
extern const sf::Color PLANT_COLOR;
extern const sf::Color WATER_COLOR;
extern const sf::Color FIRE_COLOR;
extern const sf::Color STEEL_COLOR;
extern const sf::Color STEAM_COLOR;
extern const sf::Color CONWAY_COLOR;
extern const sf::Color CABLE_COLOR;
extern const sf::Color POWER_COLOR;

// Constants:
extern sf::String image_format;
extern int font_size;

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
    std::pair<int, int> direction = { 1, 0 };

    int x;
    int y;

    void Recolor(sf::Color color);
    void UpdateSubstanceColor();
};

class UI_Element {
public:
    sf::Vector2f pos = sf::Vector2f(0, 0);

    UI_Element(int x, int y);
};

class Button : public UI_Element {
public:
    sf::RectangleShape square;
    sf::Color default_color;
    sf::String button_name;
    sf::Texture button_texture;

    Button(int x, int y, int size, sf::String name, sf::Color button_color, sf::Texture& texture);
    void DrawItself(sf::RenderWindow& window);
    void Recolor(sf::Color color);
};

class Labeled_Button : public Button {
public:
    sf::Text button_label;

    Labeled_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, sf::Texture& texture);
    void DrawItself(sf::RenderWindow& window);
    void Recolor(sf::Color color);
};

class Paint_Button : public Labeled_Button {
public:
    Substances substance;

    Paint_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, Substances set_substance, sf::Texture& texture);
    void ChangePaintColor(sf::Color& paint_color, Substances& paint_substance);
};

class Save_Button : public Labeled_Button {
public:
    Save_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, sf::Texture& texture) :Labeled_Button(x,y,size,name,label,default_font,button_color, texture) {};
    void Save_Image(sf::Image& image, sf::String filename);
    void Clicked(sf::RenderWindow& window, bool simulate, sf::Image& image, int grid_num, int grid_size, Grid_Tiles**& grid_list);
};

class Load_Button : public Labeled_Button {
public:
    Load_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, sf::Texture& texture) :Labeled_Button(x, y, size, name, label, default_font, button_color, texture) {};
    void Load_Image(sf::Image& image, sf::String filename);
    void Clicked(sf::RenderWindow& window, bool simulate, int grid_num, int grid_size, Grid_Tiles**& grid_list, bool* isVisible, bool* isNewFrame, tinyxml2::XMLDocument* stored_doc);
};

class Confirm_Button : public Labeled_Button {
public:
    Confirm_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, sf::Texture& texture) :Labeled_Button(x, y, size, name, label, default_font, button_color, texture) {};
    void Clicked(sf::RenderWindow& main_window, int grid_num, Grid_Tiles**& grid_list, tinyxml2::XMLDocument& doc, bool* isVisible);
    sf::FloatRect getBounds() const { return square.getGlobalBounds(); }
};


class Cancel_Button : public Labeled_Button {
public:
	Cancel_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, sf::Texture& texture) :Labeled_Button(x, y, size, name, label, default_font, button_color, texture) {};
    void Clicked(bool* isVisible);
    sf::FloatRect getBounds() const { return square.getGlobalBounds(); }
};

class Simulate_Button : public Labeled_Button {
public:
    bool clicked = false;
    bool is_active = false;
    sf::Texture on_texture;

    Simulate_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, sf::Texture& texture) :Labeled_Button(x, y, size, name, label, default_font, button_color, texture) {};
    void Simulate(bool& simulate);
    void ToggleSimulate(bool& simulate);
    void Release();
};

class Plus_Button : public Labeled_Button {
public:
    bool clicked = false;

    Plus_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, sf::Texture& texture);
    void Clicked(sf::RenderWindow& window, bool simulate, int& brush_size, sf::Text& counter, int grid_num);
    void Release();
    void ScrollUp(sf::RenderWindow& window, bool simulate, int& brush_size, sf::Text& counter);
};

class Minus_Button : public Labeled_Button {
public:
    bool clicked = false;

    Minus_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, sf::Texture& texture);
    void Clicked(sf::RenderWindow& window, bool simulate, int& brush_size, sf::Text& counter);
    void Release();
    void ScrollDown(sf::RenderWindow& window, bool simulate, int& brush_size, sf::Text& counter);
};

class Plus_Time_Button : public Plus_Button {
public:
    Plus_Time_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, sf::Texture& texture) :Plus_Button(x, y, size, name, label, default_font, button_color, texture) {};
    void Clicked(sf::RenderWindow& window, bool simulate, int& time_speed, sf::Text& counter);
};

class Minus_Time_Button : public Minus_Button {
public:
    Minus_Time_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, sf::Texture& texture):Minus_Button(x, y, size, name, label, default_font, button_color, texture) {};
    void Clicked(sf::RenderWindow& window, bool simulate, int& time_speed, sf::Text& counter);
};

class Menu_Popup_Plus_Button : public Plus_Button {
public:
    bool clicked = false;

    Menu_Popup_Plus_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, sf::Texture& texture):Plus_Button(x, y, size, name, label, default_font, button_color, texture) {};
    void Clicked(sf::RenderWindow& window, sf::Color& bg_color, sf::Text& counter, sf::String option);
    void DrawItself(sf::RenderWindow& window);
    void Release();
};

class Menu_Popup_Minus_Button : public Minus_Button {
public:
    bool clicked = false;

    Menu_Popup_Minus_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, sf::Texture &texture) :Minus_Button(x, y, size, name, label, default_font, button_color, texture) {};
    void Clicked(sf::RenderWindow& window, sf::Color& bg_color, sf::Text& counter, sf::String option);
    void DrawItself(sf::RenderWindow& window);
    void Release();
};

// Other Functions:
void UpdateGridBackground(Grid_Tiles**& grid_list, int grid_num, sf::Color new_bg_color);

#endif
#pragma once
