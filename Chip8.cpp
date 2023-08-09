#include "Chip8.h"


Chip8::Chip8() {

	CLR();

	uint8_t char_ROM[] = {					//adr
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0    0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1    5
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2    a
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3    f
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4    
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5    
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6    
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80, // F
		0x90, 0x90, 0xF0, 0x90, 0x90, // H
	};

	stackPointer = 0xffe;

}

//FETCH
void Chip8::update() {

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	instruction = ram[programCounter];
	programCounter += 1;
	instruction = instruction << 8;
	instruction += ram[programCounter];
	programCounter += 1;

	executeCommand();

}

//DECODE => calls execute(opcode) functions
void Chip8::executeCommand() {

	uint16_t instructionGroup;

	//take the MSB
	switch (instruction & 0xf000)
	{
	case 0x0000: 
		if (instruction == 0x00EE)RET();
		break;
	case 0x1000:
		if (instruction == 0x00EE)JMP();
		break;
		
	case 0x2000:
		break;

	case 0x3000:
		break;

	case 0x4000:
		break;

	case 0x5000:
		break;

	case 0x6000:
		break;

	case 0x7000:
		break;


#pragma region 0X8000
	case 0x8000:

		instructionGroup = instruction & 0x000f;

		switch (instructionGroup) {

			// LD VX, VY — 8XY0
		case 0x0000:
			// Implementation...
			break;

			// OR VX, VY — 8XY1
		case 0x0001:
			// Implementation...
			break;

			// AND VX, VY — 8XY2
		case 0x0002:
			// Implementation...
			break;

			// XOR VX, VY — 8XY3
		case 0x0003:
			// Implementation...
			break;

			// ADD VX, VY — 8XY4
		case 0x0004:
			// Implementation...
			break;

			// SUB VX, VY — 8XY5
		case 0x0005:
			// Implementation...
			break;

			// SHR VX {, VY} — 8XY6
		case 0x0006:
			// Implementation...
			break;

			// SUBN VX, VY — 8XY7
		case 0x0007:
			// Implementation...
			break;

			// SHL VX {, VY} — 8XYE
		case 0x000E:
			// Implementation...
			break;

		}

	break;
#pragma endregion	

	// SNE VX, VY — 9XY0
	case 0x9000:
		// Implementation...
		break;

		// LD I, NNN — ANNN
	case 0xA000:
		// Implementation...
		break;

		// JMP V0, NNN — BNNN
	case 0xB000:
		// Implementation...
		break;

		// RND VX, NN – CXNN
	case 0xC000:
		// Implementation...
		break;

		// DRW VX, VY, N — DXYN
	case 0xD000:
		// Implementation...
		break;

		// SKP VX — EX9E
	case 0xE000:
		// Implementation...
		break;

		// LD VX, DT — FX07
		// LD VX, K — FX0A
		// ... more cases ...
	case 0xF000:
		// Implementation...
		break;

	}
}

void Chip8::CLR() {

}

void Chip8::JMP() {

}

void Chip8::CALL()
{
}

void Chip8::CE()
{
}

void Chip8::RET() {

}


void Chip8::CNE()
{
}

void Chip8::LDC()
{
}

void Chip8::ADDC()
{
}

void Chip8::OR()
{
}

void Chip8::AND()
{
}

void Chip8::XOR()
{
}

void Chip8::ADDR(){
}

void Chip8::SUBX()
{
}

void Chip8::SHR()
{
}

void Chip8::SUBY()
{
}

void Chip8::SHL()
{
}

void Chip8::RNE()
{
}

void Chip8::LDI()
{
}

void Chip8::JMPZ()
{
}

void Chip8::RND()
{
}

void Chip8::DRW()
{
	/*
	* std::cout << "DRW DXYN " << toHex(instruction) << "\n";
		//DRW VX, VY, N — DXYN
		regAddress = getAddress(instruction);
		value = instruction & 0x000f;

		uint16_t Vx = registerFile[regAddress.first];
		uint16_t Vy = registerFile[regAddress.second];

		//index = y * width + x
		uint8_t colorChecker = 0x80;
		uint16_t ramSpritePos = indexRegister;

		for (size_t i = 0; i < 5 * 8; i++) {

			size_t startingPos = Vy * SCREEN_WIDTH + Vx;

			if ((ram[ramSpritePos] & colorChecker) == colorChecker) {

				pixels[startingPos].setFillColor(sf::Color::White);
			}


			colorChecker >>= 1;
			Vx += 1;
			if (colorChecker == 0) {
				colorChecker = 0x80;
				ramSpritePos += 1;
				Vx = registerFile[regAddress.first];
				Vy += 1;
			}

		}
	*/
}

void Chip8::SKP()
{
}

void Chip8::SKNP()
{
}

void Chip8::LDD()
{
}

void Chip8::WFK()
{
}

void Chip8::LDRD()
{
}

void Chip8::LDRS()
{
}

void Chip8::ADDI()
{
}

void Chip8::LD_FX29()
{
}

void Chip8::LD_FX33()
{
}

void Chip8::LD_FX55()
{
}

void Chip8::LD_FX65()
{
}

Chip8::~Chip8() {}
