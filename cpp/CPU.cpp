#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "CPU.h"
#include "PPU.h"

/*
0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
0x200-0xFFF - Program ROM and work RAM
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::timerDecrement()
{
    if(delayTimer > 0)
        delayTimer--;
    if(soundTimer > 0)
        soundTimer--;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::loadFont()
{
    unsigned char fonts[]={
                        0xF0,//0
                        0x90,
                        0x90,
                        0x90,
                        0xF0,

                        0x20,//1
                        0x60,
                        0x20,
                        0x20,
                        0x70,

                        0xF0,//2
                        0x10,
                        0xF0,
                        0x80,
                        0xF0,

                        0xF0,//3
                        0x10,
                        0xF0,
                        0x10,
                        0xF0,

                        0x90,//4
                        0x90,
                        0xF0,
                        0x10,
                        0x10,

                        0xF0,//5
                        0x80,
                        0xF0,
                        0x10,
                        0xF0,

                        0xF0,//6
                        0x80,
                        0xF0,
                        0x90,
                        0xF0,

                        0xF0,//7
                        0x10,
                        0x20,
                        0x40,
                        0x40,

                        0xF0,//8
                        0x90,
                        0xF0,
                        0x90,
                        0xF0,

                        0xF0,//9
                        0x90,
                        0xF0,
                        0x10,
                        0xF0,

                        0xF0,//A
                        0x90,
                        0xF0,
                        0x90,
                        0x90,

                        0xE0,//B
                        0x90,
                        0xE0,
                        0x90,
                        0xE0,

                        0xF0,//C
                        0x80,
                        0x80,
                        0x80,
                        0xF0,

                        0xE0,//D
                        0x90,
                        0x90,
                        0x90,
                        0xE0,

                        0xF0,//E
                        0x80,
                        0xF0,
                        0x80,
                        0xF0,

                        0xF0,//F
                        0x80,
                        0xF0,
                        0x80,
                        0x80};

    for(int i = 0; i < (5*16); i++)
    {
        memory[i] = fonts[i]; 
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::romToMemory(std::string romName)
{
    romName = "Games\\" + romName;

    std::ifstream rom;
    rom.open(romName, std::ios::binary|std::ios::in|std::ios::ate);
    int romSize = rom.tellg();
    rom.close();
    rom.open(romName, std::ios::binary|std::ios::in);
    
    for(int i = 0x200; i < 4096; memory[i++] = 0);//Initializes all the memory addresses to 0

    for(int i = 0x200; i <= 0xfff && rom.read((char*)(&memory[i]), 1); i++);//Reads in the rom file into memory.

    pc = 0x200;

    loadFont();
    ppu.createWindow();
    rom.close();
  
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CPU::power()
{
    return ppu.running();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*For Opcode Reference go to http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#Ex9E */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::runOpcode()
{
    //takes in the opcode and returns the last x-y bits(x-exclusive, y-inclusive)
    //(ex:opcodeRange(0xABCD, 4, 8) = C)
    auto opcodeRange = [](unsigned short opcode, short y, short x){return (((opcode >> x) << x) ^ opcode) >> y;};

    bool collision = false;
    opcode = (memory[pc] << 8) +memory[pc+1];
    unsigned short x = opcodeRange(opcode, 8, 12);
    unsigned short y = opcodeRange(opcode, 4, 8);

    pc+=2;

    std::cout<<std::hex<<opcode;

    ppu.events();

    switch(opcode >> 12)
    {
        case 0x0:
            switch(opcodeRange(opcode, 0, 12)) 
            {
                case 0x0E0:  //CLS
                    ppu.clear();
                    ppu.initPixels();
                    break;
                case 0x0EE:  //RET
                    pc = stack.pop();
                    break;
                default:
                    std::cout<<" dosent exist";
                    break;
            }
            break;
        case 0x1:  //1nnn - JP addr
            pc = (opcode & 0x0FFF);
            std::cout<<std::hex<<" pc: "<<pc<<" ";
            break;
        case 0x2:  //2nnn - CALL addr
            stack.push(pc);
            pc = (opcode & 0x0FFF);
            break;
        case 0x3:  //3xkk - SE Vx, byte
            if(V[x] == (opcode & 0x00FF))
                pc+=2;
            break;
        case 0x4:  //4xkk - SNE Vx, byte
            if(V[x] != (opcode & 0x00FF))
                pc+=2;
            break;
        case 0x5:  //5xy0 - SE Vx, Vy
            if(V[x] == V[y])
                pc+=2;
            break;
        case 0x6:  //6xkk - LD Vx, byte
            V[x] = (opcode & 0x00FF);
            break;
        case 0x7:  //7xkk - ADD Vx, byte
            V[x] += (opcode & 0x00FF);
            break;
        case 0x8:  //8xy#
            switch(opcodeRange(opcode, 0, 4)) 
            {
                case 0x0:  //LD Vx, Vy
                    V[x] = V[y];
                    break;
                case 0x1:  //OR Vx, Vy
                    V[x] = (V[x] | V[y]);
                    break;
                case 0x2:  //AND Vx, Vy
                    V[x] = (V[x] & V[y]);
                    break;
                case 0x3:  //XOR Vx, Vy
                    V[x] = (V[x] ^ V[y]);
                    break;
                case 0x4:  //ADD Vx, Vy
                    V[0xF] = ((V[x] + V[y]) > 255);
                    V[x] = (V[x] + V[y]) & 0xFF;
                    break;
                case 0x5:  //SUB Vx, Vy MIGHT BE FLAWED
                    V[0xF] = (V[x] > V[y]);
                    V[x] -= V[y];
                    break;
                case 0x6:  //SHR Vx {, Vy}
                    V[0xF] = (V[x] & 1);
                    
                    V[x] = (V[y] >> 1);
                    break;
                case 0x7:  //SUBN Vx, Vy
                    V[0xF] = (V[y] > V[x]);
                    V[x] = V[y] - V[x];
                    break;
                case 0xE:  //SHL Vx {, Vy}
                    V[0xF] = (V[x] >> 7);
                    //V[x] *= V[y];
                    V[0xF] = (V[x] << 1);
                    break;
                default:
                    std::cout<<" dosent exist";
                    break;
            }
            break;
        case 0x9:  //9xy0 - SNE Vx, Vy
            if(V[x] != V[y])
                pc+=2;
            break;
        case 0xA:  //Annn - LD I, addr
            I = (opcode & 0x0FFF);
            break;
        case 0xB:  //Bnnn - JP V0, addr 
            pc = ((opcode & 0x0FFF) + V[0]);
            break;
        case 0xC:  //Cxkk - RND Vx, byte
            srand(time(NULL));
            V[x] = ((rand()%256) & (opcode & 0x00FF));
            break;
        case 0xD:  //Dxyn - DRW Vx, Vy, nibble
            collision = false;
            
            for(int n = 0; n < (opcode & 0xF); n++)
            {
                collision = ppu.setPixel(V[x], V[y]+n, memory[I+n]);
                if(!power())
                    
                    std::cout<<"\nIm turning off " << n;
            }
            
            V[0xF] = collision;
            ppu.render();
            break;
        case 0xE: //Ex##
            switch(opcodeRange(opcode, 0, 8))
            {
                case 0x9E:  //SKP Vx
                    if(ppu.hasKey(V[x]))
                        pc+=2;
                    break;
                case 0xA1:  //SKNP Vx
                    if(!ppu.hasKey(V[x]))
                        pc+=2;
                    break;
                default:
                    std::cout<<" dosent exist";
                    break;
            }
            break;
        case 0xF:  //Fx##
            switch(opcodeRange(opcode, 0, 8))
            {
                
                case 0x07:  //LD Vx, DT
                    V[x] = delayTimer;
                    break;
                case 0x0A:  //LD Vx, K
                    while(!ppu.keyPressedThisCycle())
                        ppu.events();
                    V[x] = ppu.getLastKeyPressed();
                    break;
                case 0x15:  //LD DT, Vx
                    delayTimer = V[x];
                    break;
                case 0x18:  //LD ST, Vx
                    soundTimer = V[x];
                    break;
                case 0x1E:  //ADD I, Vx
                    I += V[x];
                    break;
                case 0x29:  //LD F, Vx
                    I = V[x] * 5;
                    break;
                case 0x33:  //LD B, Vx
                    memory[I] = V[x]/100;
                    memory[I+1] = (V[x]%100)/10;
                    memory[I+2] = (V[x]%10);
                    //I+=I+2;
                    break;

                /*
                NOTE: opcodes 0xFX55 and 0xFX65 may or may not need to change register I
                EXAMPLE Pong works when you change the program counter but TICTAC dosent,
                however if you dont change the program counter then tictac works. This is
                due to two different common implementations of chip8. I will need to 
                implement a user setting for this later.
                */
                case 0x55:  //LD [I], Vx
                    for(int i = 0; i <= x; i++)
                        memory[I+i] = V[i];
                    I+=x+1;
                    break;
                case 0x65:  //LD Vx, [I]
                    for(int i = 0; i <= x; i++)
                        V[i] = memory[I+i];
                    I+=x+1;
                    break;
                default:
                    std::cout<<" dosent exist";
                    break;
            }
            break;
            default:
                std::cout<<" dosent exist";
                break;
    }
    std::cout<<"\n";
}