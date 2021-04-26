#include "CPU.h"
void CPU::startCPU()
{
	OPCODE = 0; // Reset OPCODE
	PC = 0x100;
	A = 0; // Reset stack pointer

	std::fill_n(memory, 4096, 0);
}

bool CPU::load(const char *path)
{

    FILE * rom = fopen(path, "r");

	if (rom == NULL)
	{
		return false;
	}

	fseek(rom, 0, SEEK_END);
	long rom_size = ftell(rom);
	rewind(rom);

	char* rom_buffer = new char[sizeof(char) * rom_size]; // Allocate memory for ROM


	size_t result = fread(rom_buffer, sizeof(char), (size_t)rom_size, rom); // Write ROM to allocated buffer

	if (result != rom_size)
	{
		std::cout << "Something went wrong...";
		return false;
	}

	for (int i = 0; i < rom_size; i++)
	{
	    memory[i + 0x100] = (uint8_t)rom_buffer[i];
	}


	fclose(rom);
	delete[] rom_buffer;

	return true;
}

void CPU::cycle()
{
	OPCODE = memory[PC] << 8 | memory[PC + 1];

	//std::cout << "Current Instruction: " <<std::hex << OPCODE << "\n";

	switch (OPCODE & 0xF000)
    {
        case 0x0000:
            switch (OPCODE & 0x0F00)
            {
                case 0x0000:
                    A += (OPCODE & 0x00FF);
                    PC += 2;
                    break;

                case 0x0100:
                    A = (OPCODE & 0x00FF);
                    PC += 2;
                    break;

                case 0x0200:
                    A ^= (OPCODE & 0x00FF);
                    PC += 2;
                    break;

                case 0x0300:
                    A |= (OPCODE & 0x00FF);
                    PC += 2;
                    break;

                case 0x0400:
                    A &= (OPCODE & 0x00FF);
                    PC += 2;
                    break;

                default:
                    A--;
                    PC += 2;
                    break;
            }
            break;

        case 0x8000:
            A = memory[(OPCODE & 0x0FFF)];
            PC += 2;
            break;

        case 0xD000:
            if (!(std::find(blocked.begin(), blocked.end(), (OPCODE & 0x0FFF)) != blocked.end()))
            {
                memory[(OPCODE & 0x0FFF)] ^= A;
            }
            PC += 2;
            break;

        case 0xF000:
            if (!(std::find(blocked.begin(), blocked.end(), (OPCODE & 0x0FFF)) != blocked.end()))
            {
                memory[(OPCODE & 0x0FFF)] = A;
            }
            PC +=2;
            break;

        case 0x2000:
            PC = (OPCODE & 0x0FFF);
            break;

        case 0x3000:
            if (A == 0)
            {
                PC = (OPCODE & 0x0FFF);
            }
            else {PC += 2;}
            break;

        case 0x4000:
            if (A == 1)
            {
                PC = (OPCODE & 0x0FFF);
            }
            else {PC += 2;}
            break;

        case 0x5000:
            if (A == 0xFF)
            {
                PC = (OPCODE & 0x0FFF);
            }
            else {PC += 2;}
            break;

        case 0x6000:
            if (A == (OPCODE & 0x00FF)) {
                A = 0;
            }
            else if (A < (OPCODE & 0x00FF)) {
                A = 0x1;
            }
            else if (A > (OPCODE & 0x00FF)) {
                A = 0xFF;
            }
            PC += 2;
            break;

        case 0x7000:
            if (A == memory[(OPCODE & 0x0FFF)])
            {
                A = 0;
            }
            else if (A < memory[(OPCODE & 0x0FFF)])
            {
                A = 0x1;
            }
            else if (A > memory[(OPCODE & 0x0FFF)])
            {
                A = 0xFF;
            }
            PC += 2;
            break;

        case 0x1000:
            if (OPCODE == 0x1337)
            {
                std::cout << A << "\n";
            }
            else
            {
                A--;
            }
            PC += 2;
            break;

        case 0xB000:
            if (OPCODE == 0xBEEF)
            {
                PC = 0x100;
                A = 0x42;
            }
            else
            {
                A--;
                PC += 2;
            }
            break;

        case 0x9000:
            if (!(std::find(blocked.begin(), blocked.end(), (OPCODE & 0x0FFF)) != blocked.end()))
            {
                blocked.push_back((OPCODE & 0x0FFF));
            }
            PC += 2;
            break;

        case 0xA000:
            for (auto i = blocked.begin(); i != blocked.end(); ++i) {
                if (*i == (OPCODE & 0x0FFF)) {
                    blocked.erase(i);
                    i--;
                }
            }
            PC += 2;
            break;

        case 0xC000:
            if (!(std::find(blocked.begin(), blocked.end(), (OPCODE & 0x0FFF)) != blocked.end()))
            {
                memory[(OPCODE & 0x0FFF)] ^= 0x42;
            }
            PC += 2;
            break;

        case 0xE000:
            if (OPCODE != 0xEEEE)
            {
                A--;
            }
            PC += 2;
            break;

        default:
            A--;
            PC += 2;
            break;

    }
}