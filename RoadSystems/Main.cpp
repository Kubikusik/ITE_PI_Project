#include <iostream>
#include <SFML/Graphics.hpp>
#include "UIElements.h"
//#include "ConwaysGameOfLife.h"
#include "ConwaysGameOfLifeMaksiowy.h"

//ui consts
const int grid_size = 19;
const int button_size = 50;
const int ui_size = 300;
const int grid_num = 42;
const int map_x = grid_num * (grid_size + 1);
const int map_y = grid_num * (grid_size + 1);
const int margin = 20;
sf::Font default_font;


//Function for making all the color buttons (all substances)
void MakeButtons(std::vector<Paint_Button>& color_button_list, std::vector<sf::Color>& color_list) {

    int i = 0;
    Paint_Button sand_button(0, i*(button_size + margin), button_size, "yellow_b", "Sand", default_font, sf::Color(246, 215, 176), SAND);
    color_button_list.push_back(sand_button);
    color_list.push_back(sf::Color(246, 215, 176));
    i++;
    Paint_Button plant_button(0, i * (button_size + margin), button_size, "green_b", "plant", default_font, sf::Color(53, 136, 86), PLANT);
    color_button_list.push_back(plant_button);
    color_list.push_back(sf::Color(53, 136, 86));
    i++;
    Paint_Button water_button(0, i * (button_size + margin), button_size, "water_b", "Water", default_font, sf::Color(142, 194, 228), WATER);
    color_button_list.push_back(water_button);
    color_list.push_back(sf::Color(142, 194, 228));
    i++;
    Paint_Button fire_button(0, i * (button_size + margin), button_size, "red_b", "Fire", default_font, sf::Color(229, 81, 0), FIRE);
    color_button_list.push_back(fire_button);
    color_list.push_back(sf::Color(229, 81, 0));
    i++;
    Paint_Button steel_button(0, i * (button_size + margin), button_size, "steel_b", "Steel", default_font, sf::Color(111, 106, 96), STEEL);
    color_button_list.push_back(steel_button);
    color_list.push_back(sf::Color(111, 106, 96));
    i++;
    Paint_Button conway_button(0, i * (button_size + margin), button_size, "conway_b", "Nanomachine", default_font, sf::Color(5, 15, 45), CONWAY);
    color_button_list.push_back(conway_button);
    color_list.push_back(sf::Color(5, 15, 45));
    i++;
    Paint_Button cable_button(0, i * (button_size + margin), button_size, "cable_b", "Cable", default_font, sf::Color(45, 15, 45), CABLE);
    color_button_list.push_back(cable_button);
    color_list.push_back(sf::Color(45, 15, 45));
}

