#include <sstream>

#include "Window_SFML.h"
#include "Chip8.h"

bool keys[16] = {};

void toBin(unsigned n)
{
	/* step 1 */
	if (n > 1)
		toBin(n / 2);

	/* step 2 */
	std::cout << n % 2;
}

void clearPixels(sf::RectangleShape* pixels) {

	for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++){
		pixels[i].setFillColor(sf::Color::Black);
	}
}

void reFreshScreen(sf::RectangleShape* pixels, uint8_t* display) {

	for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		
		if (display[i] == 0) {
			pixels[i].setFillColor(sf::Color::Black);
			
		}
		else if (display[i] == 1) {
			pixels[i].setFillColor(sf::Color::White);
			
		}
		
	}
}

int main() {
	
	Window_SFML window;

	window.run();
	
	////Window_SFML window;
	//Chip8 chip_8;

	//sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH * RESIZE_SCREEN_WIDTH, SCREEN_HEIGHT * RESIZE_SCREEN_HEIGHT), "SFML works!");
	//sf::RenderWindow windowInfo(sf::VideoMode(400,500), "window info");
	//
	//windowInfo.setPosition(sf::Vector2i(0, 0));

	//sf::Font font;
	//if (!font.loadFromFile("Retro Gaming.ttf")) {
	//	std::cout << "ERROR :: failed to load font \n";
	//}

	//sf::Text regs[16],PC_text,indexReg;

	//for (size_t i = 0; i < 16; i++){
	//	regs[i].setFont(font);
	//	regs[i].setCharacterSize(24);
	//	regs[i].setFillColor(sf::Color::Black);
	//	regs[i].setPosition(0, (float)(i * 20));
	//}
	//

	//PC_text.setFont(font);
	//PC_text.setCharacterSize(24);
	//PC_text.setFillColor(sf::Color::Black);
	//PC_text.setPosition(160, 0);

	//indexReg.setFont(font);
	//indexReg.setCharacterSize(24);
	//indexReg.setFillColor(sf::Color::Black);
	//indexReg.setPosition(160, 30);

	//sf::SoundBuffer buffer;
	//if (!buffer.loadFromFile("chip8_squareWave.wav")) {
	//	std::cout << "ERROR : failed the load sound file \n";
	//}
	//	
	//sf::Sound sound;
	//sound.setBuffer(buffer);
	//
	//
	////init pixels
	//sf::RectangleShape pixels[SCREEN_WIDTH * SCREEN_HEIGHT];

	//float x = 0;
	//float y = 0;
	////SCREEN_WIDTH* SCREEN_HEIGHT
	//srand((unsigned int)(time(0)));
	//float xoff = RESIZE_SCREEN_WIDTH;
	//float yoff = RESIZE_SCREEN_HEIGHT;
	////RESIZE_SCREEN_HEIGHT 20

	//for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {

	//	pixels[i].setSize(sf::Vector2f(xoff, yoff));
	//	pixels[i].setPosition(x, y);
	//	pixels[i].setFillColor(sf::Color::Black);
	//	x += xoff;
	//	
	//	if (x >= SCREEN_WIDTH * RESIZE_SCREEN_WIDTH) {
	//		x = 0;
	//		y += yoff;
	//	}
	//}

	//uint8_t flag = 0;
	//

	//while (window.isOpen())
	//{
	//
	//	sf::Event event;
	//	while (window.pollEvent(event))
	//	{
	//		if (event.type == sf::Event::Closed){
	//			window.close();
	//			windowInfo.close();
	//		}

	//	}

	//	window.clear();
	//	windowInfo.clear(sf::Color::Blue);
	//	//--------------------//

	//	//---update---//
	//
	//	flag = chip_8.update(keys);

	//	if (flag == 1){
	//		clearPixels(pixels);				

	//	}else if(flag == 2){
	//		reFreshScreen(pixels,chip_8.display);			

	//	}else if (flag == 3) {
	//		waitForKeys();
	//		
	//		chip_8.setFlag(4);
	//		//execute wait for key command
	//		flag = chip_8.update(keys);
	//	}
	//	
	//	if (chip_8.soundTimer > 0) {
	//		sound.play();			
	//	}
	//	

	//	flag = 0;
	//	
	//	//draw Pixels
	//	for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++){
	//		window.draw(pixels[i]);				
	//	}

	//	for (size_t i = 0; i < 16; i++){

	//		regs[i].setString("V"+ chip_8.toHex((uint16_t)(i)) + ":" + chip_8.toHex(chip_8.registerFile[i]));
	//		windowInfo.draw(regs[i]);
	//	}
	//	
	//	PC_text.setString("PC:" + chip_8.toHex(chip_8.programCounter));
	//	indexReg.setString("I:" + chip_8.toHex(chip_8.indexRegister));
	//	
	//	windowInfo.draw(PC_text);
	//	windowInfo.draw(indexReg);

	//	
	//	//--------------------//
	//	window.display();
	//	windowInfo.display();


	//	
	//	//reset keymap
	//	for (size_t i = 0; i < 16; i++) {
	//		keys[i] = false;
	//	}
	//	

	//	
	//	
	//}

}