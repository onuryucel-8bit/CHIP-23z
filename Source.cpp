#include "window/Window_SFML.h"
#include "chip8/Chip8.h"

int main(int argc, char* argv[]) {


	// c:/.../file/chip8.exe
	std::string fileDirectory = argv[0];
	std::cout << "argv 0 : " << fileDirectory.substr(0,fileDirectory.length()-10)<< "\n";
	
	fileDirectory = fileDirectory.substr(0, fileDirectory.length() - 10);
	
	std::cout << "===============================\n";
	std::cout << "Welcome to Chip_z23" << "\n";
	std::cout << "===============================\n";
	
	//take args 0 = rom location 1 = emuType

	std::cout << "File name " << argv[1] << "\n";
	
	Window_SFML window(fileDirectory, argv[1], EMU_TEXT_ROM);
	window.run();
	
	

	//if (argc == 3){
	
		
	//std::string rom = "ROMS/PONG.ch8";
	
	//Window_SFML window(rom,EMU_TEXT_ROM);

	//window.run();
	//}

}