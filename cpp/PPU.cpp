#include"PPU.h"
#include <SDL2/SDL.h> 
#include "iostream"

///////////////////////////////////////////////////////Initialiizes the Window and Renderer///////////////////////////////////////////////////////////////////
void PPU::createWindow()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
	   window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w*mult, h*mult, 0); //create the window

	if(window==NULL) //check for window errors
	{       
      printf("Could not create window: %s\n", SDL_GetError());
      return;
	}

    renderer = SDL_CreateRenderer(window, -1, 0); //Create the renderer

    if(renderer == NULL) //check for renderer errors
    {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return;
    }
    
    isRunning = true; //if everything has been created properly then set isRunning equal to true
    initPixels();
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}   
///////////////////////////////////////////////////////////Initializes Pixels///////////////////////////////////////////////////////////////	
void PPU::initPixels()
{
    for(int i = 0; i < w; i++)
    {        
        for(int j = 0; j < h; j++)
            pixels[i][j] = false;
    }
}
///////////////////////////////////////////////////////////handles events///////////////////////////////////////////////////////////////	
void PPU::events()
{
    SDL_Event event;
    SDL_PollEvent(&event); //checks the event type
    
   keyPressed = false; 

   char hold = lastKeyPressed;

    switch(event.type)
    {
        case SDL_QUIT: //on quit stop running
            isRunning = false;
            std::cout<<"Quit";
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    isRunning = false;
                    std::cout<<"Escape";
                    break;
                case SDLK_1:
                    keys[0x1] = true;
                    lastKeyPressed = 0x1;
                    break;
                 case SDLK_2:
                    keys[0x2] = true;
                    lastKeyPressed = 0x2;
                    break;
                 case SDLK_3:
                    keys[0x3] = true;
                    lastKeyPressed = 0x3;                    
                    break;
                  case SDLK_4:
                    keys[0xC] = true;
                    lastKeyPressed = 0xC;
                    break;
                 case SDLK_q:
                    keys[0x4] = true;
                    lastKeyPressed = 0x4;                    
                    break;
                 case SDLK_w:
                    keys[0x5] = true;
                    lastKeyPressed = 0x5;                    
                    break;
                 case SDLK_e:                    
                    keys[0x6] = true;
                    lastKeyPressed = 0x6;
                    break;
                  case SDLK_r:                    
                    keys[0xD] = true;
                    lastKeyPressed = 0xD;
                    break;
                 case SDLK_a:                    
                    keys[0x7] = true;
                    lastKeyPressed = 0x7;
                    break;
                 case SDLK_s:                    
                    keys[0x8] = true;
                    lastKeyPressed = 0x8;
                    break;
                 case SDLK_d:                    
                    keys[0x9] = true;
                    lastKeyPressed = 0x9;
                    break;
                  case SDLK_f:                    
                    keys[0xE] = true;
                    lastKeyPressed = 0xE;
                    break;
                 case SDLK_z:                    
                    keys[0xA] = true;
                    lastKeyPressed = 0xA;
                    break;
                 case SDLK_x:                    
                    keys[0x0] = true;
                    lastKeyPressed = 0x0;
                    break;
                 case SDLK_c:                    
                    keys[0xB] = true;
                    lastKeyPressed = 0xB;
                    break;
                  case SDLK_v:                    
                    keys[0xF] = true;
                    lastKeyPressed = 0xF;
                    break;
            }
            break;

            case SDL_KEYUP:
            switch(event.key.keysym.sym)
            {
                case SDLK_1:
                    keys[0x1] = false;
                    break;
                 case SDLK_2:
                    keys[0x2] = false;
                    break;
                 case SDLK_3:
                    keys[0x3] = false;                    
                    break;
                  case SDLK_4:
                    keys[0xC] = false;
                    break;
                 case SDLK_q:
                    keys[0x4] = false;                    
                    break;
                 case SDLK_w:
                    keys[0x5] = false;                    
                    break;
                 case SDLK_e:                    
                    keys[0x6] = false;
                    break;
                  case SDLK_r:                    
                    keys[0xD] = false;
                    break;
                 case SDLK_a:                    
                    keys[0x7] = false;
                    break;
                 case SDLK_s:                    
                    keys[0x8] = false;
                    break;
                 case SDLK_d:                    
                    keys[0x9] = false;
                    break;
                  case SDLK_f:                    
                    keys[0xE] = false;
                    break;
                 case SDLK_z:                    
                    keys[0xA] = false;
                    break;
                 case SDLK_x:                    
                    keys[0x0] = false;
                    break;
                 case SDLK_c:                    
                    keys[0xB] = false;
                    break;
                  case SDLK_v:                    
                    keys[0xF] = false;
                    break;
            }
            break;
    }

    if(hold != lastKeyPressed)
      keyPressed = true;
}
///////////////////////////////////////////////////////////Returns the last key that was pressed///////////////////////////////////////////////////////////////
unsigned char PPU::getLastKeyPressed()
{
   return lastKeyPressed;
}
///////////////////////////////////////////////////////////Checks if a key has been pressed this cycle///////////////////////////////////////////////////////////////
bool PPU::keyPressedThisCycle()
{ 
   return keyPressed;
}
///////////////////////////////////////////////////////////Checks to see if a key has been pressed///////////////////////////////////////////////////////////////
bool PPU::hasKey(int key)
{
    return keys[key];
}
////////////////////////////////////////////////////////Creates the pixels to be drawn to the ppu//////////////////////////////////////////////////////////////////
bool PPU::setPixel(int x, int y, unsigned char pixelMap)
{
   SDL_Rect *pixel = new SDL_Rect;
   bool collision = false;
   bool oldState; //checks the previous state of the pixel
   
   int X = 0;
   int Y = 0;

   if(y > h)
         Y = ((y%h)-1);
      else
         Y = y;

   for(int i = 0; i < 8; i++)
   {
      if(x+i > w) 
         X = (((x+i)%w)-1);
      else
         X = (x+i);

      oldState = pixels[X][Y]; 

      pixels[X][Y] = pixels[X][Y] ^ ((pixelMap >> (7-i)) & 1); //A pixels state is determined by the XOR of its current state and its corresponding new map
      
      if(!collision && oldState && !pixels[X][Y]) //If a pixel collision has been detected, then the function returns the information to the CPU
         collision = false;
      pixel->x = X*mult;
      pixel->y = Y*mult;

      pixel->w = 1*mult;
      pixel->h = 1*mult;

      if(pixels[X][Y])
         SDL_SetRenderDrawColor(renderer,255,255,255,255);
      else
         SDL_SetRenderDrawColor(renderer,0,0,0,0);

      SDL_RenderFillRect(renderer, pixel);
      
   }
   delete pixel;
   pixel = nullptr;
   return collision;
}
////////////////////////////////////////////////////////handles rendering//////////////////////////////////////////////////////////////////
void PPU::render()
{
    //SDL_RenderClear(renderer); //clear the renderer
    SDL_RenderPresent(renderer); //updates the ppu with the current state of the renderer
}
void PPU::clear()
{
    initPixels();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer); //clear the renderer
}
///////////////////////////////////////////////////removes everything///////////////////////////////////////////////////////////////////////
void PPU::destroy   ()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
//////////////////////////////////////////////////////checks if the game is running////////////////////////////////////////////////////////////////////
bool::PPU::running()
{
    return isRunning;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////