#include <iostream>
#include <SFML/Graphics.hpp>
#include "UIElements.h"
#include "Menu_Popup.h"
#include <vector>

Slider::Slider(sf::Vector2f position, sf::Vector2f size) {
    // Initialize the slider background (a vertical bar)
    Slider_Shape.setPosition(position);
    Slider_Shape.setSize(size);
    Slider_Shape.setFillColor(sf::Color(150, 150, 150));  // Grey background

    // Initialize the handle (the draggable part)
    Handle_Shape.setSize(sf::Vector2f(size.x, 20));  // Handle width matches the slider width
    Handle_Shape.setFillColor(sf::Color::Black);  // Cyan handle
    Handle_Shape.setPosition(position.x, position.y);  // Position at the top
}

MenuPopup::MenuPopup(sf::RenderWindow* window, sf::Font default_font) {
    this->main_window = window;
    this->default_font = default_font;
    MenuBgInit();

    // Initialize color_slider directly
    this->color_slider = Slider(sf::Vector2f(800, 200), sf::Vector2f(50, 255));  // Direct initialization
}

void MenuPopup::MenuBgInit() {
    background.setSize(sf::Vector2f(
        (this->main_window)->getSize().x / 1.3f,
        (this->main_window)->getSize().y / 1.3f
    ));
    background.setPosition(sf::Vector2f(
        (this->main_window->getSize().x / 2.0f - background.getSize().x / 2.0f),
        (this->main_window->getSize().y / 2.0f - background.getSize().y / 2.0f)
    ));

    background.setFillColor(sf::Color(200, 200, 200));
    bgTexture.loadFromFile("Background.png");
    bgTexture.setRepeated(true);
    background.setTexture(&bgTexture);
}

void MenuPopup::MenuDraw() {
    (this->main_window)->draw(background);

    sf::Color bg_color = GetBgColor(); // Get current background color

    //// Red color adjustment buttons
    //Menu_Popup_Plus_Button increase_R(300, 200, 50, "plus_r", "+", default_font, sf::Color(200, 0, 0));
    //Menu_Popup_Minus_Button decrease_R(350, 200, 50, "minus_r", "-", default_font, sf::Color(200, 0, 0));

    //// Green color adjustment buttons
    //Menu_Popup_Plus_Button increase_G(300, 300, 50, "plus_g", "+", default_font, sf::Color(0, 200, 0));
    //Menu_Popup_Minus_Button decrease_G(350, 300, 50, "minus_g", "-", default_font, sf::Color(0, 200, 0));

    //// Blue color adjustment buttons
    //Menu_Popup_Plus_Button increase_B(300, 400, 50, "plus_b", "+", default_font, sf::Color(0, 0, 200));
    //Menu_Popup_Minus_Button decrease_B(350, 400, 50, "minus_b", "-", default_font, sf::Color(0, 0, 200));

    sf::Text color_display;
    color_display.setFont(default_font);
    color_display.setCharacterSize(20);
    color_display.setPosition(550, 250);
    color_display.setFillColor(sf::Color::Black);

    //if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    //    increase_R.Clicked(*(this->main_window), bg_color, color_display, "r");
    //    decrease_R.Clicked(*(this->main_window), bg_color, color_display, "r");

    //    increase_G.Clicked(*(this->main_window), bg_color, color_display, "g");
    //    decrease_G.Clicked(*(this->main_window), bg_color, color_display, "g");

    //    increase_B.Clicked(*(this->main_window), bg_color, color_display, "b");
    //    decrease_B.Clicked(*(this->main_window), bg_color, color_display, "b");

    //    // Update the global background color based on button clicks
    //    ChangeBackgroundColor(bg_color);
    //}
    //else {
    //    increase_R.Release(); // Reset clicked state
    //    decrease_R.Release();

    //    increase_G.Release(); // Reset clicked state
    //    decrease_G.Release();

    //    increase_B.Release(); // Reset clicked state
    //    decrease_B.Release();
    //}

    color_display.setString("R: " + std::to_string(bg_color.r) +
        " G: " + std::to_string(bg_color.g) +
        " B: " + std::to_string(bg_color.b));

   /* increase_R.DrawItself(*(this->main_window));
    decrease_R.DrawItself(*(this->main_window));
    increase_G.DrawItself(*(this->main_window));
    decrease_G.DrawItself(*(this->main_window));
    increase_B.DrawItself(*(this->main_window));
    decrease_B.DrawItself(*(this->main_window));*/

    HandleMouseClick(*main_window); // Handle clicks on the gradient and slider
    DrawGradient(*main_window); // Draw the gradient

    // Adjust the background color based on the slider
    color_slider.DrawItself(*(this->main_window)); // Draw slider last
    this->main_window->draw(color_display);
}
sf::Color HSVtoRGB(float H, float S, float V) {
    float C = S * V; // Chroma
    float HPrime = std::fmod(H / 60, 6.f); // H'
    float X = C * (1 - std::fabs(std::fmod(HPrime, 2.f) - 1));
    float M = V - C;

    float R = 0.f;
    float G = 0.f;
    float B = 0.f;

    switch (static_cast<int>(HPrime)) {
    case 0: R = C; G = X;        break; // [0, 1)
    case 1: R = X; G = C;        break; // [1, 2)
    case 2:        G = C; B = X; break; // [2, 3)
    case 3:        G = X; B = C; break; // [3, 4)
    case 4: R = X;        B = C; break; // [4, 5)
    case 5: R = C;        B = X; break; // [5, 6)
    }

    R += M;
    G += M;
    B += M;

    sf::Color color;
    color.r = static_cast<sf::Uint8>(std::round(R * 255));
    color.g = static_cast<sf::Uint8>(std::round(G * 255));
    color.b = static_cast<sf::Uint8>(std::round(B * 255));

    return color;
}

