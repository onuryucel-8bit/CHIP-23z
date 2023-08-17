#pragma once

#include <iostream>

//for sleep
#include <chrono>
#include <thread>

//for uint8/16 types
#include <cstdint>

//for toHex func
#include <sstream>

//file reading
#include <fstream>
#include <string>

//for RND opcode
#include <random>

//file reading
#include <vector>

#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32

#define RESIZE_SCREEN_WIDTH 20
#define RESIZE_SCREEN_HEIGHT 20

#define CHIP8_SLEEP_TIME 500

class Chip8
{
 public:
	 Chip8();
	 Chip8(std::string romFile, std::string ramFile);
	~Chip8();
	
	uint8_t registerFile[16] = {};

	uint16_t indexRegister;
	uint16_t programCounter;

	uint8_t stackPointer;

	uint8_t display[SCREEN_WIDTH * SCREEN_HEIGHT];

	uint8_t soundTimer;
	uint8_t delayTimer;


	/*
	*  FLAGS:
	*   0 : nothing
	*   1 : clear screen
	*   2 : DRW
	*   3 : wait for keys WFK
	*   4 : KEY pressed for WFK return flag
	*/
	int update(bool keys[16]);

	std::string toHex(uint16_t dec);

	uint8_t toDec(std::string hex);

	std::string toBin(int n);

	bool keyMap[16] = {};

	void setFlag(uint8_t flag);

	std::vector<uint8_t>* loadMachineCode_fromFile(std::string path, std::string type);

	char inputCh8Flag;

private:

	uint8_t stack[256] = {};

	uint8_t ch8flag;

	uint8_t inputVx;

	uint16_t ramAddr;

	uint16_t instruction;
	uint8_t ram[0xfff] = {};//4095 0xfff

	//void test_OpcodeLoader(uint16_t opcode);

	void executeCommand();

	void CLR_00E0();
	void RET_00EE();
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
	void SKP_Ex9E();//skip if key pressed

	void SKNP_ExA1();//skip if key not pressed
	void LDD_Fx07();//load delay timer
	void WFK_Fx0A();//wait for key
	void LDRD_Fx15();//load reg to delay timer

	void LDRS_Fx18();//load reg to sound timer
	void ADDI_Fx1E();//index reg + Vx
	void IIR_FX29();//increase index reg 

	void DTB_FX33();//dec to bcd DTB
	void STR_FX55();//STORE
	void CFR_FX65();//copy from ram CFR
	
};