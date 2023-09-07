#include "Chip8.h"

Chip8::Chip8(std::string romFile, emulatorType type) {

	this->emuType = type;
	this->romFile = romFile;
}

bool Chip8::init() {

	std::vector<uint8_t>* ROM = loadMachineCode_fromFile(romFile);

	if (ROM == nullptr) {
		return false;
	}

	if (emuType == BINARY_CLASSIC_ROM)
		programCounter = 0x200;
	else
		programCounter = ramAddr;

	

	stackPointer = 0x0;
	indexRegister = 0;

	soundTimer = 0;

	ch8flag = 0;
	inputCh8Flag = 0;

	if (emuType == TEXT_ROM) {
		for (size_t i = 0; i < ROM_SIZE; i++) {
			ram[i] = (*ROM)[i];
		}
	}

	return true;
}

std::vector<uint8_t>* Chip8::loadMachineCode_fromFile(std::string path) {

	std::cout << "ROM name " << path.substr(5,path.length() - 5) << "\n";

	romLoaderCH8 loader(path, ROM_SIZE);

	if (emuType == TEXT_ROM) {

		if (loader.initLoader() == -1)
			return nullptr;

		std::vector<uint8_t>* ROM = new std::vector<uint8_t>(ROM_SIZE);

		std::cout << "rom cap" << ROM->capacity() << "\n";

		int ramAddress = loader.getCharsetData(ROM);

		loader.getVariables(ROM);

		loader.getCodeSection(ROM, ramAddress);

		ramAddr = ramAddress;

		return ROM;
	}
	else if (emuType == BINARY_CLASSIC_ROM) {

		uint8_t CHARSET[] = {
			0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
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
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
		};

		for (size_t i = 0; i < 80; i++)
		{
			ram[i] = CHARSET[i];
		}

		if (loader.load_ClassicChip8ROM(ram) != 0) {
			std::cout << "ERROR : load Classic Chip8 ROM \n";
		}

		
	}

	return nullptr;
	
}

uint8_t Chip8::toDec(std::string hex){

	//0xFE => FE

	unsigned int dec = std::stoul(hex, nullptr, 16);
	
	return (uint8_t)(dec);
}

std::string Chip8::toBin(int n) {
	std::string bin = "";

	while (n > 1) {

		bin += std::to_string(n % 2);
		n /= 2;
	}

	bin += std::to_string(n);

	std::reverse(bin.begin(), bin.end());

	return bin;
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

//FETCH
int Chip8::update(bool* keys) {

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
	else if (inputCh8Flag == 1) {

		if (keyMap[inputVx]) {
			programCounter += 2;
			keyMap[inputVx] = false;
			ch8flag = 5;
		}

		inputCh8Flag = 0;

	}
	else if (inputCh8Flag == 2) {
		
		if (!keyMap[inputVx]) {
			programCounter += 2;
		}

		inputCh8Flag = 0;
	}
	else{

		executeCommand();
	}

	for (size_t i = 0; i < 16; i++){
		keys[i] = false;
	}

	return ch8flag;
}

//DECODE => calls execute(opcode) functions
void Chip8::executeCommand() {

	instruction = ram[programCounter];
	programCounter += 1;
	instruction = instruction << 8;
	instruction += ram[programCounter];
	programCounter += 1;

	uint16_t instructionGroup;

	//take the MSB
	switch (instruction & 0xf000)
	{
	case 0x0000: 
		if (instruction == 0x00E0)CLR_00E0();
		else if (instruction == 0x00EE)RET_00EE();
		break;
	case 0x1000:
		JMP_1nnn();
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
		SKP_Ex9E();
		SKNP_ExA1();
		
		break;

	case 0xF000:
		
		instructionGroup = instruction & 0x00ff;

		switch (instructionGroup)
		{
		case 0x07:
			LDD_Fx07();
			break;
		case 0x0A:
			WFK_Fx0A();
			break;
		case 0x15:
			LDRD_Fx15();
			break;
		case 0x18:
			LDRS_Fx18();
			break;
		case 0x1E:
			ADDI_Fx1E();
			break;
		case 0x29:
			IIR_FX29();
			break;
		case 0x33:
			DTB_FX33();
			break;
		case 0x55:
			STR_FX55();
			break;
		case 0x65:
			CFR_FX65();
			break;
		}
		
		break;

	}
}

void Chip8::CLR_00E0() {
	for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++){
		display[i] = 0;
	}

	ch8flag = 1;
}

