#include <iostream>
#include "Menu_Popup.h"
#include <vector>

Slider::Slider(sf::Vector2f position, sf::Vector2f size) {
    // Initialize the slider background (a vertical bar)
    Slider_Shape.setPosition(position);
    Slider_Shape.setSize(size);
    Slider_Shape.setFillColor(sf::Color(150, 150, 150));  // Grey background

    // Initialize the handle (the draggable part)
    Handle_Shape.setSize(sf::Vector2f(size.x, 20));  // Handle width matches the slider width
    //Handle_Shape.setFillColor(sf::Color::Black);  // Black handle
    Handle_Shape.setPosition(position.x, position.y);  // Position at the top

}

MenuPopup::MenuPopup(sf::RenderWindow* window, sf::Font& default_font) {
    this->main_window = window;
    this->default_font = default_font;
    MenuBgInit();

    // Initialize color_slider directly
    this->color_slider = Slider(sf::Vector2f(800, 200), sf::Vector2f(50, 255));  // Direct initialization
    if (!color_slider.HandleTexture.loadFromFile("./Textures/MenuPopup/ColorSliderHandle.png")) {
        std::cerr << "Error: Could not load the texture\n";
    }
    else {
        std::cout << "Texture loaded successfully\n";
    }
    color_slider.Handle_Shape.setTexture(&color_slider.HandleTexture);

    sound_slider = SoundSlider(sf::Vector2f(300, 500), sf::Vector2f(255, 50));
    if (!sound_slider.HandleTexture.loadFromFile("./Textures/MenuPopup/SoundSliderHandle.png")) {
        std::cerr << "Error: Could not load the texture\n";
    }
    else {
        std::cout << "Texture loaded successfully\n";
    }
    sound_slider.Handle_Shape.setTexture(&sound_slider.HandleTexture);

    music_slider = SoundSlider(sf::Vector2f(600, 500), sf::Vector2f(255, 50));
    if (!music_slider.HandleTexture.loadFromFile("./Textures/MenuPopup/SoundSliderHandle.png")) {
        std::cerr << "Error: Could not load the texture\n";
    }
    else {
        std::cout << "Texture loaded successfully\n";
    }
    music_slider.Handle_Shape.setTexture(&music_slider.HandleTexture);
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
    bgTexture.loadFromFile("./Textures/MenuPopup/Background.png");
    //bgTexture.setRepeated(true);
    background.setTexture(&bgTexture);

    ColorSquareCross.setPosition(5000, 5000);
    ColorSquareCross.setSize(sf::Vector2f(30,30));
    ColorSquareCrossTexture.loadFromFile("./Textures/MenuPopup/Cross.png");
    ColorSquareCross.setTexture(&ColorSquareCrossTexture);

    ColorPreview.setPosition(sf::Vector2f(305, 305));
    ColorPreview.setSize(sf::Vector2f(70,70));
    ColorPreview.setFillColor(GetBgColor());

    ColorPReviewBorder.setPosition(sf::Vector2f(300, 300));
    ColorPReviewBorder.setSize(sf::Vector2f(80, 80));
    ColorPReviewBorder.setFillColor(sf::Color::Black);
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
    color_display.setPosition(260, 225);
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
    DrawGradient(*main_window); // Draw the gradient
    HandleMouseClick(*main_window); // Handle clicks on the gradient and slider
    sound_slider.UpdateHandlePosition(*main_window);

    // Adjust the background color based on the slider
    color_slider.DrawItself(*main_window); // Draw slider last
    this->main_window->draw(color_display);
    sound_slider.DrawItself(*main_window);
    music_slider.DrawItself(*main_window);

    sf::Text sound_volume;
    sound_volume.setFont(default_font);
    sound_volume.setCharacterSize(20);
    sound_volume.setPosition(sound_slider.Slider_Shape.getPosition().x, sound_slider.Slider_Shape.getPosition().y - 30);
    sound_volume.setFillColor(sf::Color::Black);
    sound_volume.setString("Sound Volume " + std::to_string(int(sound_slider.GetValue())));
    main_window->draw(sound_volume);

    sf::Text music_volume;
    music_volume.setFont(default_font);
    music_volume.setCharacterSize(20);
    music_volume.setPosition(music_slider.Slider_Shape.getPosition().x, music_slider.Slider_Shape.getPosition().y - 30);
    music_volume.setFillColor(sf::Color::Black);
    music_volume.setString("Music Volume " + std::to_string(int(music_slider.GetValue())));
    main_window->draw(music_volume);
    music_slider.UpdateVolume(music, music_slider.GetValue());

}


