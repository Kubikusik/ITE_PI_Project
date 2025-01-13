#include <iostream>
#include <SFML/Graphics.hpp>
#include "ConwaysGameOfLife.h"
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include <random>


//constants:
const int gravity_const = -1;
const int water_spread_const = 1;
const int max_velocity = 10;



//test idea of different rules than normal conway's game of life (chance based instead of deterministic) - DEPRECATED
/*
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
*/


std::vector<Grid_Tiles> UpdateMap;

bool IsViableLocation(Grid_Tiles**& grid_list, int grid_num, int i, int j, Substances substance) {
    if (i>=0 && i<grid_num && j>=0 && j<grid_num &&
        grid_list[i][j].next_substance == substance && !grid_list[i][j].isUpdated) {
        return true;
    }
    else {
        return false;
    }
}

void MoveTo(Grid_Tiles**& grid_list, int i, int j, int target_i, int target_j) {
    
    grid_list[i][j].next_substance = grid_list[target_i][target_j].substance;
    grid_list[target_i][target_j].next_substance = grid_list[i][j].substance;
    grid_list[target_i][target_j].way = grid_list[i][j].way;

    grid_list[target_i][target_j].isUpdated = true;
    UpdateMap.push_back(grid_list[target_i][target_j]);
    grid_list[i][j].isUpdated = true;
    UpdateMap.push_back(grid_list[i][j]);
}

void SandUpdate(Grid_Tiles**& grid_list, int grid_num, int i, int j) {

    //falling down:
    if (IsViableLocation(grid_list, grid_num, i, j+1, DEAD)) {
        MoveTo(grid_list, i, j, i, j + 1);
    }

    //falling left:
    else if (IsViableLocation(grid_list, grid_num, i-1, j + 1, DEAD)) {
        MoveTo(grid_list, i, j, i - 1, j + 1);
    }
    //falling right:
    else if (IsViableLocation(grid_list, grid_num, i + 1, j + 1, DEAD)) {
        MoveTo(grid_list, i, j, i + 1, j + 1);
    }
    //falling under water:
    else if (IsViableLocation(grid_list, grid_num, i, j + 1, WATER)) {
        MoveTo(grid_list, i, j, i, j + 1);
    }
    //falling left under water:
    else if (IsViableLocation(grid_list, grid_num, i -1, j + 1, WATER)) {
        MoveTo(grid_list, i, j, i - 1, j + 1);
    }
    //falling right under water:
    else if (IsViableLocation(grid_list, grid_num, i + 1, j + 1, WATER)) {
        MoveTo(grid_list, i, j, i + 1, j + 1);
    }
}

void WaterUpdate(Grid_Tiles**& grid_list, int grid_num, int i, int j) {
    if (IsViableLocation(grid_list, grid_num, i, j+1, DEAD)) {
        MoveTo(grid_list, i, j, i, j + 1);
    }
    else if (IsViableLocation(grid_list, grid_num, i - grid_list[i][j].way, j + 1, DEAD))
        MoveTo(grid_list,i, j, i - grid_list[i][j].way, j + 1);

    else if (IsViableLocation(grid_list, grid_num, i + grid_list[i][j].way, j + 1, DEAD))
        MoveTo(grid_list, i, j, i + grid_list[i][j].way, j + 1);

    else if (IsViableLocation(grid_list, grid_num, i + grid_list[i][j].way, j, DEAD))
        MoveTo(grid_list, i, j , i + grid_list[i][j].way, j);

    else if (IsViableLocation(grid_list, grid_num, i + grid_list[i][j].way, j, WATER)) {
        grid_list[i][j].way = -grid_list[i][j].way;
    }

    else if (!IsViableLocation(grid_list, grid_num, i + grid_list[i][j].way, j, DEAD)) {
        grid_list[i][j].way = -grid_list[i][j].way;
    }
        
}

