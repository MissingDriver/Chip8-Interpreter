#ifndef PPU_H
#define PPU_H
#include "SDL2/SDL.h" 
class PPU
{
    private:
        SDL_Window  *window;
        SDL_Renderer *renderer;
        int mult = 20;
        const static int w = 64;
        const static int h = 32;
        bool pixels[w][h];
        bool isRunning = false;
        bool keys[16] = {false}; //Keeps tracks of keys pressed(chip8 key values are 0x0-0xF)
        bool keyPressed = false;
        unsigned char lastKeyPressed = 0;
    public:
        void initPixels();
        void clear();
        void createWindow();
        void events();
        bool setPixel(int x, int y, unsigned char pixelMap);
        void render();
        void destroy();
        bool running();
        bool hasKey(int key);
        bool keyPressedThisCycle();
        unsigned char getLastKeyPressed();
};
#endif