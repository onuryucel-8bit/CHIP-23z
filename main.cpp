#include "window/Window_SFML.h"
#include "chip8/Chip8.h"

emulatorType strToEnum(std::string str) {
	if (str == "EMU_BINARY_CLASSIC_ROM")return EMU_BINARY_CLASSIC_ROM;
	else if (str == "EMU_BINARY_ROM")return EMU_BINARY_ROM;
	else if (str == "EMU_TEXT_ROM")return EMU_TEXT_ROM;
	else return EMU_EMPTY;
}

int main(int argc, char* argv[]) {


	// c:/.../file/chip8.exe
	std::string fileDirectory = argv[0];
	std::cout << "argv 0 : " << fileDirectory.substr(0,fileDirectory.length()-10)<< "\n";	
	fileDirectory = fileDirectory.substr(0, fileDirectory.length() - 10);
	
	std::cout << "===============================\n";
	std::cout << "Welcome to Chip_z23" << "\n";
	std::cout << "===============================\n";
	
	std::cout << "========EMU TYPES=====\n";
	std::cout << "EMU_BINARY_CLASSIC_ROM\n";
	std::cout << "EMU_BINARY_ROM ! not writed\n";
	std::cout << "EMU_TEXT_ROM\n";
	std::cout << "========================\n";

	Window_SFML window(fileDirectory);

	//while (true) {

		std::cout << "Enter emu type\n";

		std::string strEmu;
		//std::cin >> strEmu;

		if (strEmu == "exit" || strEmu == "EXIT") {
			//break;
		}

		std::cout << "file name with extension\n";
		std::string fileName;
		//std::cin >> fileName;

		strEmu = "EMU_TEXT_ROM";
		fileName = "./ROMs/TankSprite.mch8";
		

		emulatorType emutype = strToEnum(strEmu);

		if (emutype != EMU_EMPTY && emutype != EMU_BINARY_ROM) {

			

			window.setEmuType(emutype);
			window.setFileName(fileName);
			window.run();
		}
	//}

}