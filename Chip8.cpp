#include "Chip8.h"


Chip8::Chip8() {

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

	ramAddr = 0;

	size_t arrayLength = sizeof(char_ROM) / sizeof(char_ROM[0]);

	//loading ROM
	for (size_t i = 0; i < arrayLength; i++){
		ram[i] = char_ROM[i];
	}

	programCounter = (uint16_t)arrayLength;
	ramAddr = programCounter;
	
	stackPointer = 0xffe;
	indexRegister = 0;

	soundTimer = 10;

	ch8flag = 0;

	uint16_t spriteIndex = 0xA000;
	uint16_t spriteX = 0x6000;
	uint16_t spriteY = 0x6100;
	//I = 5 load spr 1

	for (size_t i = 0; i < 17; i++) {

		test_OpcodeLoader(spriteIndex);
		//V0 = 2
		test_OpcodeLoader(spriteX);
		//V1 = 5
		test_OpcodeLoader(spriteY);
		//DRW
		test_OpcodeLoader(0xD010);

		spriteX += 0x0005;
		if (spriteX > 0x6028) {
			spriteX = 0x6000;
			spriteY += 0x0006;
		}

		spriteIndex += 0x0005;
	}

	test_OpcodeLoader(0xF50A);

	
}


//translate decimal to hex
std::string Chip8::toHex(uint16_t dec) {

	std::stringstream ss;
	ss << std::hex << std::uppercase << dec;
	return ss.str();
}

void Chip8::setFlag(uint8_t flag) {
	ch8flag = flag;
}

void Chip8::test_OpcodeLoader(uint16_t opcode) {

	ram[ramAddr] = (opcode & 0xff00) >> 8;
	ramAddr++;
	ram[ramAddr] = opcode & 0x00ff;
	ramAddr++;

}

//FETCH
int Chip8::update(bool keys[16]) {

	std::this_thread::sleep_for(std::chrono::milliseconds(CHIP8_SLEEP_TIME));
	
	if (soundTimer != 0) {
		soundTimer--;
	}

	if (delayTimer != 0) {
		delayTimer--;
	}

	for (size_t i = 0; i < 16; i++){
		keyMap[i] = keys[i];
	}

	//execute wait for keys command
	if (ch8flag == 4) {
		uint16_t regX = instruction & 0x0f00;
		regX >>= 8;
		
		for (size_t i = 0; i < 16; i++){
			
			if (keyMap[i]) {
				registerFile[regX] = (uint8_t)(i);
			}
		}
		

		ch8flag = 0;

	
	}
	else{
		instruction = ram[programCounter];
		programCounter += 1;
		instruction = instruction << 8;
		instruction += ram[programCounter];
		programCounter += 1;

		executeCommand();
	}

	return ch8flag;
}

