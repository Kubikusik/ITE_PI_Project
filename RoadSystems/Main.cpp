#include <iostream>
#include <SFML/Graphics.hpp>
#include "UIElements.h"
#include "ConwaysGameOfLife.h"
#include "ConwaysGameOfLifeMaksiowy.h"

//ui consts
const int grid_size = 19;
const int button_size = 49;
const int ui_size = 300;
const int grid_num = 40;
const int map_x = grid_num * (grid_size + 1);
const int map_y = grid_num * (grid_size + 1);
const int margin = 20;

//conway consts
const int sim_tpf = 2; //ticks per frame of simulations
const sf::Color ALIVE_COLOR(255, 0, 0);
const sf::Color DEAD_COLOR(255, 255, 255);

sf::Font default_font;

void MakeButtons(std::vector<Paint_Button>& color_button_list, std::vector<sf::Color>& color_list) {

    int i = 0;
    Paint_Button sand_button(0, i*(50+margin), 50, "yellow_b", "Sand", default_font, sf::Color(246, 215, 176));
    color_button_list.push_back(sand_button);
    color_list.push_back(sf::Color(246, 215, 176));
    i++;
    Paint_Button plant_button(0, i * (50 + margin), 50, "green_b", "plant", default_font, sf::Color(53, 136, 86));
    color_button_list.push_back(plant_button);
    color_list.push_back(sf::Color(53, 136, 86));
    i++;
    Paint_Button water_button(0, i * (50 + margin), 50, "water_b", "Water", default_font, sf::Color(142, 194, 228));
    color_button_list.push_back(water_button);
    color_list.push_back(sf::Color(142, 194, 228));
    i++;
    Paint_Button fire_button(0, i * (50 + margin), 50, "red_b", "Fire", default_font, sf::Color(229, 81, 0));
    color_button_list.push_back(fire_button);
    color_list.push_back(sf::Color(229, 81, 0));
    i++;
    Paint_Button steam_button(0, i * (50 + margin), 50, "steam_b", "Steam", default_font, sf::Color(111, 106, 96));
    color_button_list.push_back(steam_button);
    color_list.push_back(sf::Color(111, 106, 96));
}

