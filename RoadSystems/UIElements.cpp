#include <iostream>
#include <SFML/Graphics.hpp>
#include "UIElements.h"
#include "FileManager.h"
#include "tinyxml2.h"

//constants:
#define WHITE sf::Color(255,255,255)
const int margin = 20;
sf::String image_format = ".png";
int font_size = 30;


//constants:
//const sf::Color ALIVE_COLOR(255, 0, 0);
sf::Color DEAD_COLOR(255, 255, 255);
const sf::Color SAND_COLOR(246, 215, 176);
const sf::Color PLANT_COLOR(53, 136, 86);
const sf::Color WATER_COLOR(142, 194, 228);
const sf::Color FIRE_COLOR(229, 81, 0);
const sf::Color STEEL_COLOR(111, 106, 96);
const sf::Color STEAM_COLOR(90, 90, 96);
const sf::Color CONWAY_COLOR(5, 15, 45);
const sf::Color CABLE_COLOR(45, 15, 45);
const sf::Color POWER_COLOR(255,255,51);


void ChangeBackgroundColor(sf::Color color) {
    DEAD_COLOR = color;
}
 
sf::Color GetBgColor() {
    return DEAD_COLOR;
}

//all elements have their functions below

void UpdateGridBackground(Grid_Tiles** &grid_list, int grid_num, sf::Color new_bg_color) {
    for (int row = 0; row < grid_num; row++) {
        for (int column = 0; column < grid_num; column++) {
            if (grid_list[row][column].substance == DEAD) {
                grid_list[row][column].default_color = new_bg_color;
            }
            grid_list[row][column].square.setFillColor(new_bg_color);

        }
    }
}

//Grid:
void Grid_Tiles::Recolor(sf::Color color) {
    square.setFillColor(color);
}

