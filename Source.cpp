#include <iostream>
#include <cinttypes>
#include <chrono>
#include <thread>
#include <sstream>
#include <time.h>
#include <cstdlib>

#include <SFML/Graphics.hpp>
#include "RandomAccessMemory.h"
enum KEYWORDS {
	CLR = 0x00E0,
};

#define RAM_SIZE 4096 
#define PRINT_LINE_JUMP 100

#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32

#define RESIZE_SCREEN_WIDTH 20
#define RESIZE_SCREEN_HEIGHT 20

uint8_t ram[RAM_SIZE];

uint8_t registerFile[16] = {};

uint16_t indexRegister;
uint16_t programCounter;
uint8_t  stackPointer = 0x00;

uint8_t  delayTimer;
uint8_t  soundTimer;

sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH * RESIZE_SCREEN_WIDTH, SCREEN_HEIGHT * RESIZE_SCREEN_HEIGHT), "SFML works!");

sf::RectangleShape pixels[SCREEN_WIDTH * SCREEN_HEIGHT];

uint16_t ramAddr = 0x0052;

//translate decimal to hex
std::string toHex(uint16_t dec) {

	std::stringstream ss;
	ss << "0x" << std::hex << std::uppercase << dec;
	return ss.str();
}

void toBin(unsigned n)
{
	/* step 1 */
	if (n > 1)
		toBin(n / 2);

	/* step 2 */
	std::cout << n % 2;
}

void clear_RAM() {
	for (int i = 0; i < RAM_SIZE; i++) {
		ram[i] = 0x0;
	}
}

void print_RAM() {

	for (int i = 0; i < RAM_SIZE ; i++) {
		
		if (i % PRINT_LINE_JUMP == 0 && i != 0) {
			std::cout << "\n";
		}
		std::cout << std::hex << (int)ram[i] << " :: ";
		
	}
}

void clear_Registers() {

	for (size_t i = 0; i < 16; i++){
		registerFile[i] = 0;
	}

	programCounter = 0;

	delayTimer = 0;
	soundTimer = 0;
}

std::pair<uint8_t, uint8_t> getAddress(uint16_t instruction) {

	std::pair<uint8_t, uint8_t> address;

	return address;
}

