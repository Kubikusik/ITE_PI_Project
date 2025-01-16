#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "Manager.h"

enum Sounds {
    GridClick,
    UIClick
};

class Event_Handler {
public:
    bool isNewFrame = true;
    int searched_x = 0;
    int searched_y = 0;
    int last_selected = 0;
    Manager* manager; // Use a pointer to avoid circular dependency
public:
    Event_Handler(Manager* manager_ref); // Pass Manager as a dependency
    void Loop();
    void RenderAll();
    void GridTilesInteraction(sf::Event &event);
    void UIButtonsInteraction(sf::Event &event);
    void PlaySound(Sounds soundtype);
};



#endif
