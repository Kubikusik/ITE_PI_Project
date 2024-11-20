#include <iostream>
#include <SFML/Graphics.hpp>
#include "UIElements.h"
#include "FileManager.h"
#include "tinyxml2.h"

//constants:
#define WHITE sf::Color(255,255,255)
const int margin = 20;
sf::String image_format = ".png";

//all elements have their functions below

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

void Button::DrawItself(sf::RenderWindow& window) {
    window.draw(square);
}
void Button::Recolor(sf::Color color) {
    square.setFillColor(color);
}


//Labeled_Button:
Labeled_Button::Labeled_Button(int x, int y, int size, sf::String name,sf::String label, sf::Font &default_font, sf::Color button_color):Button(x, y, size, name, button_color) {
    
    //text next to button parameters initialization
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
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !simulate){

			//otwiera explorer do wyboru miejsca zapisu i zwraca œcie¿kê do pliku jako string
            std::string saveLocation = FileManager::SaveLocation(); 

			if (saveLocation != "") { //jak kliknie anuluj czy zamknie to nie zapisuje
				tinyxml2::XMLDocument doc; //nowy DOM pliku XML

				doc.Parse("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"); //logiczne
				tinyxml2::XMLElement* root = doc.NewElement("root"); //tworzy element root
				doc.InsertFirstChild(root); //otwiera <root>
                for (int i = 0; i < grid_num; i++) {
                    for (int j = 0; j < grid_num; j++) {
                        if (grid_list[i][j].default_color != WHITE) {
							tinyxml2::XMLElement* cell = doc.NewElement("cell"); //tworzy element cell
                            //wpisywanie parametrów
                            cell->SetAttribute("x", i);
                            cell->SetAttribute("y", j);
                            cell->SetAttribute("r", grid_list[i][j].default_color.r);
                            cell->SetAttribute("g", grid_list[i][j].default_color.g);
                            cell->SetAttribute("b", grid_list[i][j].default_color.b);
                            //zapisanie <cell *parametry*>
                            root->InsertEndChild(cell);
                        }
                    }
                }
                //tego syfu zapomnia³em lmao
                doc.InsertEndChild(root); //zamyka </root>
                //zapisanie
				doc.SaveFile(saveLocation.c_str());
			}

            //**Stara implementacja, NIE RUSZAÆ
            //for (int i = 0; i < grid_num; i++) {
            //    for (int j = 0; j < grid_num; j++) {
            //        sf::Color cell_color = grid_list[i][j].default_color;
            //        // Loop over each pixel within the cell area
            //        for (int k = 0; k < grid_size; k++) {
            //            for (int l = 0; l < grid_size; l++) {
            //                int x = i * (grid_size + 1) + k;
            //                int y = j * (grid_size + 1) + l;
            //                image.setPixel(x, y, cell_color);
            //            }
            //        }
            //    }
            //}
            //Save_Image(image, "image1");
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
    //if mouse is touching this button
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



//Plus_Button:
Plus_Button::Plus_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color) :Labeled_Button(x, y, size, name, label, default_font, button_color) {
    button_label.setPosition(sf::Vector2f(x + size/3, y));
}

void Plus_Button::Clicked(sf::RenderWindow& window, bool simulate, int &brush_size, sf::Text &counter, int grid_num) {
    if (square.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))) {
        //Hovering mouse over cell:
        Recolor(sf::Color(abs(default_color.r - 60), abs(default_color.g - 60), abs(default_color.b - 60)));


        //Left-clicking hovered-over cell = adding smth:
        if (!simulate && !clicked) {
            //printf("clicked + !\n");
            //if (brush_size < grid_num / 2) {
            if (brush_size < 9) {
                brush_size += 1;
                counter.setString("brush size:" + std::to_string(brush_size));
            }

        }
    } 
    else Recolor(default_color);

    clicked = true;
}
void Plus_Button::Release() {
    // Allow the button to be clicked again
    clicked = false;
}

void Plus_Button::ScrollUp(sf::RenderWindow& window, bool simulate, int& brush_size, sf::Text& counter) {
    if (!simulate) {
        if (brush_size < 9) {
            brush_size += 1;
            counter.setString("brush size:" + std::to_string(brush_size));
        }

    }
}


//Minus_Button:
Minus_Button::Minus_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color) :Labeled_Button(x, y, size, name, label, default_font, button_color) {
    button_label.setPosition(sf::Vector2f(x + size/3, y));
}

void Minus_Button::Clicked(sf::RenderWindow& window, bool simulate, int &brush_size, sf::Text &counter) {
   
    if (square.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))) {
        //Hovering mouse over cell:
        Recolor(sf::Color(abs(default_color.r - 60), abs(default_color.g - 60), abs(default_color.b - 60)));


        //Left-clicking hovered-over cell = adding smth:
        if (!simulate && !clicked) {
            if (brush_size > 1) {
                brush_size -= 1;
                counter.setString("brush size:" + std::to_string(brush_size));
            }
                
        }
    }
    else Recolor(default_color);

    clicked = true;
}

void Minus_Button::Release() {
    // Allow the button to be clicked again
    clicked = false;
}

void Minus_Button::ScrollDown(sf::RenderWindow& window, bool simulate, int& brush_size, sf::Text& counter) {
    if (!simulate) {
        if (brush_size >1) {
            brush_size -= 1;
            counter.setString("brush size:" + std::to_string(brush_size));
        }

    }
}



//Minus_Time_Button:

void Minus_Time_Button::Clicked(sf::RenderWindow& window, bool simulate, int& time_speed, sf::Text& counter) {

    if (square.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))) {
        //Hovering mouse over cell:
        Recolor(sf::Color(abs(default_color.r - 60), abs(default_color.g - 60), abs(default_color.b - 60)));


        //Left-clicking hovered-over cell = adding smth:
        if (!simulate && !clicked) {
            //printf("clicked - !\n");
            if (time_speed>5) {
                time_speed -= 5;
                counter.setString("time speed:" + std::to_string(time_speed));
            }

        }
    }
    else Recolor(default_color);

    clicked = true;
}

//Plus_Time_Button:
void Plus_Time_Button::Clicked(sf::RenderWindow& window, bool simulate, int& time_speed, sf::Text& counter) {

    if (square.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))) {
        //Hovering mouse over cell:
        Recolor(sf::Color(abs(default_color.r - 60), abs(default_color.g - 60), abs(default_color.b - 60)));


        //Left-clicking hovered-over cell = adding smth:
        if (!simulate && !clicked) {
            //printf("clicked + !\n");
            if (time_speed < 20) {
                time_speed += 5;
                counter.setString("time speed:" + std::to_string(time_speed));
            }

        }
    }
    else Recolor(default_color);

    clicked = true;
}