void executeCommand(uint16_t instruction) {

	std::pair< uint16_t, uint16_t> regAddress;

	uint16_t instructionGroup = 0;
	uint16_t registerX = 0;
	uint16_t registerY = 0;
	uint8_t value = 0;
	//looking for far left bit
	switch (instruction & 0xf000)
	{
		case 0x0000:
					
			//RET — 00EE
		    if (instruction == 0x00EE) {
				programCounter = ram[stackPointer] & 0x00ff;
				stackPointer -= 0x01;
				programCounter = ram[stackPointer] & 0xff00;
				stackPointer -= 0x01;
			}

		break;

		//JMP — 1NNN
		case 0x1000:
			std::cout << "JMP 1NNN\n";
			programCounter = instruction & 0x0fff;
			break;
		//CALL NNN — 2NNN
		case 0x2000:
			std::cout << "CALL 2NNN\n";
			ram[stackPointer] = programCounter & 0x00ff;
			stackPointer += 0x01;
			ram[stackPointer] = programCounter & 0xff00;
			stackPointer += 0x01;

			programCounter = instruction & 0x0fff;

			break;
		//SE VX, NN — 3XNN
		case 0x3000:
			std::cout << "SE 3XNN\n";
			registerX = instruction & 0x0f00;
			registerX = registerX >> 8;
			value = instruction & 0x00ff;
			
			if (registerFile[registerX] == value) {
				programCounter += 0x02;
			}
			break;
		case 0x4000:
			std::cout << "SE 4XNN\n";
			//SNE VX, NN — 4XNN
			registerX = instruction & 0x0f00;
			value = instruction & 0x00ff;

			if (registerFile[registerX] != value) {
				programCounter += 0x02;
			}
			break;
		case 0x5000:
			//SE VX, VY — 5XY0
			std::cout << "SE 5XY0\n";
			registerX = instruction & 0x0f00;
			registerY = instruction & 0x00f0;

			if (registerFile[registerX] == registerFile[registerY]) {
				programCounter += 0x02;
			}
			break;
		case 0x6000:
			//LD VX, NN — 6XNN
			std::cout << "LD 6XNN instruction = " << toHex(instruction) << "\n";
			
			registerX = instruction & 0x0f00;
			registerX = registerX >> 8;

			value     = instruction & 0x00ff;

			registerFile[registerX] = value;

			break;

		//ADD VX, NN — 7XNN
		case 0x7000:
			std::cout << "ADD 7XNN " << toHex(instruction) << "\n";
			registerX = instruction & 0x0f00;
			registerX = registerX >> 8;
			value = instruction & 0x00ff;

			registerFile[registerX] = registerFile[registerX] + value;

		break;

		
#pragma region 0X8000
		case 0x8000:
	
			instructionGroup = instruction & 0x000f;

			switch (instructionGroup){

				//LD VX, VY — 8XY0
				case 0x0000:
					registerX = instruction & 0x0f00;
					registerX = registerX >> 8;
					registerY = instruction & 0x00f0;
					registerY = registerY >> 4;

					registerFile[registerX] = registerFile[registerY];
				break;
				//OR VX, VY — 8XY1
				case 0x0001:
					std::cout << "OR 8XY1 " << toHex(instruction) << "\n";
					 registerX = instruction & 0x0f00;
					 registerX = registerX >> 8;
					 
					 registerY = instruction & 0x00f0;
					 registerY = registerY >> 4;
					 
					registerFile[registerX] = registerFile[registerX] | registerFile[registerY];
				break;
				//AND VX, VY — 8XY2
				case 0x0002:
					 std::cout << "AND 8XY2 " << toHex(instruction) << "\n";
					 registerX = instruction & 0x0f00;
					 registerX = registerX >> 8;
					 registerY = instruction & 0x00f0;
					 registerY = registerY >> 4;

					registerFile[registerX] &= registerFile[registerY];
				break;
				//XOR VX, VY — 8XY3
				case 0x0003:
					 std::cout << "XOR 8XY3 " << toHex(instruction) << "\n";
					 registerX = instruction & 0x0f00;
					 registerX = registerX >> 8;
					 registerY = instruction & 0x00f0;
					 registerY = registerY >> 4;

					registerFile[registerX] ^= registerFile[registerY];
				break;
				//ADD VX, VY — 8XY4
				case 0x0004:
					std::cout << "ADD 8XY4 " << toHex(instruction) << "\n";
					registerX = instruction & 0x0f00;
					registerX = registerX >> 8;
					registerY = instruction & 0x00f0;
					registerY = registerY >> 4;

					if (registerFile[registerX] + registerFile[registerY] > 0xff) {
						registerFile[0xf] = 1;
					}
					else{
						registerFile[0xf] = 0;
					}
					registerFile[registerX] += registerFile[registerY];
				break;
				//SUB VX, VY — 8XY5
				case 0x0005:
					 registerX = instruction & 0x0f00;
					 registerY = instruction & 0x00f0;

					registerFile[registerX] -= registerFile[registerY];
				break;
				//SHR VX {, VY} — 8XY6
				//SUBN VX, VY — 8XY7
				//SHL VX {, VY} — 8XYE

			}
		#pragma endregion
			

		break;
		case 0x9000:
			//SNE VX, VY — 9XY0
			registerX = instruction & 0x0f00;
			registerY = instruction & 0x00f0;

			if (registerFile[registerX] > registerFile[registerY]){
				programCounter += 0x02;
			}
		break;
		case 0xA000:
			//LD I, NNN — ANNN
			indexRegister = instruction & 0x0fff;
		break;
		
		case 0xB000:
			//JMP V0, NNN — BNNN
			programCounter = registerFile[0] + (instruction & 0x0fff);
		break;
		case 0xC000:
			//RND VX, NN – CXNN
			registerX = instruction & 0x0f00;
			value     = instruction & 0x00ff;

			//TODO random
			registerFile[registerX] = value;
		break;
		case 0xD000:
		{
			//DRW VX, VY, N — DXYN
			registerX = instruction & 0x0f00;
			registerY = instruction & 0x00f0;
			value = instruction & 0x000f;

			//index = y * width + x

			int index = registerFile[registerY] * SCREEN_WIDTH + registerFile[registerX];

			pixels[index].setFillColor(sf::Color::White);
		}
		break;
		case 0xE000:
			//SKP VX — EX9E
			//SKNP VX — EXA1
		break;

		case 0xF000:
			//LD VX, DT — FX07
			//LD VX, K — FX0A
			//LD DT, VX — FX15
			//LD ST, VX — FX18
			//ADD I, VX — FX1E
			//LD F, VX — FX29
			//LD B, VX — FX33
			//LD [I], VX — FX55
			//LD VX, [I] — FX65
		break;
	}
}

void test_ProgramRam(KEYWORDS opcode) {
	ram[ramAddr] = (opcode & 0xff00) >> 8;
	ramAddr++;
	ram[ramAddr] = opcode & 0x00ff;
	ramAddr++;
}

