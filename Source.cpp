#include "Window_SFML.h"
#include "Chip8.h"


int main(int argc, char* argv[]) {

	std::cout << "===============================\n";
	std::cout << "Welcome to Chip8" << "\n";
	std::cout << "===============================\n";

	//if (argc == 3){
		
	std::string rom = "charROM.txt";
	std::string ram = "Source.och8";
		Window_SFML window(rom, ram);

		window.run();
	//}

}