void SteamUpdate(Grid_Tiles**& grid_list, int grid_num, int i, int j) {
    if (IsViableLocation(grid_list, grid_num, i, j - 1, DEAD)) {
        MoveTo(grid_list, i, j, i, j - 1);
    }
    else if (IsViableLocation(grid_list, grid_num, i - grid_list[i][j].way, j - 1, DEAD))
        MoveTo(grid_list, i, j, i - grid_list[i][j].way, j - 1);

    else if (IsViableLocation(grid_list, grid_num, i + grid_list[i][j].way, j - 1, DEAD))
        MoveTo(grid_list, i, j, i + grid_list[i][j].way, j - 1);

    else if (IsViableLocation(grid_list, grid_num, i + grid_list[i][j].way, j, DEAD))
        MoveTo(grid_list, i, j, i + grid_list[i][j].way, j);

    else if (IsViableLocation(grid_list, grid_num, i + grid_list[i][j].way, j, WATER)) {
        grid_list[i][j].way = -grid_list[i][j].way;
    }

    else if (!IsViableLocation(grid_list, grid_num, i + grid_list[i][j].way, j, DEAD)) {
        grid_list[i][j].way = -grid_list[i][j].way;
    }


    if (j==0 && rand() % 100 == 0) {
        grid_list[i][j].next_substance = DEAD;
        grid_list[i][j].isUpdated = true;
        UpdateMap.push_back(grid_list[i][j]);
    }
}

void PlantUpdate(Grid_Tiles**& grid_list, int grid_num, int i, int j) {

    int growth_rate = 3;
    bool is_grown = false;

    if (IsViableLocation(grid_list, grid_num, i, j - 1, DEAD)
        && IsViableLocation(grid_list, grid_num, i - 1, j - 1, DEAD)
        && IsViableLocation(grid_list, grid_num, i + 1, j - 1, DEAD)
        && grid_list[i][j].growing) {

        //up:
        if (!is_grown) {
            for (int k = 1; k < growth_rate; k++) {
                if (IsViableLocation(grid_list, grid_num, i, j - k, DEAD) && rand() % 5 == 0) {
                    grid_list[i][j - k].next_substance = PLANT;
                    grid_list[i][j - k].isUpdated = true;
                    UpdateMap.push_back(grid_list[i][j-k]);
                    grid_list[i][j].isUpdated = true;
                    UpdateMap.push_back(grid_list[i][j]);
                    is_grown = true;
                }
                else break;
            }
        }

        //left-up:
        if (!is_grown || rand() % 2 == 0) {
            for (int k = 1; k < growth_rate; k++) {
                if (IsViableLocation(grid_list, grid_num, i - k, j - k, DEAD) && rand() % 5 == 0) {
                    grid_list[i - k][j - k].next_substance = PLANT;
                    grid_list[i - k][j - k].isUpdated = true;
                    UpdateMap.push_back(grid_list[i-k][j-k]);
                    grid_list[i][j].isUpdated = true;
                    UpdateMap.push_back(grid_list[i][j]);
                    is_grown = true;
                }
                else break;
            }
        }

        //right-up:
        if (!is_grown || rand() % 2 == 0) {
            for (int k = 1; k < growth_rate; k++) {
                if (IsViableLocation(grid_list, grid_num, i + k, j - k, DEAD) && rand() % 5 == 0) {
                    grid_list[i + k][j - k].next_substance = PLANT;
                    grid_list[i + k][j - k].isUpdated = true;
                    UpdateMap.push_back(grid_list[i+k][j-k]);
                    grid_list[i][j].isUpdated = true;
                    UpdateMap.push_back(grid_list[i][j]);
                    is_grown = true;
                }
                else break;
            }
        }
    }
    if ((IsViableLocation(grid_list, grid_num, i - 1, j, DEAD)
        || IsViableLocation(grid_list, grid_num, i + 1, j, DEAD))
        && grid_list[i][j].growing) {

        int empty_neigbors = 0;

        for (int x_temp = i - 1; x_temp <= i + 1; x_temp++) {
            for (int y_temp = j - 1; y_temp <= j + 1; y_temp++) {
                if (IsViableLocation(grid_list, grid_num, x_temp, y_temp, DEAD))
                    empty_neigbors++;
            }

        }
        
        //left:
        if (!is_grown && rand()%20 == 0 && empty_neigbors>=6) {
            for (int k = 1; k < growth_rate; k++) {
                if (IsViableLocation(grid_list, grid_num, i - k, j, DEAD)) {
                    grid_list[i - k][j].next_substance = PLANT;
                    grid_list[i - k][j].isUpdated = true;
                    UpdateMap.push_back(grid_list[i-k][j]);
                    grid_list[i][j].isUpdated = true;
                    UpdateMap.push_back(grid_list[i][j]);
                    is_grown = true;
                }
                else break;
            }
        }

        //right:
        if (!is_grown && rand() % 20 == 0 && empty_neigbors >= 6) {
            for (int k = 1; k < growth_rate; k++) {
                if (IsViableLocation(grid_list, grid_num, i + k, j, DEAD) ) {
                    grid_list[i + k][j].next_substance = PLANT;
                    grid_list[i + k][j].isUpdated = true;
                    UpdateMap.push_back(grid_list[i+k][j]);
                    grid_list[i][j].isUpdated = true;
                    UpdateMap.push_back(grid_list[i][j]);
                    is_grown = true;
                }
                else break;
            }
        }
    }

    /*if (grid_list[i][j].growing = false) {
        for (int k = -1; k <= 1; k++) {
            for (int l = -1; l <= 1; l++) {
                if (IsViableLocation(grid_list, grid_num, i + k, j + l, WATER)) {
                    grid_list[i + k][j + l].next_substance = DEAD;
                    grid_list[i + k][j + l].isUpdated = true;
                    UpdateMap.push_back(grid_list[i + k][j + l]);

                    grid_list[i][j].growing = true;
                    grid_list[i][j].isUpdated = true;
                    UpdateMap.push_back(grid_list[i][j]);
                }
            }
        }
    }
    else if (grid_list[i][j].growing = true) {
        int water = 0;
        for (int k = -1; k <= 1; k++) {
            for (int l = -1; l <= 1; l++) {
                if (IsViableLocation(grid_list, grid_num, i + k, j + l, WATER)) {
                    water++;
                }
            }
        }
        if (water == 0) {
            grid_list[i][j].growing = false;
        }
    }*/
    
}

