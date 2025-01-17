#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "Manager.h"

class Event_Handler {
public:
    int searched_x = 0;
    int searched_y = 0;
    Manager* manager; // Use a pointer to avoid circular dependency
public:
    Event_Handler(Manager* manager_ref); // Pass Manager as a dependency
    void Loop();
    void RenderAll();
    void GridTilesInteraction(sf::Event &event);
    void UIButtonsInteraction(sf::Event &event);
};

#endif
