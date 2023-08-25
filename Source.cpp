#include "window/Window_SFML.h"
#include "chip8/Chip8.h"


int main(int argc, char* argv[]) {

	std::cout << "===============================\n";
	std::cout << "Welcome to Chip8" << "\n";
	std::cout << "===============================\n";

	//if (argc == 3){
		
	std::string rom = "Source.och8";
	
	Window_SFML window(rom);

	window.run();
	//}

}