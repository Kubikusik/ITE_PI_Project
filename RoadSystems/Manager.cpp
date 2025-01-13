#include "Manager.h"
#include "Menu_Popup.h"

Manager::Manager() {
    ui_buttons = std::make_unique<UIButtonSet>();  // Allocates memory for the UI buttons set
    srand(time(NULL));
    default_font.loadFromFile("OpenSansRegular.ttf");
    ChangeBackgroundColor(bg_color);
    paint_color = SAND_COLOR;
    window.create(sf::VideoMode(map_x + ui_size, map_y), "Conways Simulation", sf::Style::Default);
    window.setFramerateLimit(60);
    image.create(grid_num * (grid_size + 1), grid_num * (grid_size + 1), GetBgColor());
    menu_popup = new MenuPopup(&window, default_font);
    CreateGrid();
    CreateButtons(button_list, color_list);  // Dereference here to pass to the function
}

Manager::~Manager() {
    for (int i = 0; i < grid_num; ++i) {
        delete[] grid_list[i];
    }
    delete[] grid_list;
}

void Manager::CreateGrid() {
    grid_list = new Grid_Tiles * [grid_num]; //making a 2d array of objects of type Grid_Tiles (from "UIElements.h")
    for (int row = 0; row < grid_num; row++) {
        grid_list[row] = new Grid_Tiles[grid_num];
        for (int column = 0; column < grid_num; column++) {
            grid_list[row][column].square.setFillColor(GetBgColor()); //default color = bg_color
            grid_list[row][column].square.setPosition(sf::Vector2f(ui_size + row * (grid_size + 1), column * (grid_size + 1)));
            grid_list[row][column].square.setSize(sf::Vector2f(grid_size, grid_size));
            grid_list[row][column].x = row;
            grid_list[row][column].y = column;
        }
    }
}

//Function for making all the color buttons (all substances)
void Manager::CreateButtons(std::vector<Paint_Button>& color_button_list, std::vector<sf::Color>& color_list){
    int i = 0;

    // Sand Button
    ui_buttons->sand_button = std::make_unique<Paint_Button>(
        0, i * (button_size + margin), button_size, "yellow_b", "Sand", default_font,
        sf::Color(246, 215, 176), SAND
    );
    color_button_list.push_back(*ui_buttons->sand_button);
    color_list.push_back(sf::Color(246, 215, 176));
    i++;

    // Plant Button
    ui_buttons->plant_button = std::make_unique<Paint_Button>(
        0, i * (button_size + margin), button_size, "green_b", "Plant", default_font,
        sf::Color(53, 136, 86), PLANT
    );
    color_button_list.push_back(*ui_buttons->plant_button);
    color_list.push_back(sf::Color(53, 136, 86));
    i++;

    // Water Button
    ui_buttons->water_button = std::make_unique<Paint_Button>(
        0, i * (button_size + margin), button_size, "water_b", "Water", default_font,
        sf::Color(142, 194, 228), WATER
    );
    color_button_list.push_back(*ui_buttons->water_button);
    color_list.push_back(sf::Color(142, 194, 228));
    i++;

    // Fire Button
    ui_buttons->fire_button = std::make_unique<Paint_Button>(
        0, i * (button_size + margin), button_size, "red_b", "Fire", default_font,
        sf::Color(229, 81, 0), FIRE
    );
    color_button_list.push_back(*ui_buttons->fire_button);
    color_list.push_back(sf::Color(229, 81, 0));
    i++;

    // Steel Button
    ui_buttons->steel_button = std::make_unique<Paint_Button>(
        0, i * (button_size + margin), button_size, "steel_b", "Steel", default_font,
        sf::Color(111, 106, 96), STEEL
    );
    color_button_list.push_back(*ui_buttons->steel_button);
    color_list.push_back(sf::Color(111, 106, 96));
    i++;

    // Conway Button
    ui_buttons->conway_button = std::make_unique<Paint_Button>(
        0, i * (button_size + margin), button_size, "conway_b", "Nanomachine", default_font,
        sf::Color(5, 15, 45), CONWAY
    );
    color_button_list.push_back(*ui_buttons->conway_button);
    color_list.push_back(sf::Color(5, 15, 45));
    i++;

    // Cable Button
    ui_buttons->cable_button = std::make_unique<Paint_Button>(
        0, i * (button_size + margin), button_size, "cable_b", "Cable", default_font,
        sf::Color(45, 15, 45), CABLE
    );
    color_button_list.push_back(*ui_buttons->cable_button);
    color_list.push_back(sf::Color(45, 15, 45));
    i++;

    // Save Button
    ui_buttons->save_button = std::make_unique<Save_Button>(
        0, i * (button_size + margin), button_size, "save_b", "Save", default_font,
        sf::Color(50, 50, 50)
    );
    i++;

    // Load Button
    ui_buttons->load_button = std::make_unique<Load_Button>(
        0, i * (button_size + margin), button_size, "load_b", "Load", default_font,
        sf::Color(50, 50, 50)
    );
    i++;

    // Simulate Button
    ui_buttons->simulate_button = std::make_unique<Simulate_Button>(
        0, i * (button_size + margin), button_size, "sim_b", "Run", default_font,
        sf::Color(255, 0, 0)
    );
    i++;

    // Brush Size Buttons
    ui_buttons->decrease_brush_button = std::make_unique<Minus_Button>(
        0, i * (button_size + margin), button_size, "minus_b", "-", default_font,
        sf::Color(255, 255, 255, 10)
    );
    ui_buttons->increase_brush_button = std::make_unique<Plus_Button>(
        5 * button_size, i * (button_size + margin), button_size, "plus_b", "+", default_font,
        sf::Color(255, 255, 255, 10)
    );
    ui_buttons->brush_size_label = std::make_unique<Labeled_Button>(
        -margin / 2, i * (button_size + margin), button_size, "brush_counter",
        "brush size:" + std::to_string(brush_size), default_font, sf::Color(0, 0, 0, 0)
    );
    i++;

    // Time Speed Buttons
    ui_buttons->increase_time_button = std::make_unique<Minus_Time_Button>(
        0, i * (button_size + margin), button_size, "minus_t_b", "-", default_font,
        sf::Color(255, 255, 255, 10)
    );
    ui_buttons->decrease_time_button = std::make_unique<Plus_Time_Button>(
        5 * button_size, i * (button_size + margin), button_size, "plus_t_b", "+", default_font,
        sf::Color(255, 255, 255, 10)
    );
    ui_buttons->time_speed_label = std::make_unique<Labeled_Button>(
        -margin / 2, i * (button_size + margin), button_size, "time_counter",
        "time speed:" + std::to_string(sim_tpf), default_font, sf::Color(0, 0, 0, 0)
    );
}