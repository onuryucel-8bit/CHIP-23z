#include "Window_SFML.h"

#pragma region init

void Window_SFML::createSound() {

	if (!buffer.loadFromFile(CURRENT_DIRECTORY + "res/ch8_squareWave.wav")) {
		std::cout << "ERROR : failed the load sound file \n";
	}

	sound.setBuffer(buffer);
}

void Window_SFML::createTexts() {

	if (!font.loadFromFile(CURRENT_DIRECTORY + "res/Retro Gaming.ttf")) {
		std::cout << "ERROR :: failed to load font \n";
	}

	for (size_t i = 0; i < 16; i++) {
		regs[i].setFont(font);
		regs[i].setCharacterSize(24);
		regs[i].setFillColor(sf::Color::Black);
		regs[i].setPosition(0, (float)(i * 20));

		keyboard[i].setFont(font);
		keyboard[i].setCharacterSize(24);
		keyboard[i].setFillColor(sf::Color::Black);
		keyboard[i].setPosition(160, (float)(i * 25 + 150));
	}

	PC.setFont(font);
	PC.setCharacterSize(24);
	PC.setFillColor(sf::Color::Black);
	PC.setPosition(160, 0);

	indexReg.setFont(font);
	indexReg.setCharacterSize(24);
	indexReg.setFillColor(sf::Color::Black);
	indexReg.setPosition(160, 30);

	SP.setFont(font);
	SP.setCharacterSize(24);
	SP.setFillColor(sf::Color::Black);
	SP.setPosition(160, 60);

	delayTimer.setFont(font);
	delayTimer.setCharacterSize(24);
	delayTimer.setFillColor(sf::Color::Black);
	delayTimer.setPosition(160, 90);

	soundTimer.setFont(font);
	soundTimer.setCharacterSize(24);
	soundTimer.setFillColor(sf::Color::Black);
	soundTimer.setPosition(160, 120);

}

void Window_SFML::createPixels() {
	//create pixels

	float x = 0;
	float y = 0;

	float xoff = RESIZE_SCREEN_WIDTH;
	float yoff = RESIZE_SCREEN_HEIGHT;

	for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {

		pixels[i].setSize(sf::Vector2f(xoff, yoff));
		pixels[i].setPosition(x, y);
		pixels[i].setFillColor(sf::Color::Black);
		x += xoff;

		if (x >= SCREEN_WIDTH * RESIZE_SCREEN_WIDTH) {
			x = 0;
			y += yoff;
		}
	}

}

void Window_SFML::createWindows() {

	m_window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH * RESIZE_SCREEN_WIDTH, SCREEN_HEIGHT * RESIZE_SCREEN_HEIGHT), "SFML works!");
	m_window->setPosition(sf::Vector2i(300, 0));

	m_windowInfo = new sf::RenderWindow(sf::VideoMode(300, 600), "window info");
	m_windowInfo->setPosition(sf::Vector2i(0, 0));
}

Window_SFML::Window_SFML(std::string CURRENT_DIRECTORY, std::string romFile,emulatorType type): chip_8(romFile,type) {

	this->CURRENT_DIRECTORY = CURRENT_DIRECTORY;

	createWindows();
	createPixels();
	createTexts();
	createSound();

	flag = 0;
	
}

#pragma endregion

void Window_SFML::getInputs() {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
		keys[0] = true;
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
		keys[1] = true;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
		keys[2] = true;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4))
		keys[3] = true;

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
		keys[4] = true;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		keys[5] = true;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
		keys[6] = true;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
		keys[7] = true;


	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		keys[8] = true;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		keys[9] = true;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		keys[10] = true;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F))
		keys[11] = true;


	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
		keys[12] = true;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
		keys[13] = true;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
		keys[14] = true;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V))
		keys[15] = true;
}

void Window_SFML::updateKeys() {
	for (char i = 0; i < 16; i++){
		keys[i] = chip_8.keyMap[i];
	}
}