//DECODE => calls execute(opcode) functions
void Chip8::executeCommand() {

	uint16_t instructionGroup;

	//take the MSB
	switch (instruction & 0xf000)
	{
	case 0x0000: 
		if (instruction == 0x00E0)CLR();
		else if (instruction == 0x00EE)RET();
		break;
	case 0x1000:
		if (instruction == 0x00EE)JMP_1nnn();
		break;
		
	case 0x2000:
		CALL_2nnn();
		break;

	case 0x3000:
		CE_3xnn();
		break;

	case 0x4000:
		CNE_4xnn();
		break;

	case 0x5000:
		RE_5xy0();
		break;

	case 0x6000:
		LDC_6xnn();
		break;

	case 0x7000:
		ADDC_7xnn();
		break;


#pragma region 0X8000
	case 0x8000:

		instructionGroup = instruction & 0x000f;

		switch (instructionGroup) {

			// LD VX, VY — 8XY0
		case 0x0000:
			MOV_8xy0();
			break;

			// OR VX, VY — 8XY1
		case 0x0001:
			OR_8xy1();
			break;

			// AND VX, VY — 8XY2
		case 0x0002:
			AND_8xy2();
			break;

			// XOR VX, VY — 8XY3
		case 0x0003:
			XOR_8xy3();
			break;

			// ADD VX, VY — 8XY4
		case 0x0004:
			ADDR_8xy4();
			break;

			// SUB VX, VY — 8XY5
		case 0x0005:
			SUBX_8xy5();
			break;

			// SHR VX {, VY} — 8XY6
		case 0x0006:
			SHR_8xy6();
			break;

			// SUBN VX, VY — 8XY7
		case 0x0007:
			SUBY_8xy7();
			break;

			// SHL VX {, VY} — 8XYE
		case 0x000E:
			SHL_8xyE();
			break;

		}

	break;
#pragma endregion	

	// SNE VX, VY — 9XY0
	case 0x9000:
		RNE_9xy0();
		break;

		// LD I, NNN — ANNN
	case 0xA000:
		LDI_Annn();
		break;

		// JMP V0, NNN — BNNN
	case 0xB000:
		JMPZ_Bnnn();
		break;

		// RND VX, NN – CXNN
	case 0xC000:
		RND_Cxnn();
		break;

		// DRW VX, VY, N — DXYN
	case 0xD000:
		// Implementation...
		DRW_Dxyn();
		break;

		// SKP VX — EX9E
	case 0xE000:
		SKP_9x9E();
		break;

		// LD VX, DT — FX07
		// LD VX, K — FX0A
		// ... more cases ...
	case 0xF000:
		if ((instruction & 0x000A) == 0x000A) {
			WFK();
		}
		break;

	}
}

void Chip8::CLR() {
	for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++){
		display[i] = 0;
	}

	ch8flag = 1;
}

void Chip8::RET() {
	programCounter = ram[stackPointer];
}

#pragma region Jumps

void Chip8::JMP_1nnn() {
	programCounter = instruction & 0x0fff;
}

#pragma endregion

void Chip8::CALL_2nnn(){
	ram[stackPointer] = programCounter & 0xff0; 
	stackPointer += 1;
	ram[stackPointer] = programCounter & 0x00f;
}

void Chip8::CE_3xnn() {

	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	if (registerFile[regX] == (instruction & 0x00ff)) {
		programCounter += 2;
	}
}

void Chip8::CNE_4xnn(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	if (registerFile[regX] != (instruction & 0x00ff)) {
		programCounter += 2;
	}
}

void Chip8::RE_5xy0() {
	
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;
	uint16_t regY = instruction & 0x00f0;
	regX >>= 4;

	if (registerFile[regX] == registerFile[regY]) {
		programCounter += 2;
	}
}

void Chip8::LDC_6xnn() {
	uint16_t regX = instruction & 0x0f00;
	regX = regX >> 8;

	uint8_t value = instruction & 0x00ff;

	registerFile[regX] = value;
	std::cout << "LDC 6XNN V" <<toHex(regX)<<":" << toHex(registerFile[regX]) << "\n";
}

void Chip8::ADDC_7xnn(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint8_t value = instruction & 0x00ff;

	registerFile[regX] += value;
}

void Chip8::MOV_8xy0() {
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint16_t regY = instruction & 0x00f0;
	regY >>= 4;

	registerFile[regX] = registerFile[regY];
}

void Chip8::OR_8xy1(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint16_t regY = instruction & 0x00f0;
	regY >>= 4;

	registerFile[regX] |= registerFile[regY];
}

void Chip8::AND_8xy2(){

	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint16_t regY = instruction & 0x00f0;
	regY >>= 4;

	registerFile[regX] &= registerFile[regY];
}

void Chip8::XOR_8xy3(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint16_t regY = instruction & 0x00f0;
	regY >>= 4;

	registerFile[regX] ^= registerFile[regY];
}

void Chip8::ADDR_8xy4() {
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint16_t regY = instruction & 0x00f0;
	regY >>= 4;

	if (registerFile[regX] + registerFile[regY] > 0xff) {
		registerFile[0xf] = 1;
	}else{
		registerFile[0xf] = 0;
	}

	registerFile[regX] += registerFile[regY];
}

