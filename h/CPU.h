#ifndef CPU_H
#define CPU_H
#include <string>
#include <iostream>
#include "PPU.h"
class CPU
{
    private:
        unsigned short opcode = 0; //The current opcode being read by the CPU.
        unsigned char memory[4096]; //Represents the memory of the chip8 "system".
        unsigned char V[16]; //8-bit data registers, some instructions use V[16] as a flag.
        unsigned short I; //16-bit register used with several opcodes that involve memory operations.
        unsigned short pc; //program counter has a range from 0x000 to 0xFFF
        unsigned char delayTimer;
        unsigned char soundTimer;
        PPU ppu;
        class Stack
        {
            private:
                unsigned short stack[16];
                unsigned short sp = 0;//tracks the position of the stack
            public:
                void push(unsigned short x)
                {
                    if(sp<16)
                        stack[sp++] = x;
                    else
                        std::cout<<"The Stack Is Full\n";
                }
                unsigned short pop()
                {
                    if(sp > 0)
                        return stack[--sp];
                    
                    std::cout<<"The stack is allready empty\n";
                    return -1;
                }
        }; 

        Stack stack;
    public:
        void romToMemory(std::string romName = "PONG");    
        void runOpcode();
        void loadFont();
        bool power();
        void timerDecrement();
};
#endif