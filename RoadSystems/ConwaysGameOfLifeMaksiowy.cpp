#include <iostream>
#include <SFML/Graphics.hpp>
#include "ConwaysGameOfLife.h"
#include "UIElements.h"
#include <time.h>
#include <cstdlib>


//constants:
const sf::Color ALIVE_COLOR(255, 0, 0);
const sf::Color DEAD_COLOR(255, 255, 255);
const sf::Color SAND_COLOR(246, 215, 176);
const sf::Color PLANT_COLOR(53, 136, 86);
const sf::Color WATER_COLOR(142, 194, 228);
const sf::Color FIRE_COLOR(229, 81, 0);
const sf::Color STEAM_COLOR(111, 106, 96);


//need for counting neighbors by color:
struct ColorMap {
    sf::Color color;
    int count;
};

//test idea of different rules than normal conway's game of life (chance based instead of deterministic) - DEPRECATED
void ConwaysGameOfLifeMaksiowy(Grid_Tiles**& grid_list, int grid_num) {
    int neighbor_counter;
    //srand(time(NULL));

    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            grid_list[i][j].temp_color = grid_list[i][j].default_color;
        }
    }
    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            neighbor_counter = CountNeighbors(i - 1, j - 1, i + 1, j + 1, i, j, grid_list, grid_num, ALIVE_COLOR);

            if ((rand() % 2) % 2) {
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
            else {
                if (grid_list[i][j].default_color == DEAD_COLOR) {
                    if (neighbor_counter > 2) {
                        grid_list[i][j].temp_color = DEAD_COLOR;
                    }
                    if (neighbor_counter == 3) {
                        grid_list[i][j].temp_color = ALIVE_COLOR;
                    }
                }
                else {
                    if (neighbor_counter == 3 || neighbor_counter == 4)
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
    }

    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            grid_list[i][j].default_color = grid_list[i][j].temp_color;
        }
    }
}