void Chip8::SUBX_8xy5(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint16_t regY = instruction & 0x00f0;
	regY >>= 4;

	if (registerFile[regX] > registerFile[regY]) {
		registerFile[0xf] = 1;
	}
	else {
		registerFile[0xf] = 0;
	}

	registerFile[regX] -= registerFile[regY];
}

void Chip8::SHR_8xy6(){

	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	registerFile[regX] >>= 1;
	registerFile[0xf] = regX & 0x01;
}

void Chip8::SUBY_8xy7(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint16_t regY = instruction & 0x00f0;
	regY >>= 4;

	if (registerFile[regX] > registerFile[regY]) {
		registerFile[0xf] = 1;
	}
	else {
		registerFile[0xf] = 0;
	}

	registerFile[regY] -= registerFile[regX];
}

void Chip8::SHL_8xyE(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	registerFile[regX] <<= 1;
	registerFile[0xf] = regX & 0x80;
}

void Chip8::RNE_9xy0(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint16_t regY = instruction & 0x00f0;
	regY >>= 4;

	if (registerFile[regX] != registerFile[regY]) {
		programCounter += 2;
	}

}

void Chip8::LDI_Annn(){
	indexRegister = instruction & 0x0fff;
}

void Chip8::JMPZ_Bnnn(){
	programCounter = registerFile[0] + (instruction & 0x0fff);
}

void Chip8::RND_Cxnn(){

}

void Chip8::DRW_Dxyn()
{
	std::cout << "DRW_DXYN" << toHex(instruction) << "\n";
	std::cout << "V0_x" << toHex(registerFile[0]) << "\n";
	std::cout << "V1_y" << toHex(registerFile[1]) << "\n";

	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint16_t regY = instruction & 0x00f0;
	regY >>= 4;

	uint8_t value = instruction & 0x000f;

	//std::cout << "DRW DXYN " << toHex(instruction) << "\n";
	////DRW VX, VY, N — DXYN
	
	uint16_t Vx = registerFile[regX];
	uint16_t Vy = registerFile[regY];

	///index = y * width + x
	uint8_t colorChecker = 0x80;
	uint16_t ramSpritePos = indexRegister;

	for (size_t i = 0; i < 5 * 8; i++) {

		size_t startingPos = Vy * SCREEN_WIDTH + Vx;

		if ((ram[ramSpritePos] & colorChecker) == colorChecker) {

			display[startingPos] = 1;
			//pixels[startingPos].setFillColor(sf::Color::White);
		}
		
		colorChecker >>= 1;
		Vx += 1;
		if (colorChecker == 0) {
			colorChecker = 0x80;
			ramSpritePos += 1;
			Vx = registerFile[regX];
			Vy += 1;
		}

	}
	
	ch8flag = 2;
}

void Chip8::SKP_9x9E(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint8_t Vx = registerFile[regX];
	//0x00
	Vx &= 0x0f;

	if (keyMap[Vx]) {
		programCounter += 2;
		keyMap[Vx] = false;
	}
}

void Chip8::SKNP(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint8_t Vx = registerFile[regX];
	//0x00
	Vx &= 0x0f;

	if (!keyMap[Vx]) {
		programCounter += 2;		
	}
}

void Chip8::LDD(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;
	registerFile[regX] = delayTimer;
}

void Chip8::WFK(){
	ch8flag = 3;
}

void Chip8::LDRD_Fx15(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;
	delayTimer = registerFile[regX];
}

void Chip8::LDRS_Fx18(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;
	soundTimer = registerFile[regX];
}

void Chip8::ADDI_Fx1E(){

	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	indexRegister += registerFile[regX];
}

void Chip8::LD_FX29(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	indexRegister = registerFile[regX];
}

void Chip8::LD_FX33(){
	//BCD
}

void Chip8::LDRR_FX55(){
	
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	for (uint16_t i = 0; i < regX; i++){
		ram[indexRegister + i] = registerFile[i];
	}
}

void Chip8::LD_FX65(){

	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	for (uint16_t i = 0; i < regX; i++) {
		registerFile[i] = ram[indexRegister + i];
	}
}

Chip8::~Chip8() {}
