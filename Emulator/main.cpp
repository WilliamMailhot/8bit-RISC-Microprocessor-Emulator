#include "CPU.h"

CPU emu;

int main()
{
    const char *path = "PATH_TO_ROM";

    emu.startCPU();
    emu.load(path);


    while (true)
    {
        emu.cycle();
    }
}
