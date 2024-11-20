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


//simulation of physics using cellular automata
void ConwaysPhysics(Grid_Tiles**& grid_list, int grid_num, std::vector<sf::Color>& color_list) {
    srand(time(NULL));

    //color map of neighbors
    std::vector<ColorMap> color_map;
    

    //fill temp color with current colors of each cell (temp color is needed so u can update whole map at once at the end)
    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            grid_list[i][j].temp_color = grid_list[i][j].default_color;
        }
    }

    //for each cell do:
    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            std::map<sf::Color, int> neighbor_map;

            //for each color count neigbors of that color:
            for (sf::Color color: color_list) {
                ColorMap map;
                map.color = color;
                map.count = CountNeighbors(i - 1, j - 1, i + 1, j + 1, i, j, grid_list, grid_num, color);
                color_map.push_back(map);
            }


            //Sand:
            //falling
            if (j + 1 < grid_num) {
                if (grid_list[i][j+1].default_color == DEAD_COLOR && grid_list[i][j].default_color == SAND_COLOR) {
                    grid_list[i][j+1].temp_color = grid_list[i][j].default_color;
                    grid_list[i][j].temp_color = DEAD_COLOR;
                }
            }

            //Plant:
            //die if too much plants around

            //how many neighbors are also plant:
            if (grid_list[i][j].default_color == PLANT_COLOR) {
                sf::Color searched_color = PLANT_COLOR;
                int neighbor_count = 0;
                for (int k = 0; k < color_map.size(); k++) {
                    if (color_map[k].color == searched_color) {
                        neighbor_count = color_map[k].count;
                    }
                }
                if (neighbor_count >= 5) {
                    grid_list[i][j].temp_color = DEAD_COLOR;
                }
            }


            //grow if enough, but not too much plant around
            if (grid_list[i][j].default_color == DEAD_COLOR) {
                //how many neighbors are plants:
                sf::Color searched_color = PLANT_COLOR;
                int neighbor_count = 0;
                for (int k = 0; k < color_map.size(); k++) {
                    if (color_map[k].color == searched_color) {
                        neighbor_count = color_map[k].count;
                    }
                }

                if (neighbor_count >1 && neighbor_count <3 && grid_list[i][j].temp_color == DEAD_COLOR) {
                    grid_list[i][j].temp_color = PLANT_COLOR;
                }
            }

            //Water sim:
            if (grid_list[i][j].default_color == WATER_COLOR) {
                // Primary downward flow
                if (j + 1 < grid_num &&
                    grid_list[i][j + 1].default_color == DEAD_COLOR &&
                    grid_list[i][j + 1].temp_color != WATER_COLOR) {
                    grid_list[i][j + 1].temp_color = WATER_COLOR;
                    grid_list[i][j].temp_color = DEAD_COLOR;
                    grid_list[i][j + 1].isBounceRight = grid_list[i][j].isBounceRight;
                }
                // Diagonal flow if straight down is blocked
                else if (j + 1 < grid_num &&
                    ((i + 1 < grid_num &&
                        grid_list[i + 1][j + 1].default_color == DEAD_COLOR &&
                        grid_list[i + 1][j + 1].temp_color != WATER_COLOR) ||
                        (i - 1 >= 0 &&
                            grid_list[i - 1][j + 1].default_color == DEAD_COLOR &&
                            grid_list[i - 1][j + 1].temp_color != WATER_COLOR))) {
                    if (grid_list[i][j].isBounceRight &&
                        i + 1 < grid_num &&
                        grid_list[i + 1][j + 1].default_color == DEAD_COLOR &&
                        grid_list[i + 1][j + 1].temp_color != WATER_COLOR) {
                        grid_list[i + 1][j + 1].temp_color = WATER_COLOR;
                        grid_list[i][j].temp_color = DEAD_COLOR;
                        grid_list[i + 1][j + 1].isBounceRight = true;
                    }
                    else if (!grid_list[i][j].isBounceRight &&
                        i - 1 >= 0 &&
                        grid_list[i - 1][j + 1].default_color == DEAD_COLOR &&
                        grid_list[i - 1][j + 1].temp_color != WATER_COLOR) {
                        grid_list[i - 1][j + 1].temp_color = WATER_COLOR;
                        grid_list[i][j].temp_color = DEAD_COLOR;
                        grid_list[i - 1][j + 1].isBounceRight = false;
                    }
                }
                // Bouncing mechanics
                else {
                    if (grid_list[i][j].isBounceRight) {
                        // Try to move right
                        if (i + 1 < grid_num &&
                            grid_list[i + 1][j].default_color == DEAD_COLOR &&
                            grid_list[i + 1][j].temp_color != WATER_COLOR) {
                            grid_list[i + 1][j].temp_color = WATER_COLOR;
                            grid_list[i][j].temp_color = DEAD_COLOR;
                            grid_list[i + 1][j].isBounceRight = true;

                            // Change direction if hit right wall
                            if (i + 2 >= grid_num) {
                                grid_list[i + 1][j].isBounceRight = false;
                            }
                        }
                        // If can't move right, try left
                        else if (i - 1 >= 0 &&
                            grid_list[i - 1][j].default_color == DEAD_COLOR &&
                            grid_list[i - 1][j].temp_color != WATER_COLOR) {
                            grid_list[i - 1][j].temp_color = WATER_COLOR;
                            grid_list[i][j].temp_color = DEAD_COLOR;
                            grid_list[i - 1][j].isBounceRight = false;
                        }
                    }
                    else {
                        // Try to move left
                        if (i - 1 >= 0 &&
                            grid_list[i - 1][j].default_color == DEAD_COLOR &&
                            grid_list[i - 1][j].temp_color != WATER_COLOR) {
                            grid_list[i - 1][j].temp_color = WATER_COLOR;
                            grid_list[i][j].temp_color = DEAD_COLOR;
                            grid_list[i - 1][j].isBounceRight = false;

                            // Change direction if hit left wall
                            if (i - 2 < 0) {
                                grid_list[i - 1][j].isBounceRight = true;
                            }
                        }
                        // If can't move left, try right
                        else if (i + 1 < grid_num &&
                            grid_list[i + 1][j].default_color == DEAD_COLOR &&
                            grid_list[i + 1][j].temp_color != WATER_COLOR) {
                            grid_list[i + 1][j].temp_color = WATER_COLOR;
                            grid_list[i][j].temp_color = DEAD_COLOR;
                            grid_list[i + 1][j].isBounceRight = true;
                        }
                    }
                }
            }


            //clear color_map so next cell can use it again
            color_map.clear();
        }
    }


    //update all cells at the end of simulation frame to be displayed properly
    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            grid_list[i][j].default_color = grid_list[i][j].temp_color;
        }
    
    }
}