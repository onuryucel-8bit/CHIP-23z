#pragma once
#include <iostream>
#include <cstdlib>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#include "Chip8.h"

class Window_SFML
{
public:
	Window_SFML();
	~Window_SFML();

	void run();

	uint8_t flag;
private:

	bool keys[16] = {};

	Chip8 chip_8;

	sf::RenderWindow* m_window;
	sf::RenderWindow* m_windowInfo;

	sf::RectangleShape pixels[SCREEN_WIDTH * SCREEN_HEIGHT];

	sf::SoundBuffer buffer;
	sf::Font font;

	sf::Text regs[16];
	sf::Text PC;//program counter
	sf::Text indexReg;
	sf::Text SP;//stack pointer

	sf::Text delayTimer;
	sf::Text soundTimer;

	sf::Sound sound;

	void createPixels();
	void createWindows();
	void createTexts();
	void createSound();

	void getInputs();

	void drawTexts();

	void reFreshScreen();
	void clearScreen();

	void updateChip8();

	void waitForKeys();

	void drawPixels();
	
};
