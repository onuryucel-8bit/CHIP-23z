#include "window/Window_SFML.h"
#include "chip8/Chip8.h"


int main(int argc, char* argv[]) {

	std::cout << "===============================\n";
	std::cout << "Welcome to Chip_z23" << "\n";
	std::cout << "===============================\n";

	//if (argc == 3){
		
	std::string rom = "ROMS/MovingSprite.och8";
	
	Window_SFML window(rom,CHIP_Z23);

	window.run();
	//}

}