void FireUpdate(Grid_Tiles**& grid_list, int grid_num, int i, int j) {
    //falling down:
    if (IsViableLocation(grid_list, grid_num, i, j + 1, DEAD) && rand()%4) {
        MoveTo(grid_list, i, j, i, j + 1);
    }
    //falling left:
    else if (IsViableLocation(grid_list, grid_num, i - 1, j + 1, DEAD) && rand()%2) {
        MoveTo(grid_list, i, j, i - 1, j + 1);
    }
    //falling right:
    else if (IsViableLocation(grid_list, grid_num, i + 1, j + 1, DEAD)) {
        MoveTo(grid_list, i, j, i + 1, j + 1);
    }
    
    for (int k = -1; k <= 1; k++) {
        for (int l = -1; l <= 1; l++) {
            if (IsViableLocation(grid_list, grid_num, i + k, j + l, PLANT) && !(rand()%3==0)) {
                grid_list[i + k][j + l].next_substance = FIRE;
                grid_list[i + k][j + l].isUpdated = true;
                UpdateMap.push_back(grid_list[i + k][j + l]);
            }
        }
    }

    if (rand() % 10 == 0) {
        grid_list[i][j].next_substance = DEAD;
        grid_list[i][j].isUpdated = true;
        UpdateMap.push_back(grid_list[i][j]);
    }

    for (int k = -1; k <= 1; k++) {
        for (int l = -1; l <= 1; l++) {
            if (IsViableLocation(grid_list, grid_num, i + k, j + l, WATER)) {
                grid_list[i][j].next_substance = STEAM;
                grid_list[i][j].isUpdated = true;
                UpdateMap.push_back(grid_list[i][j]);
            }
        }
    }
}

