#include "Load_Popup.h"
#include "EventHandler.h"
#include "Manager.h"
#include "UIElements.h"
#include "tinyxml2.h"
#include "windows.h"

LoadPopup::LoadPopup(sf::RenderWindow* window, sf::Font font, sf::Clock *clock, bool * NewFrame) {
    this->clock = clock;
    this->NewFrame = NewFrame;
    this->main_window = window;
    this->default_font = font;
    grid = new Grid_Tiles* [grid_num];
    for (int row = 0; row < grid_num; row++) {
        grid[row] = new Grid_Tiles[grid_num];
        for (int column = 0; column < grid_num; column++) {
            grid[row][column].square.setFillColor(GetBgColor()); //default color = bg_color
            grid[row][column].square.setPosition(sf::Vector2f(ui_size + row * (grid_size + 1), column * (grid_size + 1)));
            grid[row][column].square.setSize(sf::Vector2f(grid_size, grid_size));
            grid[row][column].x = row;
            grid[row][column].y = column;
        }
    }
    sf::Texture temp_texture;
    if (!temp_texture.loadFromFile("./Textures/UIButtons/SubstanceButtons/SubstanceTemp.png")) std::cout << "ERROR OF TEXTURE";
    else std::cout << "CORRECTO MUNDO";
    LoadBgInit();
}

bool LoadPopup::LoadPreset() {
    try {
        for (int i = 0; i < grid_num; i++) {
            for (int j = 0; j < grid_num; j++) {
                grid[i][j].default_color = GetBgColor();
                grid[i][j].substance = DEAD;
            }
        }

        tinyxml2::XMLElement* root = doc.FirstChildElement("bg"); // gets bg element
        if (root == nullptr) {
            throw std::runtime_error("Invalid XML file format.");
        }

        ChangeBackgroundColor(sf::Color(root->IntAttribute("r"), root->IntAttribute("g"), root->IntAttribute("b"))); // sets bg color
        UpdateGridBackground(grid, grid_num, DEAD_COLOR); // updates grid background color

        tinyxml2::XMLElement* cell = root->FirstChildElement("cell"); // gets first cell element
        while (cell != nullptr) { // while there are cells read and set their attributes
            int x = cell->IntAttribute("x");
            int y = cell->IntAttribute("y");
            int r = cell->IntAttribute("r");
            int g = cell->IntAttribute("g");
            int b = cell->IntAttribute("b");
            Substances s = (Substances)cell->IntAttribute("s");
            if (x < 0 || x >= grid_num || y < 0 || y >= grid_num) {
                throw std::runtime_error("Cell coordinates out of bounds.");
            }
			if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
				throw std::runtime_error("Invalid color values.");
			}
			if (s < 0 || s >= 10) {
				throw std::runtime_error("Invalid substance value.");
			}
            
            grid[x][y].default_color = sf::Color(r, g, b);
            grid[x][y].substance = s;
            cell = cell->NextSiblingElement("cell"); // go to next cell
        }
    }
    catch (const std::exception& e) {
        MessageBox(NULL, (L"Error while loading preview: " + std::wstring(e.what(), e.what() + strlen(e.what()))).c_str(), L"Error", MB_OK | MB_ICONERROR);
        this->isVisible = false;
        return false;
    }
    catch (...) {
        MessageBox(NULL, L"Unknown error while loading preview.", L"Error", MB_OK | MB_ICONERROR);
        this->isVisible = false;
        return false;
    }
    return true;
}