void Grid_Tiles::UpdateSubstanceColor() {
    substance = next_substance;
    if (substance == DEAD) {
        default_color = DEAD_COLOR;
    }
    else if (substance == SAND) {
        default_color = SAND_COLOR;
    }
    else if (substance == PLANT) {
        default_color = PLANT_COLOR;
    }
    else if (substance == WATER) {
        default_color = WATER_COLOR;
    }
    else if (substance == FIRE) {
        default_color = FIRE_COLOR;
    }
    else if (substance == STEEL) {
        default_color = STEEL_COLOR;
    }
    else if (substance == STEAM) {
        default_color = STEAM_COLOR;
    }
    else if (substance == CONWAY) {
        default_color = CONWAY_COLOR;
    }
    else if (substance == CABLE) {
        default_color = CABLE_COLOR;
    }
    else if (substance == POWER) {
        default_color = POWER_COLOR;
    }

    Recolor(default_color);
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
    button_label.setCharacterSize(font_size);
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

Paint_Button::Paint_Button(int x, int y, int size, sf::String name, sf::String label, sf::Font& default_font, sf::Color button_color, Substances set_substance) :Labeled_Button(x, y, size, name, label, default_font, button_color) {
    substance = set_substance;
}


void Paint_Button::ChangePaintColor(sf::Color& paint_color, Substances &paint_substance) {
    paint_color = default_color;
    paint_substance = substance;

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

			//opens dialog window to save file
            std::string saveLocation = FileManager::SaveLocation(); 

			if (saveLocation != "") { //if user didn't cancel the dialog
				tinyxml2::XMLDocument doc; //new xml 
				doc.Parse("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"); //standard xml header
				tinyxml2::XMLElement* bg = doc.NewElement("bg"); //new bg element
				bg->SetAttribute("r", DEAD_COLOR.r);
				bg->SetAttribute("g", DEAD_COLOR.g);
				bg->SetAttribute("b", DEAD_COLOR.b);
				doc.InsertEndChild(bg); //inserting bg element to xml
                for (int i = 0; i < grid_num; i++) {
                    for (int j = 0; j < grid_num; j++) {
                        if (grid_list[i][j].substance != DEAD) {
							tinyxml2::XMLElement* cell = doc.NewElement("cell"); //new cell element
							//inserting cell attributes
                            cell->SetAttribute("x", i);
                            cell->SetAttribute("y", j);
                            cell->SetAttribute("r", grid_list[i][j].default_color.r);
                            cell->SetAttribute("g", grid_list[i][j].default_color.g);
                            cell->SetAttribute("b", grid_list[i][j].default_color.b);
							cell->SetAttribute("s", grid_list[i][j].substance);
							//writing cell to xml
							doc.InsertEndChild(cell);
                        }
                    }
                }
				//save xml to file
				doc.SaveFile(saveLocation.c_str());
			}
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
            //TODO: Grid will be cleared popup message
            std::string file = FileManager::GetLocation(); //opens explorer to choose file and returns path as string
            if (file != "") {
                if (file.find(".xml") == std::string::npos) {
                    //TODO: error popup
                    Recolor(default_color);
                    return;
                }
                //Clear grid (WIP: will have popup notification)
                for (int i = 0; i < grid_num; i++) {
                    for (int j = 0; j < grid_num; j++) {
                        grid_list[i][j].default_color = DEAD_COLOR;
                        grid_list[i][j].substance = DEAD;
                    }
                }

                tinyxml2::XMLDocument doc; //initialize new XML DOM
                doc.LoadFile(file.c_str()); //loads file
				tinyxml2::XMLElement* root = doc.FirstChildElement("bg"); //gets bg element
				ChangeBackgroundColor(sf::Color(root->IntAttribute("r"), root->IntAttribute("g"), root->IntAttribute("b"))); //sets bg color
				UpdateGridBackground(grid_list, grid_num, DEAD_COLOR); //updates grid background color
                tinyxml2::XMLElement* cell = doc.FirstChildElement("cell"); //gets first cell element
                while (cell != NULL) { //while there are cells read and set their attributes
                    int x = cell->IntAttribute("x");
                    int y = cell->IntAttribute("y");
                    int r = cell->IntAttribute("r");
                    int g = cell->IntAttribute("g");
                    int b = cell->IntAttribute("b");
					Substances s = (Substances)cell->IntAttribute("s");
                    grid_list[x][y].default_color = sf::Color(r, g, b);
					grid_list[x][y].substance = s;
                    cell = cell->NextSiblingElement("cell"); //go to next cell
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

/*
void Menu_Popup::ToggleMenuPopup(sf::RenderWindow& main_window, sf::Font default_font) {
    printf("Opening menu\n");
    MenuHandler(main_window, default_font);
    clicked = true;
}

void Menu_Popup::Release() {
    clicked = false;
}

void Menu_Popup::MenuHandler(sf::RenderWindow& main_window, sf::Font default_font) {
    int menu_size_x = 600;
    int menu_size_y = 600;
    sf::RenderWindow menu_window(sf::VideoMode(menu_size_x, menu_size_y), "SideMenu", sf::Style::None);

    sf::Color bg_color = GetBgColor(); // Get current background color

    // Red color adjustment buttons
    Menu_Popup_Plus_Button increase_R(50, 50, 50, "plus_r", "+", default_font, sf::Color(200, 0, 0));
    Menu_Popup_Minus_Button decrease_R(100, 50, 50, "minus_r", "-", default_font, sf::Color(200, 0, 0));

    // Green color adjustment buttons
    Menu_Popup_Plus_Button increase_G(50, 150, 50, "plus_g", "+", default_font, sf::Color(0, 200, 0));
    Menu_Popup_Minus_Button decrease_G(100, 150, 50, "minus_g", "-", default_font, sf::Color(0, 200, 0));

    // Blue color adjustment buttons
    Menu_Popup_Plus_Button increase_B(50, 250, 50, "plus_b", "+", default_font, sf::Color(0, 0, 200));
    Menu_Popup_Minus_Button decrease_B(100, 250, 50, "minus_b", "-", default_font, sf::Color(0, 0, 200));

    sf::Text color_display;
    color_display.setFont(default_font);
    color_display.setCharacterSize(20);
    color_display.setPosition(300, 50);
    color_display.setFillColor(sf::Color::Black);

    sf::Event event;
    while (menu_window.isOpen()) {
        while (menu_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                menu_window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                menu_window.close();
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            increase_R.Clicked(menu_window, bg_color, color_display, "r");
            decrease_R.Clicked(menu_window, bg_color, color_display, "r");

            increase_G.Clicked(menu_window, bg_color, color_display, "g");
            decrease_G.Clicked(menu_window, bg_color, color_display, "g");

            increase_B.Clicked(menu_window, bg_color, color_display, "b");
            decrease_B.Clicked(menu_window, bg_color, color_display, "b");

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

        menu_window.clear(bg_color); // Use current background color for the menu
        increase_R.DrawItself(menu_window);
        decrease_R.DrawItself(menu_window);
        increase_G.DrawItself(menu_window);
        decrease_G.DrawItself(menu_window);
        increase_B.DrawItself(menu_window);
        decrease_B.DrawItself(menu_window);
        menu_window.draw(color_display);
        menu_window.display();
    }
}
*/


void Menu_Popup_Plus_Button::DrawItself(sf::RenderWindow& window) {
    window.draw(square);
    window.draw(button_label);
}

void Menu_Popup_Minus_Button::DrawItself(sf::RenderWindow& window) {
    window.draw(square);
    window.draw(button_label);
}


void Menu_Popup_Plus_Button::Clicked(sf::RenderWindow& window, sf::Color& bg_color, sf::Text& counter, sf::String option) {
    if (square.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))) {
        Recolor(sf::Color(abs(default_color.r - 60), abs(default_color.g - 60), abs(default_color.b - 60)));

        if (!clicked) { // Only process the click if it hasn't already been handled
            if (option == "r" && bg_color.r < 245) {
                bg_color.r += 10;
                counter.setString("background (R): " + std::to_string(bg_color.r));
            }
            if (option == "g" && bg_color.g < 245) {
                bg_color.g += 10;
                counter.setString("background (G): " + std::to_string(bg_color.g));
            }
            if (option == "b" && bg_color.b < 245) {
                bg_color.b += 10;
                counter.setString("background (B): " + std::to_string(bg_color.b));
            }

            clicked = true; // Mark as clicked
        }
    }
    else {
        Recolor(default_color);
    }
}

void Menu_Popup_Plus_Button::Release() {
    clicked = false; // Reset the clicked flag
}

void Menu_Popup_Minus_Button::Clicked(sf::RenderWindow& window, sf::Color& bg_color, sf::Text& counter, sf::String option) {
    if (square.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))) {
        //Hovering mouse over cell:
        Recolor(sf::Color(abs(default_color.r - 60), abs(default_color.g - 60), abs(default_color.b - 60)));

        if (!clicked) {
            if (option == "r" && bg_color.r > 9) {
                bg_color.r -= 10;
                counter.setString("background (R): " + std::to_string(bg_color.r));
            }
            if (option == "g" && bg_color.g > 9) {
                bg_color.g -= 10;
                counter.setString("background (G): " + std::to_string(bg_color.g));
            }
            if (option == "b" && bg_color.b > 9) {
                bg_color.b -= 10;
                counter.setString("background (B): " + std::to_string(bg_color.b));
            }
        }
        else Recolor(default_color);

        clicked = true;
    }
}

void Menu_Popup_Minus_Button::Release() {
    clicked = false; // Reset the clicked flag
}