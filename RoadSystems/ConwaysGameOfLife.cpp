#include <iostream>
#include <SFML/Graphics.hpp>
#include "ConwaysGameOfLife.h"
#include "UIElements.h"

const sf::Color ALIVE_COLOR(255, 0, 0);
const sf::Color DEAD_COLOR(255, 255, 255);

int CountNeighbors(int start_x, int start_y, int end_x, int end_y, int x, int y, Grid_Tiles**& grid_list, int grid_num, sf::Color searched_color) {
    int neigbor_counter = 0;

    // Clamp start and end values within the grid boundaries
    start_x = std::max(0, start_x);
    start_y = std::max(0, start_y);
    end_x = std::min(grid_num - 1, end_x);
    end_y = std::min(grid_num - 1, end_y);

    for (int k = start_x; k <= end_x; k++) {
        for (int l = start_y; l <= end_y; l++) {
            if (k == x && l == y) continue;
            if (grid_list[k][l].default_color == searched_color) {
                neigbor_counter += 1;
            }
        }
    }
    return neigbor_counter;
}

void ConwaysGameOfLife(Grid_Tiles**& grid_list, int grid_num) {
    int neighbor_counter;

    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            grid_list[i][j].temp_color = grid_list[i][j].default_color;
        }
    }
    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            neighbor_counter = CountNeighbors(i - 1, j - 1, i + 1, j + 1, i, j, grid_list, grid_num, ALIVE_COLOR);

            if (grid_list[i][j].default_color == DEAD_COLOR) {
                if (neighbor_counter > 2) {
                    grid_list[i][j].temp_color = DEAD_COLOR;
                }
                if (neighbor_counter == 3) {
                    grid_list[i][j].temp_color = ALIVE_COLOR;
                }
            }
            else {
                if (neighbor_counter == 2 || neighbor_counter == 3)
                {
                    grid_list[i][j].temp_color = ALIVE_COLOR;
                }
                else
                {
                    grid_list[i][j].temp_color = DEAD_COLOR;
                }
            }
        }
    }

    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            grid_list[i][j].default_color = grid_list[i][j].temp_color;
        }
    }
}

void ConwaysGameOfLifeManyColor(Grid_Tiles**& grid_list, int grid_num, std::vector<sf::Color>& color_list) {
    

    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            grid_list[i][j].temp_color = grid_list[i][j].default_color;
        }
    }
    
    for (auto each : color_list) {
        sf::Color alive = each;
        for (int i = 0; i < grid_num; i++) {
            for (int j = 0; j < grid_num; j++) {
                int neighbor_counter;
                neighbor_counter = CountNeighbors(i - 1, j - 1, i + 1, j + 1, i, j, grid_list, grid_num, alive);

                if (grid_list[i][j].default_color == alive) {
                    if (neighbor_counter == 2 || neighbor_counter == 3)
                    {
                        grid_list[i][j].temp_color = alive;
                    }
                    else
                    {
                        grid_list[i][j].temp_color = DEAD_COLOR;
                    }
                    
                }
                else {
                    if (neighbor_counter > 2) {
                        grid_list[i][j].temp_color = DEAD_COLOR;
                    }
                    if (neighbor_counter == 3) {
                        grid_list[i][j].temp_color = alive;
                    }
                }
            }
        }

    }

    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            grid_list[i][j].default_color = grid_list[i][j].temp_color;
        }
    }
}