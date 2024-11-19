#include <iostream>
#include <SFML/Graphics.hpp>
#include "ConwaysGameOfLife.h"
#include "UIElements.h"
#include <time.h>
#include <cstdlib>

const sf::Color ALIVE_COLOR(255, 0, 0);
const sf::Color DEAD_COLOR(255, 255, 255);
const sf::Color SAND_COLOR(246, 215, 176);
const sf::Color PLANT_COLOR(53, 136, 86);
const sf::Color WATER_COLOR(142, 194, 228);
const sf::Color FIRE_COLOR(229, 81, 0);
const sf::Color STEAM_COLOR(111, 106, 96);



struct ColorMap {
    sf::Color color;
    int count;
};

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

void ConwaysPhysics(Grid_Tiles**& grid_list, int grid_num, std::vector<sf::Color>& color_list) {
    //int neighbor_counter;


    std::vector<ColorMap> color_map;
    
    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            grid_list[i][j].temp_color = grid_list[i][j].default_color;
        }
    }
    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            std::map<sf::Color, int> neighbor_map;
            for (sf::Color color: color_list) {
                ColorMap map;
                map.color = color;
                map.count = CountNeighbors(i - 1, j - 1, i + 1, j + 1, i, j, grid_list, grid_num, color);
                color_map.push_back(map);
            }


            //Sand
            //falling
            if (j + 1 < grid_num) {
                if (grid_list[i][j+1].default_color == DEAD_COLOR && grid_list[i][j].default_color == SAND_COLOR) {
                    grid_list[i][j+1].temp_color = grid_list[i][j].default_color;
                    grid_list[i][j].temp_color = DEAD_COLOR;
                }
            }

            //plant
            //die if too much plants around
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
            //Water
            int neighbor_count = 0;
            if (grid_list[i][j].default_color == WATER_COLOR) {
                sf::Color searched_color = WATER_COLOR;
                
                for (int k = 0; k < color_map.size(); k++) {
                    if (color_map[k].color == searched_color) {
                        neighbor_count = color_map[k].count;
                    }
                }
            }
            if (grid_list[i][j].default_color == WATER_COLOR) {
                if (j + 1 < grid_num) {
                    if (grid_list[i][j + 1].default_color == DEAD_COLOR) {
                        grid_list[i][j + 1].temp_color = WATER_COLOR;
                        grid_list[i][j].temp_color = DEAD_COLOR;
                    }
                    else if (i - 1 >= 0 && i + 1 < grid_num) {
                        if (grid_list[i - 1][j + 1].default_color == DEAD_COLOR && rand() % 2 && grid_list[i - 1][j].temp_color == DEAD_COLOR) {
                            grid_list[i - 1][j+1].temp_color = WATER_COLOR;
                            grid_list[i][j].temp_color = DEAD_COLOR;
                        }
                        else if (grid_list[i + 1][j + 1].default_color == DEAD_COLOR && rand() % 2 && grid_list[i + 1][j].temp_color == DEAD_COLOR) {
                            grid_list[i + 1][j + 1].temp_color = WATER_COLOR;
                            grid_list[i][j].temp_color = DEAD_COLOR;
                        }
                        else if (grid_list[i - 1][j].default_color == DEAD_COLOR && grid_list[i - 1][j].temp_color == DEAD_COLOR && rand()%2) {
                            
                            if (grid_list[i + 1][j].default_color == WATER_COLOR) {
                                grid_list[i - 1][j].temp_color = WATER_COLOR;
                                grid_list[i][j].temp_color = DEAD_COLOR;
                            }
                                
                        }
                        else if (grid_list[i + 1][j].default_color == DEAD_COLOR && grid_list[i + 1][j].temp_color == DEAD_COLOR) {
                            if (grid_list[i - 1][j].default_color == WATER_COLOR){
                                grid_list[i + 1][j].temp_color = WATER_COLOR;
                                grid_list[i][j].temp_color = DEAD_COLOR;
                            }

                            
                        }
                    }
                }
                
            }


            color_map.clear();
        }
    }

    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            grid_list[i][j].default_color = grid_list[i][j].temp_color;
        }
    }
}