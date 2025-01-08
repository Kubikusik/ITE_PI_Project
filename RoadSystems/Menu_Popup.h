#ifndef MENU_POPUP_H
#define MENU_POPUP_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "UIElements.h"



sf::Color HSVtoRGB(float H, float S, float V);

class Slider {
public:
	sf::RectangleShape Slider_Shape;  // The background of the slider
	sf::RectangleShape Handle_Shape;  // The draggable part (handle)
	bool isDragging = false;  // Track if the slider is being dragged
	sf::Color SelectedColor = sf::Color::Red;

	// Default constructor
	Slider() {
		Slider_Shape.setSize(sf::Vector2f(100, 300));  // Set a visible default size
		Handle_Shape.setSize(sf::Vector2f(100, 20));   // Handle size
		Handle_Shape.setFillColor(sf::Color::Cyan);     // Default handle color
	}

	Slider(sf::Vector2f position, sf::Vector2f size);  // Constructor to initialize the slider
	void DrawItself(sf::RenderWindow& window);  // Draw the slider
	void UpdateHandlePosition(sf::RenderWindow& window);  // Update the handle position based on input
	float GetValue() const;  // Get the value of the slider
};

class MenuPopup {
public:
	sf::RectangleShape background;
	sf::Texture bgTexture;
	sf::RenderWindow* main_window;
	sf::Font default_font;

	Slider color_slider;

	bool isVisible = false;
	bool first_time = true;

	std::vector<Labeled_Button> Menu_Buttons;

	MenuPopup(sf::RenderWindow* window, sf::Font default_font);
	void MenuBgInit();
	void MenuDraw();
	void DrawGradient(sf::RenderWindow& window);
	sf::Color InterpolateColor(const sf::Color& startColor, const sf::Color& endColor, float normalizedX, float normalizedY);
	void HandleMouseClick(sf::RenderWindow& window);
	sf::Color GetColorFromGradient(float normalizedX, float normalizedY);
};
#endif
#pragma once
