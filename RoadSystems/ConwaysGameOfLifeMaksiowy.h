#ifndef CONWAYSGAMEOFLIFEMAKSIOWY_H
#define CONWAYSGAMEOFLIFEMAKSIOWY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "UIElements.h"
#pragma once
void ConwaysGameOfLifeMaksiowy(Grid_Tiles**& grid_list, int grid_num);
void ConwaysPhysics(Grid_Tiles**& grid_list, int grid_num, std::vector<sf::Color>& color_list);
#endif
#pragma once