void Chip8::RET_00EE() {
	programCounter = stack[stackPointer];
}

#pragma region Jumps

void Chip8::JMP_1nnn() {
	programCounter = instruction & 0x0fff;
}

#pragma endregion

#pragma region LineA

void Chip8::CALL_2nnn(){
	stack[stackPointer] = programCounter & 0xff0; 
	stackPointer += 1;
	stack[stackPointer] = programCounter & 0x00f;
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

	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint8_t value = instruction & 0x00ff;	

	std::random_device rd;
	std::uniform_int_distribution <int>dist(0,255);

	registerFile[regX] = (uint8_t)(dist(rd)) & value;
	
}

void Chip8::DRW_Dxyn() {

	uint8_t regX = (instruction >> 8) & 0x0f;	
	uint8_t regY = (instruction >> 4) & 0x0f;
	
	uint8_t height = instruction & 0x000f;

	uint8_t Vx = registerFile[regX];
	uint8_t Vy = registerFile[regY];

	if (Vx > SCREEN_WIDTH - 1)
		Vx = 0;
	if (Vy > SCREEN_HEIGHT - 1)
		Vy = 0;

	//std::cout << "vx " << toDec(toHex(Vx)) << " vy " << toDec(toHex(Vy)) << "\n";

	uint8_t spriteByte;
	uint8_t spritePixel;
	uint8_t color;

	uint8_t screenPixel;
	size_t startingPos;

	registerFile[0xf] = 0;

	for (uint8_t row = 0; row < height; row++) {

		spriteByte = ram[indexRegister + row];

		for (uint8_t col = 0; col < 4; col++) {

			startingPos = Vy * SCREEN_WIDTH + Vx;
			
			//let spriteByte = 1001_(4color)
			spritePixel = (spriteByte >> (7 - col)) & 0x01;
			//---//
			screenPixel = display[startingPos] >> 7;

			//check collision
			if ((screenPixel ^ spritePixel) == screenPixel) {
				registerFile[0xf] = 1;
			}

			//             1 = 0^1
			screenPixel = screenPixel ^ spritePixel;

			//copying for color values i cant find logical operation for this
			//11 = 1 / 10 = 0 / 01 = 1 / 00 = 0
			

			screenPixel <<= 7;
			color = spriteByte & 0x0f;

			display[startingPos] = screenPixel | color;
			
			Vx++;	
			if (Vx > 63)
				Vx = 0;
		}
		
		Vx = registerFile[regX];
		Vy++;

		if (Vx > SCREEN_WIDTH - 1)
			Vx = 0;
		if (Vy > SCREEN_HEIGHT - 1)
			Vy = 0;
		
	}
	

	ch8flag = 2;
}
#pragma endregion

void Chip8::SKP_Ex9E(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint8_t Vx = registerFile[regX];
	//0x00
	Vx &= 0x0f;

	if (keyMap[Vx]) {
		programCounter += 2;
		keyMap[Vx] = false;
		ch8flag = 5;

		return;
	}
	inputCh8Flag = 1;

}

void Chip8::SKNP_ExA1(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	uint8_t Vx = registerFile[regX];
	//0x00
	Vx &= 0x0f;

	if (!keyMap[Vx]) {
		programCounter += 2;
	}

	inputCh8Flag = 2;
}

void Chip8::LDD_Fx07(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;
	registerFile[regX] = delayTimer;
}

void Chip8::WFK_Fx0A(){
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

void Chip8::IIR_FX29(){
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	indexRegister = registerFile[regX] + 0x05;
}

//dec number 230 => ram[i] = 2 , ram[i + 1] = 3 ...
void Chip8::DTB_FX33(){

	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	std::string numberStr = std::to_string(registerFile[regX]);

	uint16_t i = 0;
	for (char digitChar : numberStr) {
		int digit = digitChar - '0'; // Convert char to integer
		ram[indexRegister + i] = (uint8_t)(digit);
		i++;
	}
}

void Chip8::STR_FX55(){
	
	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	for (uint16_t i = 0; i < regX; i++){
		ram[indexRegister + i] = registerFile[i];
	}
}

void Chip8::CFR_FX65(){

	uint16_t regX = instruction & 0x0f00;
	regX >>= 8;

	for (uint16_t i = 0; i < regX; i++) {
		registerFile[i] = ram[indexRegister + i];
	}
}

Chip8::~Chip8() {}

