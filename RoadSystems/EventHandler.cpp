#include "EventHandler.h"
#include "ConwaysGameOfLifeMaksiowy.h"


Event_Handler::Event_Handler(Manager* manager_ref) : manager(manager_ref) {
    manager = manager_ref;
    // Initialization code
}

void Event_Handler::Loop() {
    sf::RenderWindow& window = manager->window;

    
    //main loop, running while window wasnt closed yet
    while (window.isOpen())
    {
        // Check if the cooldown period has passed
        if (!manager->load_popup->isVisible && !manager->isNewFrame && manager->click_clock.getElapsedTime().asSeconds() >= manager->clickCooldownDuration) {
            manager->isNewFrame = true; // Re-enable clicks
        }

        sf::Event event;
        //checking if any event happened
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)        //if window is closed
                window.close();
            if (event.type == sf::Event::GainedFocus) {   //if window in focus
                manager->is_focused = true;
                window.setActive(true);
            }
            if (event.type == sf::Event::LostFocus) {    //if window is out of focus
                manager->is_focused = false;
                window.setActive(false);
                window.clear(GetBgColor());
            }

            if (event.type == sf::Event::Resized)       //if window is resized
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));

            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape && manager->is_focused) { //escape pressed closes window
                window.close();
            }

            //Menu under space button:
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M && manager->is_focused && !manager->simulate && manager->isNewFrame) {
                //menu_popup.ToggleMenuPopup(window, default_font);
                //is_focused = false; // Main window loses focus

                UpdateGridBackground(manager->grid_list, grid_num, GetBgColor());
                manager->menu_popup->isVisible = !manager->menu_popup->isVisible;
                manager->isNewFrame = false;
            }

            if (manager->is_focused && !manager->menu_popup->isVisible) {

                //recolor back after hovering
                for (int i = 0; i < grid_num; i++) {
                    for (int j = 0; j < grid_num; j++) {
                        manager->grid_list[i][j].Recolor(manager->grid_list[i][j].default_color);
                    }
                }
                UIButtonsInteraction(event);
                
            }
            GridTilesInteraction(event);
            
        }

        //simulation loop only on some frames
        if (manager->simulate && (manager->delta == 0)) {
            ConwaysPhysics(manager->grid_list, grid_num, manager->color_list);
        }

        

        //update screen
        window.clear();
        RenderAll();

        window.display();

        

        //add +1 to time and if it reaches desired tpf reset it to 0 (simulation frame)
        manager->delta += 1;
        manager->delta = manager->delta % manager->sim_tpf;
    }
}


void Event_Handler::RenderAll() {

    sf::RenderWindow& window = manager->window;

    window.draw(manager->SideUI);

    //Draw Grid
    for (int i = 0; i < grid_num; i++) {
        for (int j = 0; j < grid_num; j++) {
            window.draw(manager->grid_list[i][j].square);
        }
    }

    //Draw Buttons
    for (auto& each : manager->button_list)
    {
        each.DrawItself(window);
    }
    manager->ui_buttons->save_button->DrawItself(window);
    manager->ui_buttons->load_button->DrawItself(window);
    manager->ui_buttons->simulate_button->DrawItself(window);
    manager->ui_buttons->brush_size_label->DrawItself(window);
    manager->ui_buttons->increase_brush_button->DrawItself(window);
    manager->ui_buttons->decrease_brush_button->DrawItself(window);
    manager->ui_buttons->time_speed_label->DrawItself(window);
    manager->ui_buttons->increase_time_button->DrawItself(window);
    manager->ui_buttons->decrease_time_button->DrawItself(window);


    if (manager->menu_popup->isVisible) {
        manager->menu_popup->MenuDraw();
        manager->menu_popup->first_time = false;
    }
	if (manager->load_popup->isVisible) {
        manager->load_popup->LoadPreset();
        manager->load_popup->LoadDraw(manager->grid_list);
	}
}

void Event_Handler::GridTilesInteraction(sf::Event &event) {
    sf::RenderWindow& window = manager->window;
    if (event.type = sf::Event::MouseMoved) { //if mouse is moved

        //if window is being focused on
        if (manager->is_focused && !manager->menu_popup->isVisible && manager->isNewFrame) {

            //get position in pixels and translate it to what grid tile is that:
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            searched_x = (mouse_pos.x - ui_size) / (grid_size + 1);
            searched_y = (mouse_pos.y) / (grid_size + 1);
            //search for which cell is hovered over
            if (mouse_pos.x > ui_size && mouse_pos.y > 0 && mouse_pos.x < map_x + ui_size && mouse_pos.y < map_y) {

                int minimal_x = std::max(searched_x - manager->brush_size + 1, 0);
                int maximal_x = std::min(searched_x + manager->brush_size - 1, grid_num - 1);

                int minimal_y = std::max(searched_y - manager->brush_size + 1, 0);
                int maximal_y = std::min(searched_y + manager->brush_size - 1, grid_num - 1);

                //Hovering mouse over cell / grey-out
                for (int dx = minimal_x; dx <= maximal_x; dx++) {
                    for (int dy = minimal_y; dy <= maximal_y; dy++) {
                        manager->grid_list[dx][dy].Recolor(
                            sf::Color(
                                abs(manager->grid_list[dx][dy].default_color.r - 60),
                                abs(manager->grid_list[dx][dy].default_color.g - 60),
                                abs(manager->grid_list[dx][dy].default_color.b - 60)
                            )
                        );
                    }
                }

                //left click action on cell
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !manager->simulate) {
                    
                    PlaySound(GridClick);

                    for (int x = minimal_x; x <= maximal_x; x++) {
                        for (int y = minimal_y; y <= maximal_y; y++) {
                            
                            manager->grid_list[x][y].next_substance = manager->paint_substance;
                            manager->grid_list[x][y].UpdateSubstanceColor();
                        }
                    }
                }


                //right click action on cell
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !manager->simulate) {

                    int minimal_x = std::max(searched_x - manager->brush_size + 1, 0);
                    int maximal_x = std::min(searched_x + manager->brush_size - 1, grid_num - 1);

                    int minimal_y = std::max(searched_y - manager->brush_size + 1, 0);
                    int maximal_y = std::min(searched_y + manager->brush_size - 1, grid_num - 1);

                    for (int x = minimal_x; x <= maximal_x; x++) {
                        for (int y = minimal_y; y <= maximal_y; y++) {
                            manager->grid_list[x][y].default_color = GetBgColor();
                            manager->grid_list[x][y].substance = DEAD;
                        }
                    }

                }
            }
        }


    }
}