void LoadPopup::LoadBgInit() {
    background.setSize(sf::Vector2f(
        (this->main_window)->getSize().x / 1.3f,
        (this->main_window)->getSize().y
    ));
    background.setPosition(sf::Vector2f(
        (this->main_window->getSize().x / 2.0f - background.getSize().x / 2.0f),
        (this->main_window->getSize().y / 2.0f - background.getSize().y / 2.0f)
    ));

    background.setFillColor(sf::Color(255,255,255));

    grid_background.setFillColor(sf::Color(0, 0, 0));

    button_background.setFillColor(sf::Color(50, 50, 50));

    // Adjust grid_background size to be slightly smaller
    this->grid_background.setSize(sf::Vector2f(
        background.getSize().x - 2 * margin,
        (background.getSize().y - 2 * margin) / 1.25f
    ));

    // Align grid_background inside the background with margin
    grid_background.setPosition(
        background.getPosition().x + margin,
        background.getPosition().y + margin
    );

    this->button_background.setSize(sf::Vector2f(
        this->grid_background.getSize().x,
        background.getSize().y - this->grid_background.getSize().y - 3 * margin
    ));

    button_background.setPosition(
        grid_background.getPosition().x,
        grid_background.getPosition().y + grid_background.getSize().y + margin
    );
}

void LoadPopup::LoadDraw(Grid_Tiles** main_grid) {
    (this->main_window)->draw(background);
    const float grid_width = grid_background.getSize().x - 1.5f * margin;
    const float grid_height = grid_background.getSize().y - 1.5f * margin;

    float offset_x = margin;
    float offset_y = margin;

    // Draw grid_background
    (this->main_window)->draw(grid_background);
    (this->main_window)->draw(button_background);

    // Calculate new grid size to fit inside grid_background
    float new_grid_size = std::min((grid_width - (grid_num - 1)) / grid_num, (grid_height - (grid_num - 1)) / grid_num) - 2.0f;

    // Calculate the offset to center the grid inside grid_background
    offset_x += (grid_background.getSize().x - (new_grid_size * grid_num + (grid_num - 1))) / 2.0f;

    // Draw the grid inside grid_background with the new size and offset
    for (int row = 0; row < grid_num; row++) {
        for (int column = 0; column < grid_num; column++) {
            grid[row][column].square.setFillColor(grid[row][column].default_color); // default color = bg_color
            grid[row][column].square.setPosition(sf::Vector2f(
                grid_background.getPosition().x + offset_x + row * (new_grid_size + 1),
                grid_background.getPosition().y + offset_y + column * (new_grid_size + 1)
            ));
            grid[row][column].square.setSize(sf::Vector2f(new_grid_size, new_grid_size));
            grid[row][column].x = row;
            grid[row][column].y = column;
            (this->main_window)->draw(grid[row][column].square);
        }
    }

    Confirm_Button* confirm_button = new Confirm_Button(
        button_background.getPosition().x + margin,
        button_background.getPosition().y + margin,
        button_background.getSize().y / 2.5f,
        "confirm_b",
        "Confirm",
        default_font,
        sf::Color(0, 255, 0),
        temp_texture
    );

	confirm_button->Recolor(confirm_button->default_color);

    Cancel_Button* cancel_button = new Cancel_Button(
        button_background.getPosition().x + (button_background.getSize().x / 2) - margin,
        button_background.getPosition().y + margin,
        button_background.getSize().y / 2.5f,
        "cancel_b",
        "Cancel",
        default_font,
        sf::Color(255, 0, 0),
        temp_texture
    );

	cancel_button->Recolor(cancel_button->default_color);
    confirm_button->DrawItself(*(this->main_window));
    cancel_button->DrawItself(*(this->main_window));

    // Check for left mouse button click
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(*(this->main_window));
        sf::FloatRect confirm_bounds = confirm_button->getBounds();
        sf::FloatRect cancel_bounds = cancel_button->getBounds();

        if (confirm_bounds.contains(static_cast<sf::Vector2f>(mouse_pos))) {
            *NewFrame = false;
            clock->restart();
            this->first_time = true;
            confirm_button->Clicked(*(this->main_window), grid_num, main_grid, doc, &(this->isVisible));
            
        } else if (cancel_bounds.contains(static_cast<sf::Vector2f>(mouse_pos))) {
            *NewFrame = false;
            clock->restart();
            this->first_time = true;
            cancel_button->Clicked(&(this->isVisible));
        }
    }

    UpdateGridBackground(this->grid, grid_num, DEAD_COLOR);
}