void Window_SFML::reFreshScreen() {

	for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {

		if (chip_8.display[i] == 0x00) {
			pixels[i].setFillColor(sf::Color::Black);

		}
		else if ((chip_8.display[i] & 0x80 ) == 0x80) {

			uint8_t color = chip_8.display[i] & 0x0f;
			
			switch (color)
			{
			case 0x00:
				pixels[i].setFillColor(sf::Color::White);
				break;

			case 0x01:
				pixels[i].setFillColor(sf::Color::Red);
				break;

			case 0x02:
				pixels[i].setFillColor(sf::Color::Green);			
				break;

			case 0x03:
				pixels[i].setFillColor(sf::Color::Blue);
				break;

			case 0x04:
				pixels[i].setFillColor(sf::Color::Cyan);
				break;
			
			case 0x05:
				pixels[i].setFillColor(sf::Color::Yellow);
				break;

			case 0x06://orange
				pixels[i].setFillColor(sf::Color(255,165,0));
				break;
			}
			

		}

	}
}

void Window_SFML::clearScreen() {
	for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		pixels[i].setFillColor(sf::Color::Black);
	}
}

void Window_SFML::waitForKeys() {

	while (true) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) { keys[4] = true; break; }
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) { keys[5] = true; break; }
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) { keys[6] = true; break; }
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) { keys[7] = true; break; }

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) { keys[8] = true; break; }
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) { keys[9] = true; break; }
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) { keys[10] = true; break; }
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) { keys[11] = true; break; }

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) { keys[12] = true; break; }
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) { keys[13] = true; break; }
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) { keys[14] = true; break; }
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V)) { keys[15] = true; break; }
	}
}

void Window_SFML::updateChip8() {

	flag = chip_8.update(keys);

	if (flag == 1) {
		clearScreen();

	}
	else if (flag == 2) {
		reFreshScreen();

	}
	else if (flag == 3) {
		waitForKeys();

		chip_8.setFlag(4);
		//execute wait for key command
		flag = chip_8.update(keys);

		
	}
	else if(flag == 5){
		updateKeys();
	}

	if (chip_8.soundTimer > 0) {
		sound.play();
	}

	flag = 0;
}

#pragma region drawCommands

void Window_SFML::drawTexts() {
	for (size_t i = 0; i < 16; i++) {
		regs[i].setString("V" + std::to_string(i) + ":" + chip_8.toHex(chip_8.registerFile[i]));
		m_windowInfo->draw(regs[i]);

		keyboard[i].setString("Key " + rdx::toHex(i) + ":" + std::to_string( keys[i]));
		m_windowInfo->draw(keyboard[i]);
	}

	PC.setString("PC:" + chip_8.toHex(chip_8.programCounter));
	m_windowInfo->draw(PC);

	indexReg.setString("I:" + chip_8.toHex(chip_8.indexRegister));
	m_windowInfo->draw(indexReg);

	SP.setString("SP:" + chip_8.toHex(chip_8.stackPointer));
	m_windowInfo->draw(SP);

	delayTimer.setString("DT:" + chip_8.toHex(chip_8.delayTimer));
	m_windowInfo->draw(delayTimer);
	soundTimer.setString("ST:" + chip_8.toHex(chip_8.soundTimer));
	m_windowInfo->draw(soundTimer);
}

void Window_SFML::drawPixels() {
	for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		m_window->draw(pixels[i]);
	}
}

#pragma endregion

void Window_SFML::run() {

	if (!chip_8.init()) {
		return;
	}

	while (m_window->isOpen())
	{

		sf::Event event;
		while (m_window->pollEvent(event))
		{

			if (event.type == sf::Event::Closed) {
				m_window->close();
				m_windowInfo->close();
			}
		}

		m_window->clear();
		m_windowInfo->clear(sf::Color::Blue);
		//--------------------//

		updateChip8();
		getInputs();
		drawPixels();
		drawTexts();

		//--------------------//
		m_window->display();
		m_windowInfo->display();


	}
}

Window_SFML::~Window_SFML()
{
}