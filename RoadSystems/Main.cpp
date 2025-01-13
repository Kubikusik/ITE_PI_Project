#include <iostream>
#include "Menu_Popup.h"
#include "Manager.h"
#include "EventHandler.h"

int main()
{
    // Create the Manager instance
    Manager manager;

    // Create the Event_Handler and pass the Manager instance
    Event_Handler eventHandler(&manager);

    // Start the event loop
    eventHandler.Loop();

    return 0;
}