void Event_Handler::UIButtonsInteraction(sf::Event &event) {

    sf::RenderWindow& window = manager->window;

    //Color buttons interactions
    for (auto& each : manager->button_list) {

        if (each.square.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))) {

            //Hovering mouse over cell:
            each.Recolor(sf::Color(abs(each.default_color.r - 60), abs(each.default_color.g - 60), abs(each.default_color.b - 60)));


            //Left-clicking - selecting:
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                PlaySound(UIClick);
                each.ChangePaintColor(manager->paint_color, manager->paint_substance);
            }

        }

        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            each.Recolor(each.default_color);
        }
    }

    //BRUSH CONTROLS
    //Plus Button clicking
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        manager->ui_buttons->increase_brush_button->Clicked(window, manager->simulate, manager->brush_size, manager->ui_buttons->brush_size_label->button_label, grid_num);
    }
    else manager->ui_buttons->increase_brush_button->Release();
    //Minus Button clicking
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {;
        manager->ui_buttons->decrease_brush_button->Clicked(window, manager->simulate, manager->brush_size, manager->ui_buttons->brush_size_label->button_label);
    }
    else manager->ui_buttons->decrease_brush_button->Release();


    //TIME CONTROLS
    //Plus time Button clicking
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        manager->ui_buttons->increase_time_button->Clicked(window, manager->simulate, manager->time_speed, manager->ui_buttons->time_speed_label->button_label);
        manager->sim_tpf = std::round(1 + std::log2(21 - manager->time_speed));
    }
    else manager->ui_buttons->increase_time_button->Release();
    //Minus time Button clicking
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        manager->ui_buttons->decrease_time_button->Clicked(window, manager->simulate, manager->time_speed, manager->ui_buttons->time_speed_label->button_label);
        manager->sim_tpf = std::round(1 + std::log2(21 - manager->time_speed));
    }
    else manager->ui_buttons->decrease_time_button->Release();


    //Start simulation button
    if (manager->ui_buttons->simulate_button->square.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))) {
        //Hovering mouse over cell:

        manager->ui_buttons->simulate_button->Recolor(sf::Color(abs(manager->ui_buttons->simulate_button->default_color.r - 60),
            abs(manager->ui_buttons->simulate_button->default_color.g - 60),
            abs(manager->ui_buttons->simulate_button->default_color.b - 60)));


        //Left-clicking - selecting:
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            manager->ui_buttons->simulate_button->ToggleSimulate(manager->simulate);
        }
        else {
            manager->ui_buttons->simulate_button->Release();
        }
    }
    else {
        manager->ui_buttons->simulate_button->Recolor(manager->ui_buttons->simulate_button->default_color);
    }

    if (event.type == sf::Event::MouseWheelMoved) {
        if (event.mouseWheel.delta > 0) {
            //Plus Button clicking
            //brush_size += 1;
            manager->ui_buttons->increase_brush_button->ScrollUp(window, manager->simulate, manager->brush_size, manager->ui_buttons->brush_size_label->button_label);


        }
        else if (event.mouseWheel.delta < 0) {
            //brush_size -= 1;
            manager->ui_buttons->decrease_brush_button->ScrollDown(window, manager->simulate, manager->brush_size, manager->ui_buttons->brush_size_label->button_label);
        }
    }

    //Save Button cliking
    manager->ui_buttons->save_button->Clicked(window, manager->simulate, manager->image, grid_num, grid_size, manager->grid_list);

    //Load Button clicking
    manager->ui_buttons->load_button->Clicked(window, manager->simulate, grid_num, grid_size, manager->grid_list, &(manager->load_popup->isVisible), &manager->isNewFrame, &(manager->load_popup->doc));
}

void Event_Handler::PlaySound(Sounds soundtype) {
    if (soundtype == GridClick) {
        manager->grid_sound.setBuffer(manager->grid_sound_buff);
        //manager->grid_sound.setVolume(100.f);
        manager->grid_sound.play();
        
    }
    else if (soundtype == UIClick) {
        int curr_selected = rand() % 4;
        if (curr_selected == last_selected) curr_selected = (curr_selected + 1) % 4;
        last_selected = curr_selected;
        manager->grid_sound.setBuffer(manager->button_sound[curr_selected]);

        manager->grid_sound.play();
    }
}
    