void test_ProgramRam(uint16_t opcode) {
	ram[ramAddr] = (opcode & 0xff00) >> 8;
	ramAddr++;
	ram[ramAddr] = opcode & 0x00ff;
	ramAddr++;
}

void createPixels() {
	float x = 0;
	float y = 0;
	//SCREEN_WIDTH* SCREEN_HEIGHT
	srand((unsigned int)(time(0)));
	float xoff = RESIZE_SCREEN_WIDTH;
	float yoff = RESIZE_SCREEN_HEIGHT;
	//RESIZE_SCREEN_HEIGHT 20

	for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {

		pixels[i].setSize(sf::Vector2f(xoff, yoff));
		pixels[i].setPosition(x, y);
		pixels[i].setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
		x += xoff;
		if (x >= SCREEN_WIDTH * RESIZE_SCREEN_WIDTH) {
			x = 0;
			y += yoff;
		}
	}

}

void updateChip8(uint16_t instruction) {
	if (delayTimer > 0)
		delayTimer -= 0x01;

	if (soundTimer > 0)
		soundTimer -= 0x01;

	if (programCounter != 0xffff) {

		instruction = ram[programCounter];
		programCounter += 1;
		instruction = instruction << 8;
		instruction += ram[programCounter];
		programCounter += 1;

		//CLS - 00E0
		if (instruction == 0x00E0) {
			std::cout << "CLS\n";

			for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
				pixels[i].setFillColor(sf::Color::Black);
			}
		}
		else {
			executeCommand(instruction);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

int main() {

	//RandomAccessMemory t;
	//t.loadFromFile();

	clear_RAM();
	clear_Registers();

	uint16_t instruction = 0;

	programCounter = 0x50;

	//CLR
	test_ProgramRam(CLR);
	//v0 = 1;
	test_ProgramRam(0x600f);
	//v1 = 2;
	test_ProgramRam(0x61ff);
	
	test_ProgramRam(0x8014);
	
	

	
	uint8_t char_ROM[] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80, // F
	};

	for (size_t i = 0; i < 80; i++){
		ram[i] = char_ROM[i];
	}


	//load rom test to ram
#pragma region init_stuff



	sf::RenderWindow windowInfo(sf::VideoMode(400,500), "window info");
	windowInfo.setPosition(sf::Vector2i(0, 0));

	sf::Font font;
	if (!font.loadFromFile("Retro Gaming.ttf")) {
		std::cout << "ERROR :: failed to load font \n";
	}

	sf::Text registeFile_text[16];
	
	for (size_t i = 0; i < 16; i++){

		registeFile_text[i].setFont(font);
		
		registeFile_text[i].setString("V" + std::to_string(i) + ":" + toHex(registerFile[i]));
		registeFile_text[i].setPosition(0, (float)(i * 20));
	
		registeFile_text[i].setCharacterSize(24);
		registeFile_text[i].setFillColor(sf::Color::Black);
	
	}
	
	sf::Text programCounter_text;
	programCounter_text.setFont(font);
	programCounter_text.setPosition(150, 0);
	programCounter_text.setCharacterSize(24);
	programCounter_text.setFillColor(sf::Color::Black);

	sf::Text stackPointer_text;
	stackPointer_text.setFont(font);
	stackPointer_text.setPosition(150, 40);
	stackPointer_text.setCharacterSize(24);
	stackPointer_text.setFillColor(sf::Color::Black);

	sf::Text ramPointer_text;
	ramPointer_text.setFont(font);
	ramPointer_text.setPosition(150, 80);
	ramPointer_text.setCharacterSize(24);
	ramPointer_text.setFillColor(sf::Color::Black);
	
	createPixels();

#pragma endregion

	while (window.isOpen())
	{
		
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed){
				window.close();
				windowInfo.close();
			}
		}

		window.clear();
		windowInfo.clear(sf::Color::Blue);
		//--------------------//
#pragma region Draw
	
		//draw
		for (size_t i = 0; i < 16; i++){
			
			registeFile_text[i].setString("V" + std::to_string(i) + ":" + toHex(registerFile[i]));
			
			windowInfo.draw(registeFile_text[i]);
		}


		programCounter_text.setString("PC:" + toHex(programCounter));
		windowInfo.draw(programCounter_text);

		stackPointer_text.setString("SP:"+toHex(stackPointer));
		windowInfo.draw(stackPointer_text);

		ramPointer_text.setString("RAM[PC]" + toHex(ram[programCounter]));
			
		windowInfo.draw(ramPointer_text);


		for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++){
			window.draw(pixels[i]);
		}
#pragma endregion
		//--------------------//
		window.display();
		windowInfo.display();

		updateChip8(instruction);

	}
}