int main()
{

    srand(time(NULL)); //used for some randomness across the program
    //declaring all as needed:
    int sim_tpf = 1; //ticks per frame of simulations (sim_tpf = 2 means that only once per 2 frames there will be a simulation update)
    int time_speed = 5;
    default_font.loadFromFile("OpenSansRegular.ttf"); //loading the font used in all text
    int delta = 0; //delta time for simulation use
    int brush_size = 1;
    sf::Color bg_color(250, 250, 250);
    ChangeBackgroundColor(bg_color);

    


    int searched_x, searched_y = 0; //needed for searching mouse position on screen
    bool is_focused = true; //is window being focused on
    bool simulate = false; //is simulation running
    sf::Color paint_color(246, 215, 176); //default left click color (sand)
    Substances paint_substance = SAND; //default left click substance (sand)
    sf::RenderWindow window(sf::VideoMode(map_x + ui_size, map_y), "Klocki LOL", sf::Style::Default); //initialasing the window
    std::vector<sf::Color> color_list; //list of all colors of buttons, given to other functions
    
    window.setFramerateLimit(60); //stable framerate, so the simulation is constant and not changing with processing power fluctuactions
    //sf::View view = window.getDefaultView();
    //view.zoom(2.0f);
    //window.setView(view);
    
    std::vector<Paint_Button> button_list; //list of color buttons (all the ones that give substance)

    sf::Image image; //image used for importing/exporting maps
    image.create(grid_num*(grid_size+1), grid_num*(grid_size + 1), GetBgColor()); //making white image as basis

    Grid_Tiles** grid_list = new Grid_Tiles *[grid_num]; //making a 2d array of objects of type Grid_Tiles (from "UIElements.h")
    for (int row = 0; row < grid_num; row++) {
        grid_list[row] = new Grid_Tiles[grid_num];
        for (int column = 0; column < grid_num; column++) {
            grid_list[row][column].square.setFillColor(GetBgColor()); //default color = white
            grid_list[row][column].square.setPosition(sf::Vector2f(ui_size + row * (grid_size+1), column * (grid_size + 1)));
            grid_list[row][column].square.setSize(sf::Vector2f(grid_size, grid_size));
            grid_list[row][column].x = row;
            grid_list[row][column].y = column;
        }
    }

    MakeButtons(button_list, color_list);


    //continuing button creation, functionality buttons
    int enumerator_of_button = 7;
    Save_Button save_button(0, enumerator_of_button *(button_size + margin), button_size, "save_b", "Save", default_font, sf::Color(50, 50, 50));
    enumerator_of_button++;
    Load_Button load_button(0, enumerator_of_button *(button_size + margin), button_size, "load_b", "Load", default_font, sf::Color(50, 50, 50));
    enumerator_of_button++;
    Simulate_Button simulate_button(0, enumerator_of_button * (button_size + margin), button_size, "sim_b", "Run", default_font, sf::Color(255, 0, 0));
    enumerator_of_button++;
    
    Minus_Button decrease_brush_button(0, enumerator_of_button * (button_size + margin), button_size, "minus_b", "-", default_font, sf::Color(255, 255, 255, 10));
    Plus_Button increase_brush_button(5* button_size, enumerator_of_button * (button_size + margin), button_size, "plus_b", "+", default_font, sf::Color(255, 255, 255, 10));
    Labeled_Button brush_size_label(-margin/2, enumerator_of_button * (button_size + margin), button_size, "brush_counter", "brush size:" + std::to_string(brush_size), default_font, sf::Color(0, 0, 0, 0));
    enumerator_of_button++;
    Minus_Time_Button increase_time_button(0, enumerator_of_button * (button_size + margin), button_size, "minus_t_b", "-", default_font, sf::Color(255, 255, 255,10));
    Plus_Time_Button decrease_time_button(5 * button_size, enumerator_of_button * (button_size + margin), button_size, "plus_t_b", "+", default_font, sf::Color(255, 255, 255,10));
    Labeled_Button time_speed_label(-margin / 2, enumerator_of_button * (button_size + margin), button_size, "time_counter", "time speed:" + std::to_string(sim_tpf), default_font, sf::Color(0, 0, 0, 0));
    //main loop, running while window wasnt closed yet


    Menu_Popup menu_popup;

    while (window.isOpen())
    {
        
        //checking if any event happened
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)        //if window is closed
                window.close();
            if (event.type == sf::Event::GainedFocus) {   //if window in focus
                is_focused = true;
                window.setActive(true);
            }
            if (event.type == sf::Event::LostFocus) {    //if window is out of focus
                is_focused = false;
                window.setActive(false);
            }

            if (event.type == sf::Event::Resized)       //if window is 
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));

            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape && is_focused) { //escape pressed closes window
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && is_focused) {
                menu_popup.ToggleMenuPopup(window, default_font);
                is_focused = false; // Main window loses focus
                UpdateGridBackground(grid_list, grid_num, GetBgColor());
            }

            if (is_focused) {
                if (event.type == sf::Event::MouseWheelMoved) {
                    if (event.mouseWheel.delta > 0) {
                        //Plus Button clicking
                        //brush_size += 1;
                        increase_brush_button.ScrollUp(window, simulate, brush_size, brush_size_label.button_label);


                    }
                    else if (event.mouseWheel.delta < 0) {
                        //brush_size -= 1;
                        decrease_brush_button.ScrollDown(window, simulate, brush_size, brush_size_label.button_label);
                    }
                }
            }

            if (event.type = sf::Event::MouseMoved) { //if mouse is moved
                
                //if window is being focused on
                if (is_focused) {

                    //get position in pixels and translate it to what grid tile is that:
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    searched_x = (mouse_pos.x - ui_size) / (grid_size + 1);
                    searched_y = (mouse_pos.y) / (grid_size + 1);
                    //search for which cell is hovered over
                    if (mouse_pos.x > ui_size && mouse_pos.y > 0 && mouse_pos.x < map_x + ui_size && mouse_pos.y < map_y) {



                        //Hovering mouse over cell / grey-out
                        grid_list[searched_x][searched_y].Recolor(sf::Color(abs(grid_list[searched_x][searched_y].default_color.r - 60), abs(grid_list[searched_x][searched_y].default_color.g - 60), abs(grid_list[searched_x][searched_y].default_color.b - 60)));


                        //left click action on cell
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !simulate) {
                            int minimal_x = std::max(searched_x - brush_size +1, 0);
                            int maximal_x = std::min(searched_x + brush_size -1, grid_num - 1);

                            int minimal_y = std::max(searched_y - brush_size +1, 0);
                            int maximal_y = std::min(searched_y + brush_size -1, grid_num - 1);

                            for (int x = minimal_x; x <= maximal_x; x++) {
                                for (int y = minimal_y; y <= maximal_y; y++) {
                                    grid_list[x][y].default_color = paint_color;
                                    grid_list[x][y].substance = paint_substance;
                                }
                            }
                        }


                        //right click action on cell
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !simulate) {

                            int minimal_x = std::max(searched_x - brush_size + 1, 0);
                            int maximal_x = std::min(searched_x + brush_size - 1, grid_num - 1);

                            int minimal_y = std::max(searched_y - brush_size + 1, 0);
                            int maximal_y = std::min(searched_y + brush_size - 1, grid_num - 1);

                            for (int x = minimal_x; x <= maximal_x; x++) {
                                for (int y = minimal_y; y <= maximal_y; y++) {
                                    grid_list[x][y].default_color = GetBgColor();
                                    grid_list[x][y].substance = DEAD;
                                }
                            }
                            
                        }
                    }
                }

                
            }
        }

        //setting what color each button on keyboard does - deprecated:
        /*
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) paint_color = button_list[0].default_color;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) paint_color = button_list[1].default_color;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) paint_color = button_list[2].default_color;
        */


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
                    each.ChangePaintColor(paint_color, paint_substance);
                }
                else each.Recolor(each.default_color);

            }
        }

        //Save Button cliking
        save_button.Clicked(window, simulate, image, grid_num, grid_size, grid_list);

        //Load Button clicking
        load_button.Clicked(window, simulate, image, grid_num, grid_size, grid_list);

        //BRUSH CONTROLS
        //Plus Button clicking
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            increase_brush_button.Clicked(window, simulate, brush_size, brush_size_label.button_label, grid_num);
        }
        else increase_brush_button.Release();
        //Minus Button clicking
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            decrease_brush_button.Clicked(window, simulate, brush_size, brush_size_label.button_label);
        }
        else decrease_brush_button.Release();


        //TIME CONTROLS
        //Plus time Button clicking
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            increase_time_button.Clicked(window, simulate, time_speed, time_speed_label.button_label);
            sim_tpf = 21 - time_speed;
        }
        else increase_time_button.Release();
        //Minus time Button clicking
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            decrease_time_button.Clicked(window, simulate, time_speed, time_speed_label.button_label);
            sim_tpf = 21 - time_speed;
        }
        else decrease_time_button.Release();


        //Start simulation button
        if (simulate_button.square.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))) {
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

        //simulation loop only on some frames
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
        brush_size_label.DrawItself(window);
        increase_brush_button.DrawItself(window);
        decrease_brush_button.DrawItself(window);
        time_speed_label.DrawItself(window);
        increase_time_button.DrawItself(window);
        decrease_time_button.DrawItself(window);
        
        
        //update screen
        window.display();

        //add +1 to time and if it reaches desired tpf reset it to 0 (simulation frame)
        delta += 1;
        delta = delta % sim_tpf;

    }


    return 0;
}