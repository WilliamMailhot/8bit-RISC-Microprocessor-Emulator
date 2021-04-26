#pragma once

#include <iostream>
#include <vector>


class CPU
{
private:
	uint16_t OPCODE;
	uint16_t PC;
        uint8_t A;
        std::vector<uint16_t> blocked;
	uint8_t memory[0xFFF];

public:
	void startCPU();
	bool load(const char* path);
	void cycle();
};
