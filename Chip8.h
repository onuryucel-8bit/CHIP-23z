#pragma once

#include <cstdint>

#include "RandomAccessMemory.h"

class Chip8
{
public:
	Chip8();
	~Chip8();

	uint8_t registerFile[16] = {};
	
	uint16_t indexRegister;
	uint16_t programCounter;
	
	uint8_t stackPointer;

	uint8_t delayTimer;
	uint8_t soundTimer;

	RandomAccessMemory memory;

	void executeCommand();

private:

	

};

Chip8::Chip8()
{
}

void Chip8::executeCommand() {

}

Chip8::~Chip8()
{
}