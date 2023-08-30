#pragma once

#include<iostream>
#include<vector>
#include<fstream>
#include<string>

#include<cstdint>

#include "../utils/Radix.h"

class romLoaderCH8 {

public:
	romLoaderCH8(std::string pathname,uint16_t ramSize);

	int initLoader();

	int load_ClassicChip8ROM(uint8_t* ram);

	int getCharsetData(std::vector<uint8_t>* output_och8);

	int getVariables(std::vector<uint8_t>* output_och8);

	int getCodeSection(std::vector<uint8_t>* output_och8, int ramAddress);

private:
	
	std::string path;
	std::ifstream file;

	uint16_t ramSize;
};

