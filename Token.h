#pragma once

#include<cstdint>

class Token
{
public:
	Token();
	~Token();
	
	enum TOKEN_TYPE {
		CLR = 0x00E0,
		JMP = 0x1000,
		CALL = 0x2000,
		SEC = 0x3000,
		SNE = 0x4000,
		SER = 0x5000,
		LDC = 0x6000,
		ADDC = 0x7000
	};

	uint16_t TokenHexVersion;

	TOKEN_TYPE type;

private:

};

Token::Token()
{
}

Token::~Token()
{
}