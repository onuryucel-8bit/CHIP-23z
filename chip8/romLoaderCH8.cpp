#include "romLoaderCH8.h"

romLoaderCH8::romLoaderCH8(std::string pathname,uint16_t ramSize){
	path = pathname;
	this->ramSize = ramSize;

	if (ramSize < 0xfff) {
		std::cout << "ERROR : ramSize not enough ! \n";
	}

	file.open(path, std::ios::in);

	if (!file.is_open()) {
		std::cout << "ERROR : cannot opened ROM file" << __LINE__ << "\n" << __FILEW__ << "\n";
		return;
	}
}

int romLoaderCH8::load_ClassicChip8ROM(uint8_t* ram) {
	
	file.close();

	file.open(path,std::ios::binary | std::ios::ate);
	
	std::streampos size = file.tellg();
	char* buffer = new char[size];
	file.seekg(0, std::ios::beg);
	file.read(buffer, size);
	file.close();

	for (long i = 0; i < size; ++i)
	{
		ram[0x200 + i] = buffer[i];
	}

	delete[] buffer;

	file.close();

	return 0;
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
	
	if (line.substr(0,4) != "VARB") {
		std::cout << "ERROR : getVariables() : Theres NOT begining of VAR_SECTION_BEGIN keyword \n";
		return -1;
	}
	
	bool sectionEnding = false;
	size_t lineIndex = std::strlen("VARB") + 1;

	size_t varLength;

	do
	{

		while (lineIndex < line.length()) {

			if (line.substr(lineIndex, 15) == "VARE") {

				sectionEnding = true;
				break;
			}

			int varEndIndex = lineIndex;
			varLength = 0;
			while (true){
				
				if (line.substr(varEndIndex,1) == ",") {
					
					break;
				}
				varLength++;
				varEndIndex++;
			}

			(*output_och8)[ramAddress] = std::stoi(line.substr(lineIndex, varLength));
			ramAddress--;
			lineIndex += varLength + 1;//varLength points the "," thats why 1 added
		}

		lineIndex = 0;
		

	} while (!sectionEnding && std::getline(file,line));

	return 0;
}

int romLoaderCH8::getCodeSection(std::vector<uint8_t>* output_och8, int ramAddress) {
	
	std::string line;

	std::getline(file, line);

	//code begin
	if (line.substr(0,5) != "CODEB") {
		std::cout << "ERROR : getCodeSection : Theres NOT begining of CODEB keyword \n";
		return -1;
	}

	bool sectionEnding = false;
	size_t lineIndex = std::strlen("CODEB") + 1;

	do
	{

		while (lineIndex < line.length()) {

			if (line.substr(lineIndex, 5) == "CODEF") {

				sectionEnding = true;
				break;
			}

			(*output_och8)[ramAddress] = rdx::toDec(line.substr(lineIndex, 4));
			ramAddress++;
			lineIndex += 5;
		}

		lineIndex = 0;
		
	} while (!sectionEnding && std::getline(file, line));
	

	//close file
	file.close();

	return ramAddress;
}