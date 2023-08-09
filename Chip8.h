#pragma once

#include <chrono>
#include <thread>
#include <cstdint>

#include "Compiler_chip8.h"


#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32

#define RESIZE_SCREEN_WIDTH 20
#define RESIZE_SCREEN_HEIGHT 20

class Chip8
{
 public:
	 Chip8();
	~Chip8();
	
	uint16_t instruction;

	uint8_t registerFile[16] = {};

	uint16_t indexRegister;
	uint16_t programCounter;

	uint16_t stackPointer;

	uint8_t delayTimer;
	uint8_t soundTimer;
	
	uint8_t ram[0xfff];//4095 0xfff

	uint8_t display[SCREEN_WIDTH * SCREEN_HEIGHT];

	void update();

	void executeCommand();

private:

	void CLR();

	void RET();
	void JMP();
	void CALL();
	void CE();//SE //CONST EQUAL to reg
	
	void CNE();//SNE //const not equal to reg
	void LDC();//LOAD CONST
	void ADDC();//ADD const
	void OR();

	void AND();
	void XOR();
	void ADDR();//add reg
	void SUBX();

	void SHR();
	void SUBY();
	void SHL();
	void RNE();//reg not equal

	void JMPZ();//jump with zero reg
	void LDI();//load index reg
	void RND();
	void DRW();

	void SKP();//skip if key pressed
	void SKNP();//skip if key not pressed
	void LDD();//load delay timer
	void WFK();//wait for key

	void LDRD();//load reg to delay timer
	void LDRS();//load reg to sound timer
	void ADDI();//index reg + Vx
	void LD_FX29();

	void LD_FX33();
	void LD_FX55();
	void LD_FX65();

};