sf::Color HSVtoRGB(float h, float s, float v) {
    s /= 100.0f; // Convert saturation to range [0, 1]
    v /= 100.0f; // Convert value to range [0, 1]

    float c = v * s; // Chroma
    float x = c * (1 - std::fabs(fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    float r = 0, g = 0, b = 0;
    if (h >= 0 && h < 60) {
        r = c; g = x; b = 0;
    } else if (h >= 60 && h < 120) {
        r = x; g = c; b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0; g = c; b = x;
    } else if (h >= 180 && h < 240) {
        r = 0; g = x; b = c;
    } else if (h >= 240 && h < 300) {
        r = x; g = 0; b = c;
    } else if (h >= 300 && h < 360) {
        r = c; g = 0; b = x;
    }

    // Convert to [0, 255] and return sf::Color
    return sf::Color((r + m) * 255, (g + m) * 255, (b + m) * 255);
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

    // Create a vertex array for the gradient
    sf::VertexArray gradient(sf::Points, width * height);

    // Loop through each pixel
    for (int x = 0; x < width; ++x) {
        float saturation = x / width; // Vary saturation from left (0) to right (1)

        for (int y = 0; y < height; ++y) {
            float brightness = 1.0f - (y / height); // Vary brightness from top (1) to bottom (0)

            // Convert HSV to RGB
            sf::Color color = HSVtoRGB(hue, saturation * 100, brightness * 100);

            // Add the vertex with the color
            gradient[x + y * width].position = sf::Vector2f(topLeft.x + x, topLeft.y + y);
            gradient[x + y * width].color = color;
        }
    }

    // Draw the gradient
    window.draw(gradient);

    // Handle drawing cross, lines, and borders (unchanged)
    window.draw(ColorSquareCross);

    if (ColorSquareCross.getPosition() != sf::Vector2f(5000, 5000)) {
        drawLine(window, sf::Vector2i(ColorPReviewBorder.getPosition().x + ColorPReviewBorder.getSize().x - 5, ColorPReviewBorder.getPosition().y), sf::Vector2i(ColorSquareCross.getPosition().x, ColorSquareCross.getPosition().y), 5, sf::Color::Black);
        drawLine(window, sf::Vector2i(ColorPReviewBorder.getPosition().x + ColorPReviewBorder.getSize().x - 5, ColorPReviewBorder.getPosition().y + ColorPReviewBorder.getSize().y - 5), sf::Vector2i(ColorSquareCross.getPosition().x, ColorSquareCross.getPosition().y + ColorSquareCross.getSize().y - 5), 5, sf::Color::Black);
    }
    else {
        drawLine(window, sf::Vector2i(ColorPReviewBorder.getPosition().x + ColorPReviewBorder.getSize().x - 5, ColorPReviewBorder.getPosition().y), sf::Vector2i(topLeft), 5, sf::Color::Black);
        drawLine(window, sf::Vector2i(ColorPReviewBorder.getPosition().x + ColorPReviewBorder.getSize().x - 5, ColorPReviewBorder.getPosition().y + ColorPReviewBorder.getSize().y - 5), sf::Vector2i(topLeft.x, 0) + sf::Vector2i(0, bottomRight.y - 5), 5, sf::Color::Black);
    }

    window.draw(ColorPReviewBorder);
    ColorPreview.setFillColor(GetBgColor());
    window.draw(ColorPreview);
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

    float pos_x = Slider_Shape.getPosition().x;
    float pos_y = Slider_Shape.getPosition().y;
    float segment_height = Slider_Shape.getSize().y / 360;

    // Use `previous_bottom` to ensure continuity between segments
    float previous_bottom = pos_y;

    for (int i = 0; i < 360; i++) {
        sf::VertexArray quad(sf::Quads, 4);

        // Top of the current segment
        float current_top = previous_bottom;

        // Bottom of the current segment
        float current_bottom = pos_y + (i + 1) * segment_height;

        sf::Color color = HSVtoRGB(i, 100, 100);
        sf::Color next_color = HSVtoRGB((i + 1) % 360, 100, 100);

        // Top-left
        quad[0].position = sf::Vector2f(pos_x, current_top);
        quad[0].color = color;

        // Top-right
        quad[1].position = sf::Vector2f(pos_x + Slider_Shape.getSize().x, current_top);
        quad[1].color = color;

        // Bottom-right
        quad[2].position = sf::Vector2f(pos_x + Slider_Shape.getSize().x, current_bottom);
        quad[2].color = next_color;

        // Bottom-left
        quad[3].position = sf::Vector2f(pos_x, current_bottom);
        quad[3].color = next_color;

        // Update `previous_bottom` for the next segment
        previous_bottom = current_bottom;

        // Draw the quad
        window.draw(quad);
    }

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
            float normalizedX = (mousePos.x - topLeft.x) / (bottomRight.x - topLeft.x); // [0, 1]
            float normalizedY = (mousePos.y - topLeft.y) / (bottomRight.y - topLeft.y); // [0, 1]

            // Vary saturation based on X position and brightness based on Y position
            float saturation = normalizedX * 100;      // Convert to range [0, 100]
            float brightness = (1.0f - normalizedY) * 100; // Convert to range [0, 100]

            // Get the current hue from the slider
            float hue = color_slider.GetValue();

            // Convert HSV to RGB
            sf::Color selectedColor = HSVtoRGB(hue, saturation, brightness);

            // Set the selected color as the background color
            ColorSquareCross.setPosition(sf::Vector2f(mousePos.x - ColorSquareCross.getSize().x / 2, mousePos.y - ColorSquareCross.getSize().y / 2));
            if (ColorSquareCross.getPosition().x < topLeft.x) {
                ColorSquareCross.setPosition(topLeft.x, ColorSquareCross.getPosition().y);
            }
            if (ColorSquareCross.getPosition().x > bottomRight.x - ColorSquareCross.getSize().x) {
                ColorSquareCross.setPosition(bottomRight.x - ColorSquareCross.getSize().x, ColorSquareCross.getPosition().y);
            }
            if (ColorSquareCross.getPosition().y < topLeft.y) {
                ColorSquareCross.setPosition(ColorSquareCross.getPosition().x, topLeft.y);
            }
            if (ColorSquareCross.getPosition().y > bottomRight.y - ColorSquareCross.getSize().y) {
                ColorSquareCross.setPosition(ColorSquareCross.getPosition().x, bottomRight.y - ColorSquareCross.getSize().y);
            }

            ChangeBackgroundColor(selectedColor);
        }
    }
}



