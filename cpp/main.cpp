//6.3.0
#include "PPU.h"
#include "CPU.h"
#include<iostream>
#include<chrono>

int main( int argc, char* args[]) 
{ 
	CPU cpu;
	//PPU ppu;
	cpu.romToMemory("UFO");
	//ppu.loadFont();
	typedef std::chrono::steady_clock Time;
    typedef std::chrono::milliseconds ms;

    Time::time_point opStart;
	Time::time_point opEnd;

	Time::time_point timerStart;
	Time::time_point timerEnd;
	
	int opElapsed;
	int timerElapsed;
	//ppu.createWindow();
	opStart = Time::now();
	timerStart = Time::now();
	while(cpu.power())
	{	
		timerEnd = Time::now();
		timerElapsed = int(std::chrono::duration_cast<ms>(timerEnd-timerStart).count());

		opEnd = Time::now();
		opElapsed = int(std::chrono::duration_cast<ms>(opEnd-opStart).count());
		
		//if(timerElapsed >= 1000)
		//{
			cpu.timerDecrement();
			timerStart = Time::now();
		//}

		if(opElapsed >= 2)
		{
			cpu.runOpcode();
			opStart = Time::now();
		}
		
	}
 	return 0;   
}