void ConwayUpdate(Grid_Tiles**& grid_list, int grid_num, int i, int j) {
    int conway_neigbors = 0;
    for (int k = -1; k <= 1; k++) {
        for (int l = -1; l <= 1; l++) {
            if (IsViableLocation(grid_list, grid_num, i + k, j + l, CONWAY)) {
                conway_neigbors++;
            }
        }
    }

    if (grid_list[i][j].substance == CONWAY) {
        if (conway_neigbors < 2 || conway_neigbors>3) {
            grid_list[i][j].next_substance = DEAD;
            //grid_list[i][j].isUpdated = true;
            UpdateMap.push_back(grid_list[i][j]);
        }
    }

    if (grid_list[i][j].substance == DEAD) {
        if (conway_neigbors ==3) {
            grid_list[i][j].next_substance = CONWAY;
            //grid_list[i][j].isUpdated = true;
            UpdateMap.push_back(grid_list[i][j]);
        }
    }

}


void CableUpdate(Grid_Tiles**& grid_list, int grid_num, int i, int j) {
    std::vector<std::pair<int, int>> directions = { {1,0}, {-1,0},{0,1}, {0,-1} };
    //auto rng = std::default_random_engine{};
    //std::shuffle(std::begin(directions), std::end(directions), rng);

    for (auto dir : directions) {
        
        if (IsViableLocation(grid_list, grid_num, i + dir.first, j + dir.second, FIRE)) {
            grid_list[i][j].next_substance = POWER;
            grid_list[i][j].isUpdated = true;
            UpdateMap.push_back(grid_list[i][j]);

            grid_list[i + dir.first][j + dir.second].next_substance = DEAD;
            grid_list[i + dir.first][j + dir.second].isUpdated = true;
            UpdateMap.push_back(grid_list[i + dir.first][j + dir.second]);
        }
    }
}