int main()
{
    default_font.loadFromFile("OpenSansRegular.ttf");
    int delta = 0;

    int searched_x, searched_y = 0;
    int neigbor_counter = 0;
    bool is_focused = true;
    bool simulate = false;
    sf::Color paint_color(246, 215, 176);
    sf::RenderWindow window(sf::VideoMode(map_x + ui_size, map_y), "Klocki LOL", sf::Style::Close);
    std::vector<sf::Color> color_list;
    
    window.setFramerateLimit(60);
    
    
    std::vector<Paint_Button> button_list;

    sf::Image image;
    image.create(grid_num*(grid_size+1), grid_num*(grid_size + 1), sf::Color(255,255,255));

    Grid_Tiles** grid_list = new Grid_Tiles *[grid_num];
    for (int row = 0; row < grid_num; row++) {
        grid_list[row] = new Grid_Tiles[grid_num];
        for (int column = 0; column < grid_num; column++) {
            grid_list[row][column].square.setFillColor(sf::Color(255, 255, 255));
            grid_list[row][column].square.setPosition(sf::Vector2f(ui_size + row * (grid_size+1), column * (grid_size + 1)));
            grid_list[row][column].square.setSize(sf::Vector2f(grid_size, grid_size));
        }
    }

    MakeButtons(button_list, color_list);

    int enumerator_of_button = 5;
    Save_Button save_button(0, enumerator_of_button *(50 + margin), 50, "save_b", "Save", default_font, sf::Color(50, 50, 50));
    enumerator_of_button++;
    Load_Button load_button(0, enumerator_of_button *(50 + margin), 50, "load_b", "Load", default_font, sf::Color(50, 50, 50));
    enumerator_of_button++;
    Simulate_Button simulate_button(0, enumerator_of_button * (50 + margin), 50, "sim_b", "Run", default_font, sf::Color(255, 0, 0));


    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)        //window is closed
                window.close();
            if (event.type == sf::Event::GainedFocus)   //window in focus
                is_focused = true;
            if (event.type == sf::Event::LostFocus)     //window is out of focus
                is_focused = false;

            if (event.type = sf::Event::MouseMoved) {
                
                //if window is being focused on
                if (is_focused) {
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    searched_x = (mouse_pos.x - ui_size) / (grid_size + 1);
                    searched_y = (mouse_pos.y) / (grid_size + 1);
                    //search for which cell is hovered over
                    if (mouse_pos.x > ui_size && mouse_pos.y > 0 && mouse_pos.x < map_x + ui_size && mouse_pos.y < map_y) {



                        //Hovering mouse over cell / grey-out
                        grid_list[searched_x][searched_y].Recolor(sf::Color(abs(grid_list[searched_x][searched_y].default_color.r - 60), abs(grid_list[searched_x][searched_y].default_color.g - 60), abs(grid_list[searched_x][searched_y].default_color.b - 60)));


                        //left click action on cell
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !simulate) {
                            grid_list[searched_x][searched_y].default_color = paint_color;
                        }


                        //right click action on cell
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !simulate) {
                            grid_list[searched_x][searched_y].default_color = sf::Color(255, 255, 255);
                        }
                    }
                }

                
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { //escape pressed closes window
            window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { //Space pressed closes window
            window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && delta % sim_tpf == 0) { //C pressed closes window sometimes
            window.close();
        }
        

        //setting what color each button on keyboard does:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) paint_color = button_list[0].default_color;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) paint_color = button_list[1].default_color;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) paint_color = button_list[2].default_color;

        //recolor back after hovering
        for (int i = 0; i < grid_num; i++) {
            for (int j = 0; j < grid_num; j++) {
                grid_list[i][j].Recolor(grid_list[i][j].default_color);
            }
        }
        

        //Color buttons interactions
        for (auto& each : button_list) {

            if (each.square.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))) {

                //Hovering mouse over cell:
                each.Recolor(sf::Color(abs(each.default_color.r - 60), abs(each.default_color.g - 60), abs(each.default_color.b - 60)));


                //Left-clicking hovered-over cell = adding smth:
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    each.ChangePaintColor(paint_color);
                }
                else each.Recolor(each.default_color);

            }
        }

        //Save Button
        save_button.Clicked(window, simulate, image, grid_num, grid_size, grid_list);

        load_button.Clicked(window, simulate, image, grid_num, grid_size, grid_list);

        //Start simulation button
        if (simulate_button.square.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) || sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            //Hovering mouse over cell:
            simulate_button.Recolor(sf::Color(abs(simulate_button.default_color.r - 60), abs(simulate_button.default_color.g - 60), abs(simulate_button.default_color.b - 60)));


            //Left-clicking hovered-over cell = adding smth:
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                simulate_button.ToggleSimulate(simulate);
            }
            else {
                simulate_button.Release();
            }
        }
        else {
            simulate_button.Recolor(simulate_button.default_color);
        }

        //simulation
        if (simulate && (delta == 0)) {
            //ConwaysGameOfLife(grid_list, grid_num);
            //ConwaysGameOfLifeMaksiowy(grid_list, grid_num);
            //ConwaysGameOfLifeManyColor(grid_list, grid_num, color_list);
            ConwaysPhysics(grid_list, grid_num, color_list);
        }


        //Draw Grid
        window.clear();
        for (int i = 0; i < grid_num; i++) {
            for (int j = 0; j < grid_num; j++) {
                window.draw(grid_list[i][j].square);
            }
        }

        //Draw Buttons
        for (auto& each : button_list)
        {
            each.DrawItself(window);
        }
        save_button.DrawItself(window);
        load_button.DrawItself(window);
        simulate_button.DrawItself(window);
        
        
        //update screen
        window.display();
        delta += 1;
        delta = delta % sim_tpf;

    }

    return 0;
}