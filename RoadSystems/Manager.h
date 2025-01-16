#ifndef MANAGER_H
#define MANAGER_H

#include <SFML/Graphics.hpp> // SFML for rendering
#include <vector>
#include "UIElements.h"      // Custom UI elements
#include "Menu_Popup.h"      // MenuPopup base class
#include <memory>
#include "SFML/Audio.hpp"
#include <queue>


struct UIButtonSet {
    std::unique_ptr<Paint_Button> sand_button;
    std::unique_ptr<Paint_Button> plant_button;
    std::unique_ptr<Paint_Button> water_button;
    std::unique_ptr<Paint_Button> fire_button;
    std::unique_ptr<Paint_Button> steel_button;
    std::unique_ptr<Paint_Button> conway_button;
    std::unique_ptr<Paint_Button> cable_button;
    std::unique_ptr<Save_Button> save_button;
    std::unique_ptr<Load_Button> load_button;
    std::unique_ptr<Simulate_Button> simulate_button;
    std::unique_ptr<Minus_Button> decrease_brush_button;
    std::unique_ptr<Plus_Button> increase_brush_button;
    std::unique_ptr<Labeled_Button> brush_size_label;
    std::unique_ptr<Minus_Time_Button> increase_time_button;
    std::unique_ptr<Plus_Time_Button> decrease_time_button;
    std::unique_ptr<Labeled_Button> time_speed_label;

    // Default constructor
    UIButtonSet() {
        sand_button = nullptr;
        plant_button = nullptr;
        water_button = nullptr;
        fire_button = nullptr;
        steel_button = nullptr;
        conway_button = nullptr;
        cable_button = nullptr;
        save_button = nullptr;
        load_button = nullptr;
        simulate_button = nullptr;
        decrease_brush_button = nullptr;
        increase_brush_button = nullptr;
        brush_size_label = nullptr;
        increase_time_button = nullptr;
        decrease_time_button = nullptr;
        time_speed_label = nullptr;
    }
};

class Manager {
public:
    // Members
    int time_speed = 5;
    int sim_tpf = 5;
    sf::Font default_font;
    int delta = 0;
    int brush_size = 1;
    sf::Color bg_color = sf::Color(250, 250, 250);
    sf::Color paint_color;
    bool is_focused = true;
    bool simulate = false;
    Substances paint_substance = SAND;
    sf::RenderWindow window;
    std::vector<sf::Color> color_list;
    std::vector<Paint_Button> button_list;
    sf::Image image;
    Grid_Tiles** grid_list = nullptr; // Pointer to grid tiles
    MenuPopup *menu_popup;

    //Sounds
    sf::Music music;
    sf::Sound grid_sound;
    sf::SoundBuffer button_sound[3];
    sf::SoundBuffer grid_sound_buff;

    //Side background
    std::unique_ptr<UIButtonSet> ui_buttons;
    sf::Texture SideTexture;
    sf::RectangleShape SideUI;

    // Constructor and destructor
    Manager();
    ~Manager();

    // Methods
    void CreateGrid();
    void CreateButtons(std::vector<Paint_Button>& color_button_list, std::vector<sf::Color>& color_list);
    void LoadSounds();

    
};

#endif // MANAGER_H