void MenuPopup::DrawGradient(sf::RenderWindow& window) {
    // Define the top-left and bottom-right corners of the gradient square
    sf::Vector2f topLeft(500, 200);
    sf::Vector2f bottomRight(755, 455);

    // Get the selected hue from the slider (this will affect the base color)
    float hue = color_slider.GetValue();

    // Calculate the width and height of the square
    float width = bottomRight.x - topLeft.x;
    float height = bottomRight.y - topLeft.y;

    // Create a vector to store the gradient vertices
    std::vector<sf::Vertex> ColorSquare;

    // Loop through each column (one per pixel along the X axis)
    for (int i = 0; i < width - 1; ++i) {
        // Vary hue from 0 to 360 across the X axis (gradual transition of the entire gradient)
        // For now, we are using the hue selected by the slider.
        float saturation = (i * 1.0f / width);  // Varying saturation from left (0) to right (1)
        for (int j = 0; j < height - 1; ++j) {
            float brightness = 1.0f - (j * 1.0f / height);  // Varying brightness from top (1) to bottom (0)

            // Convert the color from HSV to RGB using the varying hue, saturation, and brightness
            sf::Color color = HSVtoRGB(hue, saturation, brightness);

            // Define the 4 vertices for each vertical strip (one for each side of the rectangle)
            ColorSquare.push_back(sf::Vertex(sf::Vector2f(topLeft.x + i, topLeft.y + j), color)); // Top-left
            ColorSquare.push_back(sf::Vertex(sf::Vector2f(topLeft.x + i, topLeft.y + j + 1), color)); // Bottom-left
            ColorSquare.push_back(sf::Vertex(sf::Vector2f(topLeft.x + i + 1, topLeft.y + j + 1), color)); // Bottom-right
            ColorSquare.push_back(sf::Vertex(sf::Vector2f(topLeft.x + i + 1, topLeft.y + j), color)); // Top-right
        }
    }

    // Draw the entire gradient as quads
    window.draw(&ColorSquare[0], ColorSquare.size(), sf::Quads);
}

sf::Color MenuPopup::InterpolateColor(const sf::Color& startColor, const sf::Color& endColor, float normalizedX, float normalizedY) {
    int r = static_cast<int>(startColor.r + (endColor.r - startColor.r) * normalizedX);
    int g = static_cast<int>(startColor.g + (endColor.g - startColor.g) * normalizedY);
    int b = static_cast<int>(startColor.b + (endColor.b - startColor.b) * normalizedX);
    return sf::Color(r, g, b);
}

void Slider::UpdateHandlePosition(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    // Check if mouse is within the handle's bounds and if the left button is pressed
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (Handle_Shape.getGlobalBounds().contains(worldPos) || isDragging) {
            isDragging = true;  // Start dragging if the handle is clicked

            // Clamp the new position to stay within the slider's bounds
            float newY = std::max(Slider_Shape.getPosition().y,
                std::min(worldPos.y, Slider_Shape.getPosition().y + Slider_Shape.getSize().y - Handle_Shape.getSize().y));

            Handle_Shape.setPosition(Handle_Shape.getPosition().x, newY);  // Update handle's Y position
        }
    }
    else {
        isDragging = false;  // Stop dragging when the mouse is released
    }
}

void Slider::DrawItself(sf::RenderWindow& window) {
    UpdateHandlePosition(window);  // Update handle position based on mouse input

    // Draw the slider background and the handle
    window.draw(Slider_Shape);
    window.draw(Handle_Shape);
}

float Slider::GetValue() const {
    float handlePositionY = Handle_Shape.getPosition().y;
    float sliderTopY = Slider_Shape.getPosition().y;
    float sliderHeight = Slider_Shape.getSize().y;

    // Calculate the relative position (value between 0 and 1)
    float normalizedValue = (handlePositionY - sliderTopY) / (sliderHeight - Handle_Shape.getSize().y);

    // Scale the value to be between 0 and 360 (hue)
    return normalizedValue * 359.0f;
}

void MenuPopup::HandleMouseClick(sf::RenderWindow& window) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        // Get mouse position
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Define the bounds of the gradient square
        sf::Vector2f topLeft(500, 200);
        sf::Vector2f bottomRight(755, 455);

        // Check if mouse is inside the gradient square
        if (mousePos.x >= topLeft.x && mousePos.x <= bottomRight.x &&
            mousePos.y >= topLeft.y && mousePos.y <= bottomRight.y) {

            // Normalize the mouse position within the square
            float normalizedX = (mousePos.x - topLeft.x) / (bottomRight.x - topLeft.x);
            float normalizedY = (mousePos.y - topLeft.y) / (bottomRight.y - topLeft.y);

            // Interpolate the color based on the normalized mouse position
            // Now we calculate the color based on selected hue, saturation and brightness
            float hue = color_slider.GetValue(); // Get the current hue from the slider
            float saturation = normalizedX;      // Vary saturation based on X position
            float brightness = 1.0f - normalizedY; // Vary brightness based on Y position

            // Get the final color based on hue, saturation, and brightness
            sf::Color selectedColor = HSVtoRGB(hue, saturation, brightness);

            // Set the selected color as the background color
            ChangeBackgroundColor(selectedColor);
        }
    }
}

sf::Color MenuPopup::GetColorFromGradient(float normalizedX, float normalizedY) {
    // You can create a gradient logic here to return a color based on the normalizedX and normalizedY values
    // For now, we return a simple gradient from red to green to blue
    int red = static_cast<int>(normalizedX * 255);
    int green = static_cast<int>(normalizedY * 255);
    int blue = 255 - red;

    return sf::Color(red, green, blue);
}