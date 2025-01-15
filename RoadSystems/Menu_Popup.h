#ifndef MENU_POPUP_H
#define MENU_POPUP_H
#include "UIElements.h"
#include "SFML/Audio.hpp"



sf::Color HSVtoRGB(float H, float S, float V);



class Slider {
public:
	sf::RectangleShape Slider_Shape;  // The background of the slider
	sf::RectangleShape Handle_Shape;  // The draggable part (handle)
	bool isDragging = false;  // Track if the slider is being dragged
	sf::Color SelectedColor = sf::Color::Red;
	sf::Texture HandleTexture;

	// Default constructor
	Slider() {
		Slider_Shape.setSize(sf::Vector2f(100, 300));  // Set a visible default size
		Handle_Shape.setSize(sf::Vector2f(100, 20));   // Handle size
		Handle_Shape.setFillColor(sf::Color::Black);     // Default handle color
	}

	Slider(sf::Vector2f position, sf::Vector2f size);  // Constructor to initialize the slider
	void DrawItself(sf::RenderWindow& window);  // Draw the slider
	void UpdateHandlePosition(sf::RenderWindow& window);  // Update the handle position based on input
	float GetValue() const;  // Get the value of the slider
};

class SoundSlider{
public:
	sf::RectangleShape Slider_Shape;  // The background of the slider
	sf::RectangleShape Handle_Shape;  // The draggable part (handle)
	bool isDragging = false;  // Track if the slider is being dragged
	sf::Color SelectedColor = sf::Color::Red;
	sf::Texture HandleTexture;

	// Default constructor
	SoundSlider() {
		Slider_Shape.setSize(sf::Vector2f(100, 300));  // Set a visible default size
		Handle_Shape.setSize(sf::Vector2f(100, 20));   // Handle size
		Handle_Shape.setFillColor(sf::Color::Black);     // Default handle color
	}

	SoundSlider(sf::Vector2f position, sf::Vector2f size);  // Constructor to initialize the slider
	void DrawItself(sf::RenderWindow& window);  // Draw the slider
	void UpdateHandlePosition(sf::RenderWindow& window);  // Update the handle position based on input
	float GetValue() const;  // Get the value of the slider
	void UpdateVolume(sf::Music* music, float volume);


};

class MenuPopup {
public:
	sf::RectangleShape background;
	sf::Texture bgTexture;
	sf::RenderWindow* main_window;
	sf::Font default_font;

	sf::RectangleShape ColorSquareCross;
	sf::Texture ColorSquareCrossTexture;

	sf::RectangleShape ColorPreview;
	sf::RectangleShape ColorPReviewBorder;

	Slider color_slider;
	SoundSlider sound_slider;
	SoundSlider music_slider;
	bool isVisible = false;
	bool first_time = true;

	sf::Music* music;

	//std::vector<Labeled_Button> Menu_Buttons;

	MenuPopup(sf::RenderWindow* window, sf::Font& default_font);
	void MenuBgInit();
	void MenuDraw();
	void DrawGradient(sf::RenderWindow& window);
	sf::Color InterpolateColor(const sf::Color& startColor, const sf::Color& endColor, float normalizedX, float normalizedY);
	void HandleMouseClick(sf::RenderWindow& window);
};

void drawLine(sf::RenderWindow& window, sf::Vector2i point1, sf::Vector2i point2, int lineWidth, sf::Color lineColor);
#endif
#pragma once