void drawLine(sf::RenderWindow& window, sf::Vector2i point1, sf::Vector2i point2, int lineWidth, sf::Color lineColor) {
    int x0 = point1.x;
    int y0 = point1.y;
    int x1 = point2.x;
    int y1 = point2.y;
    int dx = abs(x1 - x0);
    int sx, sy;
    if (x0 < x1) {
        sx = 1;
    }
    else {
        sx = -1;
    }

    int dy = -abs(y1 - y0);
    if (y0 < y1) {
        sy = 1;
    }
    else {
        sy = -1;
    }

    int err = dx + dy;

    while (true) {
        sf::RectangleShape rect(sf::Vector2f(lineWidth, lineWidth));
        rect.setFillColor(lineColor);
        rect.setPosition(x0, y0);
        window.draw(rect);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        int e2 = 2 * err;

        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

SoundSlider::SoundSlider(sf::Vector2f position, sf::Vector2f size) {
    // Initialize the slider background (a vertical bar)
    Slider_Shape.setPosition(position);
    Slider_Shape.setSize(size);
    Slider_Shape.setFillColor(sf::Color(150, 150, 150));  // Grey background

    // Initialize the handle (the draggable part)
    Handle_Shape.setSize(sf::Vector2f(20, size.y));  // Handle width matches the slider width
    //Handle_Shape.setFillColor(sf::Color::Black);  // Black handle
    Handle_Shape.setPosition(position.x, position.y);  // Position at the top

}

void SoundSlider::UpdateHandlePosition(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    // Check if mouse is within the handle's bounds and if the left button is pressedm
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (Handle_Shape.getGlobalBounds().contains(worldPos) || isDragging) {
            isDragging = true;  // Start dragging if the handle is clicked

            // Clamp the new position to stay within the slider's bounds
            float newX = std::max(Slider_Shape.getPosition().x,
                std::min(worldPos.x, Slider_Shape.getPosition().x + Slider_Shape.getSize().x - Handle_Shape.getSize().x));

            Handle_Shape.setPosition(newX, Handle_Shape.getPosition().y);  // Update handle's Y position
        }
    }
    else {
        isDragging = false;  // Stop dragging when the mouse is released

    }
}

void SoundSlider::DrawItself(sf::RenderWindow& window) {
    UpdateHandlePosition(window);  // Update handle position based on mouse input

    // Draw the slider background and the handle
    window.draw(Slider_Shape);

    float pos_x = Slider_Shape.getPosition().x;
    float pos_y = Slider_Shape.getPosition().y;
    float segment_height = Slider_Shape.getSize().y / 360;

    // Use `previous_bottom` to ensure continuity between segments
    float previous_bottom = pos_y;

    window.draw(Handle_Shape);
}

float SoundSlider::GetValue() const{
    float handlePositionX = Handle_Shape.getPosition().x;
    float sliderTopX = Slider_Shape.getPosition().x;
    float sliderWidth = Slider_Shape.getSize().x;

    // Calculate the relative position (value between 0 and 1)
    float normalizedValue = (handlePositionX - sliderTopX) / (sliderWidth - Handle_Shape.getSize().x);

    // Scale the value to be between 0 and 100 (hue)
    return normalizedValue * 100.0f;
}

void SoundSlider::UpdateVolume(sf::Music* music, float volume) {
    music->setVolume(volume);
}
