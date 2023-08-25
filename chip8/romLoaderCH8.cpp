#include "romLoaderCH8.h"

romLoaderCH8::romLoaderCH8(std::string pathname,uint16_t ramSize){
	path = pathname;
	ramSize = ramSize;

	if (ramSize < 0xfff) {
		std::cout << "ERROR : ramSize not enough ! \n";
	}

	file.open(pathname, std::ios::in);

	if (!file.is_open()) {
		std::cout << "ERROR : cannot opened ROM file" << __LINE__ << "\n" << __FILEW__ << "\n";
		return;
	}
}

int romLoaderCH8::getCharsetData(std::vector<uint8_t>* output_och8) {

	int ramAddress = 0;
	std::string line;	

	//TODO make better file input

	//get first line of source.och8
	std::getline(file, line);
	
	if (line.substr(0,4) != "ROMB") {
		std::cout << "ERROR : getCharsetData() : Theres NOT begining of Charset keyword \n";
		return -1;
	}

	//012345 <= index
	//ROMB 0x0f,0xdf,0x2f
	size_t lineIndex = 5;

	bool sectionEnding = false;

	do{
	
		while (lineIndex < line.length()) {

			if (line.substr(lineIndex, 4) == "ROME") {
				
				sectionEnding = true;
				break;
			}
						
			(*output_och8)[ramAddress] = rdx::toDec(line.substr(lineIndex, 4));
			ramAddress++;
			lineIndex += 5;
		}

		lineIndex = 0;

	} while (!sectionEnding && std::getline(file, line));

	return ramAddress;
}

int romLoaderCH8::getVariables(std::vector<uint8_t>* output_och8) {

	int ramAddress = ramSize - 0x1;
	std::string line;
	
	std::getline(file, line);
	
	if (line.substr(0,17) != "VAR_SECTION_BEGIN") {
		std::cout << "ERROR : getVariables() : Theres NOT begining of VAR_SECTION_BEGIN keyword \n";
		return -1;
	}
	
	bool sectionEnding = false;
	int lineIndex = std::strlen("VAR_SECTION_BEGIN") + 2;

	do
	{

		while (lineIndex < line.length()) {

			if (line.substr(lineIndex, 15) == "VAR_SECTION_END") {

				sectionEnding = true;
				break;
			}

			int varEndIndex = 1;			
			while (true){
				
				if (line.substr(varEndIndex,1) == ",") {
					
					break;
				}

				varEndIndex++;
			}

			(*output_och8)[ramAddress] = rdx::toDec(line.substr(lineIndex, varEndIndex - 1));
			ramAddress--;
			lineIndex += varEndIndex;
		}

		lineIndex = 0;
		

	} while (!sectionEnding && std::getline(file,line));

	return 0;
}

int romLoaderCH8::getCodeSection(std::vector<uint8_t>* output_och8, int ramAddress) {

	//size_t lineNumber = ramAddress;
	//std::string line;
	//size_t blankLineIndex;
	//int ramUsed = 0;

	//std::getline(file, line);
	//while (line.empty()) {
	//	std::getline(file, line);
	//}

	//if (line != "CODE_SECTION_BEGIN") {
	//	std::cout << "ERROR : getVariables() : Theres NOT begining of CODE_SECTION_BEGIN keyword \n";
	//	return -1;
	//}

	//while (std::getline(file, line)) {

	//	if (line == "CODE_SECTION_END") {
	//		break;
	//	}
	//	else {

	//		//get first char index of line str
	//		blankLineIndex = line.find_first_not_of(' ') + 1;

	//		while (blankLineIndex < line.length()) {

	//			(*output_och8)[lineNumber] = line.substr(blankLineIndex, 4);
	//			lineNumber++;
	//			blankLineIndex += 5;
	//			ramUsed++;
	//		}
	//	}
	//}

	//if (line != "CODE_SECTION_END") {
	//	std::cout << "ERROR: End of CODE_SECTION_END undefined ! \n";
	//	return -1;
	//}

	//return ramUsed;

	//close file
	file.close();

	return 0;
}