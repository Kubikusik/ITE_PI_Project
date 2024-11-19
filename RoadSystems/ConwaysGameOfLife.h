#ifndef CONWAYSGAMEOFLIFE_H
#define CONWAYSGAMEOFLIFE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "UIElements.h"

#pragma once
void ConwaysGameOfLife(Grid_Tiles**& grid_list, int grid_num);
int CountNeighbors(int start_x, int start_y, int end_x, int end_y, int x, int y, Grid_Tiles**& grid_list, int grid_num, sf::Color searched_color);
void ConwaysGameOfLifeManyColor(Grid_Tiles**& grid_list, int grid_num, std::vector<sf::Color>& color_list);

#endif
