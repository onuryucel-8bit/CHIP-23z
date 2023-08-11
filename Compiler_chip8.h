#pragma once

#include<iostream>
#include<fstream>
#include<string>

/*
class Compiler_chip8 {
public:
	void loadFromFile();
	uint8_t memory[200] = {};

private:
	std::string getToken(std::string source);
	
	size_t endLine = 0;
};

std::string Compiler_chip8::getToken(std::string source) {
	
	std::string token;

	while (source.substr(endLine, 1) != ";") {
		token += source.substr(endLine, 1);
		endLine++;
	}
	
	std::cout << "TOKEN : " << token << "\n";
	std::cout << "END LINE" << endLine << "\n";

	endLine++;

	return token;
}

void Compiler_chip8::loadFromFile() {
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

	while (endLine < output.length()) {
		std::string token = getToken(output);
	}
}*/