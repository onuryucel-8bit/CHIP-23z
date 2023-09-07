#include "window/Window_SFML.h"
#include "chip8/Chip8.h"


int main(int argc, char* argv[]) {

	std::cout << "===============================\n";
	std::cout << "Welcome to Chip_z23" << "\n";
	std::cout << "===============================\n";
	
	//take args 0 = rom location 1 = emuType
	/*
	if(argv[0] == TEXT_ROM || argv[0] == 2)
	if(argv[0] == BINARY_TEXT_ROM || argv[0] == 1)
	*/
	//if (argc == 3){
		
	std::string rom = "ROMS/MovingSprite.och8";
	
	Window_SFML window(rom,TEXT_ROM);

	window.run();
	//}

}