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
    menu_popup->music = &music;
    menu_popup->grid_sound = &grid_sound;
    //load_menu = new LoadMenu(ble);
    CreateGrid();
    CreateButtons(button_list, color_list);  // Dereference here to pass to the function
    LoadSounds();
    
}

void Manager::LoadSounds() {
    
    if (!music.openFromFile("./Sounds/Music/music.mp3")) std::cout << "Music error";
    else std::cout << "Music good";
    music.play();
    music.setLoop(true);
    music.setVolume(0.f);

    if (button_sound[0].loadFromFile("./Sounds/Button Sounds/click-button-166324.mp3")) std::cout << "Loaded button sound 1";
    if (button_sound[1].loadFromFile("./Sounds/Button Sounds/button-202966.mp3")) std::cout << "Loaded button sound 2";
    if (button_sound[2].loadFromFile("./Sounds/Button Sounds/old-radio-button-click-97549.mp3")) std::cout << "Loaded button sound 3";
    if (button_sound[3].loadFromFile("./Sounds/Button Sounds/walkman_button.mp3")) std::cout << "Loaded button sound 3";
    if (grid_sound_buff.loadFromFile("./Sounds/Grid Sounds/plop_grid.mp3")) std::cout << "Loaded grid sound 1";
    grid_sound.setBuffer(grid_sound_buff);
    //sound.play();
    grid_sound.setVolume(100.f);
    
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
    sf::Texture temp_texture;
    sf::Color WHITE(255, 255, 255);

    SideUI.setPosition(sf::Vector2f(0, 0));
    SideUI.setScale(sf::Vector2f(150, 420));
    if (!SideTexture.loadFromFile("./Textures/SideBg/SideBg.png")) std::cout << "Error loading side menu";
    else std::cout << "Loaded Side Correctly";
    SideUI.setFillColor(sf::Color(255, 255, 255));
    SideUI.setTexture(&SideTexture);
    

    // Sand Button
    if (!temp_texture.loadFromFile("./Textures/UIButtons/SubstanceButtons/SubstanceTemp.png")) std::cout << "ERROR OF TEXTURE";
    else std::cout << "CORRECTO MUNDO";

    ui_buttons->sand_button = std::make_unique<Paint_Button>(
        0, i * (button_size + margin), button_size, "yellow_b", "Sand", default_font,
        WHITE, SAND, temp_texture
    );
    color_button_list.push_back(*ui_buttons->sand_button);
    color_list.push_back(sf::Color(246, 215, 176));
    i++;

    // Plant Button
    ui_buttons->plant_button = std::make_unique<Paint_Button>(
        0, i * (button_size + margin), button_size, "green_b", "Plant", default_font,
        WHITE, PLANT, temp_texture
    );
    color_button_list.push_back(*ui_buttons->plant_button);
    color_list.push_back(sf::Color(53, 136, 86));
    i++;

    // Water Button
    ui_buttons->water_button = std::make_unique<Paint_Button>(
        0, i * (button_size + margin), button_size, "water_b", "Water", default_font,
        WHITE, WATER, temp_texture
    );
    color_button_list.push_back(*ui_buttons->water_button);
    color_list.push_back(sf::Color(142, 194, 228));
    i++;

    // Fire Button
    ui_buttons->fire_button = std::make_unique<Paint_Button>(
        0, i * (button_size + margin), button_size, "red_b", "Fire", default_font,
        WHITE, FIRE, temp_texture
    );
    color_button_list.push_back(*ui_buttons->fire_button);
    color_list.push_back(sf::Color(229, 81, 0));
    i++;

    // Steel Button
    ui_buttons->steel_button = std::make_unique<Paint_Button>(
        0, i * (button_size + margin), button_size, "steel_b", "Steel", default_font,
        WHITE, STEEL, temp_texture
    );
    color_button_list.push_back(*ui_buttons->steel_button);
    color_list.push_back(sf::Color(111, 106, 96));
    i++;

    // Conway Button
    ui_buttons->conway_button = std::make_unique<Paint_Button>(
        0, i * (button_size + margin), button_size, "conway_b", "Nanomachine", default_font,
        WHITE, CONWAY, temp_texture
    );
    color_button_list.push_back(*ui_buttons->conway_button);
    color_list.push_back(sf::Color(5, 15, 45));
    i++;

    // Cable Button
    ui_buttons->cable_button = std::make_unique<Paint_Button>(
        0, i * (button_size + margin), button_size, "cable_b", "Cable", default_font,
        WHITE, CABLE, temp_texture
    );
    color_button_list.push_back(*ui_buttons->cable_button);
    color_list.push_back(sf::Color(45, 15, 45));
    i++;

    // Save Button
    ui_buttons->save_button = std::make_unique<Save_Button>(
        0, i * (button_size + margin), button_size, "save_b", "Save", default_font,
        WHITE, temp_texture
    );
    i++;

    // Load Button
    ui_buttons->load_button = std::make_unique<Load_Button>(
        0, i * (button_size + margin), button_size, "load_b", "Load", default_font,
        WHITE, temp_texture
    );
    i++;

    if (!temp_texture.loadFromFile("./Textures/UIButtons/SimButton/SimButtonOff.png")) std::cout << "ERROR OF TEXTURE";
    else std::cout << "CORRECTO MUNDO";

    // Simulate Button
    ui_buttons->simulate_button = std::make_unique<Simulate_Button>(
        0, i * (button_size + margin), button_size, "sim_b", "Run", default_font,
        WHITE, temp_texture
    );
    if (!temp_texture.loadFromFile("./Textures/UIButtons/SimButton/SimButtonOn.png")) std::cout << "ERROR OF TEXTURE";
    else std::cout << "CORRECTO MUNDO";
    ui_buttons->simulate_button->on_texture = temp_texture;
    i++;

    // Brush Size Buttons
    ui_buttons->decrease_brush_button = std::make_unique<Minus_Button>(
        0, i * (button_size + margin), button_size, "minus_b", "-", default_font,
        WHITE, temp_texture
    );
    ui_buttons->increase_brush_button = std::make_unique<Plus_Button>(
        5 * button_size, i * (button_size + margin), button_size, "plus_b", "+", default_font,
        sf::Color(255, 255, 255, 10), temp_texture
    );
    ui_buttons->brush_size_label = std::make_unique<Labeled_Button>(
        -margin / 2, i * (button_size + margin), button_size, "brush_counter",
        "brush size:" + std::to_string(brush_size), default_font, sf::Color(0, 0, 0, 0), temp_texture
    );
    i++;

    // Time Speed Buttons
    ui_buttons->increase_time_button = std::make_unique<Minus_Time_Button>(
        0, i * (button_size + margin), button_size, "minus_t_b", "-", default_font,
        sf::Color(255, 255, 255, 10), temp_texture
    );
    ui_buttons->decrease_time_button = std::make_unique<Plus_Time_Button>(
        5 * button_size, i * (button_size + margin), button_size, "plus_t_b", "+", default_font,
        sf::Color(255, 255, 255, 10), temp_texture
    );
    ui_buttons->time_speed_label = std::make_unique<Labeled_Button>(
        -margin / 2, i * (button_size + margin), button_size, "time_counter",
        "time speed:" + std::to_string(sim_tpf), default_font, sf::Color(0, 0, 0, 0), temp_texture
    );
}