void PowerUpdate(Grid_Tiles**& grid_list, int grid_num, int i, int j) {
    std::vector<std::pair<int, int>> directions = { {1, 0}, {0, -1}, {0, 1}, {-1, 0} };
    auto rng = std::default_random_engine{};
    std::pair<int, int> last_dir = grid_list[i][j].direction;

    if (IsViableLocation(grid_list, grid_num, i + 2 * last_dir.first, j + 2 * last_dir.second, CABLE)
        && IsViableLocation(grid_list, grid_num, i +  last_dir.first, j + last_dir.second, STEEL)) {
        grid_list[i][j].next_substance = CABLE;
        grid_list[i][j].isUpdated = true;
        UpdateMap.push_back(grid_list[i][j]);

        grid_list[i + 2 * last_dir.first][j + 2*last_dir.second].next_substance = POWER;
        grid_list[i + 2 * last_dir.first][j + 2*last_dir.second].isUpdated = true;
        grid_list[i + 2 * last_dir.first][j + 2 * last_dir.second].direction = last_dir;

        UpdateMap.push_back(grid_list[i + 2 * last_dir.first][j + 2 * last_dir.second]);

        grid_list[i + last_dir.first][j + last_dir.second].next_substance = CABLE;
        grid_list[i + last_dir.first][j + last_dir.second].isUpdated = true;

        UpdateMap.push_back(grid_list[i + last_dir.first][j + last_dir.second]);
        return;
    }


    if (IsViableLocation(grid_list, grid_num, i + last_dir.first, j + last_dir.second, CABLE)) {
        grid_list[i][j].next_substance = CABLE;
        grid_list[i][j].isUpdated = true;
        UpdateMap.push_back(grid_list[i][j]);

        grid_list[i + last_dir.first][j + last_dir.second].next_substance = POWER;
        grid_list[i + last_dir.first][j + last_dir.second].isUpdated = true;
        grid_list[i + last_dir.first][j + last_dir.second].direction = last_dir;

        UpdateMap.push_back(grid_list[i + last_dir.first][j + last_dir.second]);
        return;
    }


    for (auto dir : directions) {
        if (dir != last_dir && dir.first != -last_dir.first && dir.second != -last_dir.second) {
            if (IsViableLocation(grid_list, grid_num, i + dir.first, j + dir.second, CABLE)) {
                grid_list[i][j].next_substance = CABLE;
                grid_list[i][j].isUpdated = true;
                UpdateMap.push_back(grid_list[i][j]);

                grid_list[i + dir.first][j + dir.second].next_substance = POWER;
                grid_list[i + dir.first][j + dir.second].isUpdated = true;
                grid_list[i + dir.first][j + dir.second].direction = dir;

                UpdateMap.push_back(grid_list[i + dir.first][j + dir.second]);
                return;
            }
        }
    }

    if (IsViableLocation(grid_list, grid_num, i - last_dir.first, j - last_dir.second, CABLE)) {
        grid_list[i][j].next_substance = CABLE;
        grid_list[i][j].isUpdated = true;
        UpdateMap.push_back(grid_list[i][j]);

        grid_list[i - last_dir.first][j - last_dir.second].next_substance = POWER;
        grid_list[i - last_dir.first][j - last_dir.second].isUpdated = true;
        grid_list[i - last_dir.first][j - last_dir.second].direction.first = -last_dir.first;
        grid_list[i - last_dir.first][j - last_dir.second].direction.second = -last_dir.second;

        UpdateMap.push_back(grid_list[i - last_dir.first][j - last_dir.second]);
        return;
    }



}
//simulation of physics using cellular automata
void ConwaysPhysics(Grid_Tiles**& grid_list, int grid_num, std::vector<sf::Color>& color_list) {
    //color map of neighbors
    //std::vector<ColorMap> color_map;

    //fill temp color with current colors of each cell (temp color is needed so u can update whole map at once at the end)
    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            grid_list[i][j].next_substance = grid_list[i][j].substance;
            grid_list[i][j].isUpdated = false;
            grid_list[i][j].isFalling = false;
        }
    }
    UpdateMap.clear();

    for (int j = grid_num - 1; j >= 0; j--) {
        for (int i = 0; i < grid_num; i++) {
            if (!grid_list[i][j].isUpdated) {
                //SAND:
                if (grid_list[i][j].substance == SAND) {
                    SandUpdate(grid_list, grid_num, i, j);
                }
                //PLANT:
                else if (grid_list[i][j].substance == PLANT) {
                    PlantUpdate(grid_list, grid_num, i, j);
                }
                //FIRE:
                else if (grid_list[i][j].substance == FIRE) {
                    FireUpdate(grid_list, grid_num, i, j);
                }
                //WATER:
                else if (grid_list[i][j].substance == WATER) {
                    WaterUpdate(grid_list, grid_num, i, j);
                }
                //STEAM:
                else if (grid_list[i][j].substance == STEAM) {
                    SteamUpdate(grid_list, grid_num, i, j);
                }
                //CONWAY:
                else if (grid_list[i][j].substance == CONWAY || grid_list[i][j].substance == DEAD) {
                    ConwayUpdate(grid_list, grid_num, i, j);
                }
                else if (grid_list[i][j].substance == CABLE) {
                    CableUpdate(grid_list, grid_num, i, j);
                }
                else if (grid_list[i][j].substance == POWER) {
                    PowerUpdate(grid_list, grid_num, i, j);
                }

            }
        }
    }


    //update all cells at the end of simulation frame to be displayed properly
    
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(UpdateMap), std::end(UpdateMap), rng);

    for (auto& elem : UpdateMap) {
        grid_list[elem.x][elem.y].substance = elem.next_substance;
        grid_list[elem.x][elem.y].UpdateSubstanceColor();
    }

    UpdateMap.clear();
}