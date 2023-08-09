#pragma once

#include<iostream>
#include<fstream>
#include<string>

class RandomAccessMemory {
public:
	void loadFromFile();
	uint8_t memory[200] = {};

private:
	void loadToMemory(std::string source);
	
	enum KEYWORDS {
		CLR = 0x00E0,
		LDC = 0x6000
	};

};

void RandomAccessMemory::loadToMemory(std::string source) {
	size_t endLine = 0;
	while (source.substr(endLine, 1) != ";") {
		endLine++;
	}

	

	std::cout << "END LINE" << endLine << "\n";
}

void RandomAccessMemory::loadFromFile() {
	// Create a text string, which is used to output the text file
	std::string myText;
	std::string output = "";
	// Read from the text file
	std::ifstream MyReadFile("Source.ch8");

	// Use a while loop together with the getline() function to read the file line by line
	while (std::getline(MyReadFile, myText)) {
		// Output the text from the file
		output += myText;
	}

	// Close the file
	MyReadFile.close();

	loadToMemory(output);
}