#include <iostream>
#include <SFML/Graphics.hpp>
#include "UIElements.h"

//definitions
#define WHITE sf::Color(255,255,255)


//constants:
const int margin = 20;
sf::String image_format = ".png";

//Grid:
void Grid_Tiles::Recolor(sf::Color color) {
    square.setFillColor(color);
}

//UI_Element:
UI_Element::UI_Element(int x, int y) {
    pos.x = x;
    pos.y = y;
}


//Button:
Button::Button(int x, int y, int size, sf::String name, sf::Color button_color):UI_Element(x, y) {
    square.setPosition(pos);
    square.setSize(sf::Vector2f(size, size));
    square.setFillColor(button_color);
    default_color = button_color;
    button_name = name;

}


//Labeled_Button:
Labeled_Button::Labeled_Button(int x, int y, int size, sf::String name,sf::String label, sf::Font &default_font, sf::Color button_color):Button(x, y, size, name, button_color) {
    
    //text next to button
    button_label.setFont(default_font);
    button_label.setString(label);
    button_label.setCharacterSize(30);
    button_label.setFillColor(WHITE);
    button_label.setScale(sf::Vector2f(1, 1));
    button_label.setPosition(sf::Vector2f(x + size + margin, y));
}

void Labeled_Button::DrawItself(sf::RenderWindow& window) {
    window.draw(square);
    window.draw(button_label);
}
void Labeled_Button::Recolor(sf::Color color) {
    square.setFillColor(color);
}


//Paint_Button:
void Paint_Button::ChangePaintColor(sf::Color& paint_color) {
    paint_color = default_color;
}


//Save_Button:
void Save_Button::Save_Image(sf::Image& image, sf::String filename) {
    filename = filename + image_format;
    image.saveToFile(filename);
}

void Save_Button::Clicked(sf::RenderWindow& window, bool simulate, sf::Image& image, int grid_num, int grid_size, Grid_Tiles**& grid_list) {
    if (square.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))) {
        //Hovering mouse over cell:
        Recolor(sf::Color(abs(default_color.r - 60), abs(default_color.g - 60), abs(default_color.b - 60)));


        //Left-clicking hovered-over cell = adding smth:
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !simulate) {
            for (int i = 0; i < grid_num; i++) {
                for (int j = 0; j < grid_num; j++) {
                    sf::Color cell_color = grid_list[i][j].default_color;
                    // Loop over each pixel within the cell area
                    for (int k = 0; k < grid_size; k++) {
                        for (int l = 0; l < grid_size; l++) {
                            int x = i * (grid_size + 1) + k;
                            int y = j * (grid_size + 1) + l;
                            image.setPixel(x, y, cell_color);
                        }
                    }
                }
            }
            Save_Image(image, "image1");
        }
        else Recolor(default_color);
    }
}

//Load_Button:
void Load_Button::Load_Image(sf::Image& image, sf::String filename) {
    filename = filename + image_format;
    image.loadFromFile(filename);
}

void Load_Button::Clicked(sf::RenderWindow& window, bool simulate, sf::Image& image, int grid_num, int grid_size, Grid_Tiles**& grid_list) {
    if (square.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))) {
        //Hovering mouse over cell:
        Recolor(sf::Color(abs(default_color.r - 60), abs(default_color.g - 60), abs(default_color.b - 60)));

        //Left-clicking hovered-over cell = adding smth:
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !simulate) {
            Load_Image(image, "image1");
            for (int i = 0; i < grid_num; i++) {
                for (int j = 0; j < grid_num; j++) {
                    grid_list[i][j].default_color = image.getPixel(i * (grid_size + 1), j * (grid_size + 1));
                }
            }
        }
        else Recolor(default_color);
    }

}

//Simulate_Button:
void Simulate_Button::Simulate(bool& simulate) {
    simulate = !simulate;
    if (simulate) {
        default_color = sf::Color(0, 255, 0);
    }
    else default_color = sf::Color(255, 0, 0);
}

void Simulate_Button::ToggleSimulate(bool& simulate) {
    // Only toggle simulate when the button is first pressed
    if (!clicked) {
        is_active = !is_active;
        simulate = is_active; // update simulate state
        if (is_active) {
            default_color = sf::Color(0, 255, 0);
        }
        else default_color = sf::Color(255, 0, 0); // green for active, red for inactive
        square.setFillColor(default_color);
        clicked = true; // mark as clicked
    }
}



void Simulate_Button::Release() {
    // Allow the button to be clicked again
    clicked = false;
}

