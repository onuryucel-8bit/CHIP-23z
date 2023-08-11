#pragma once

#include <chrono>
#include <thread>
#include <cstdint>
#include <sstream>

#include "Compiler_chip8.h"


#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32

#define RESIZE_SCREEN_WIDTH 20
#define RESIZE_SCREEN_HEIGHT 20

#define CHIP8_SLEEP_TIME 500

class Chip8
{
 public:
	 Chip8();
	~Chip8();
	
	uint8_t registerFile[16] = {};

	uint16_t indexRegister;
	uint16_t programCounter;

	uint16_t stackPointer;

	uint8_t display[SCREEN_WIDTH * SCREEN_HEIGHT];

	uint8_t soundTimer;
	uint8_t delayTimer;

	/*
	*  FLAGS:
	*   0 : clear screen
	* 
	*/
	int update(bool keys[16]);

	std::string toHex(uint16_t dec);

	bool keyMap[16] = {};

	void setFlag(uint8_t flag);

private:

	uint8_t ch8flag;

	uint16_t ramAddr;

	uint16_t instruction;
	uint8_t ram[0xfff] = {};//4095 0xfff

	

	void test_OpcodeLoader(uint16_t opcode);

	void executeCommand();

	

	void CLR();

	void RET();
	void JMP_1nnn();
	void CALL_2nnn();
	void CE_3xnn();//SE //CONST EQUAL to reg
	
	void CNE_4xnn();//SNE //const not equal to reg
	void RE_5xy0();//reg equals
	void LDC_6xnn();//LOAD CONST
	void ADDC_7xnn();//ADD const
	void MOV_8xy0();//LD 8XY0
	void OR_8xy1();

	void AND_8xy2();
	void XOR_8xy3();
	void ADDR_8xy4();//add reg
	void SUBX_8xy5();

	void SHR_8xy6();
	void SUBY_8xy7();
	void SHL_8xyE();
	void RNE_9xy0();//reg not equal


	void LDI_Annn();//load index reg
	void JMPZ_Bnnn();//jump with zero reg	
	void RND_Cxnn();
	void DRW_Dxyn();

	void SKP_9x9E();//skip if key pressed
	void SKNP();//skip if key not pressed
	void LDD();//load delay timer
	void WFK();//wait for key

	void LDRD_Fx15();//load reg to delay timer
	void LDRS_Fx18();//load reg to sound timer
	void ADDI_Fx1E();//index reg + Vx
	void LD_FX29();

	void LD_FX33();
	void LDRR_FX55();//LOAD reg to ram
	void LD_FX65();
	
};