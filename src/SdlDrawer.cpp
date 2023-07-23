#include "SDLDrawer.h"
#include "SDL.h"

#include <stdexcept>
#include <string>
#include <array>
#include <iostream>

SdlDrawer::SdlDrawer()
{
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        throw std::runtime_error{"SDL could not initialize! SDL_Error: " + std::string{SDL_GetError()} + "\n"};
    }
    int SCREEN_WIDTH = 640;
    int SCREEN_HEIGHT = 320;
    //Create window
    window.reset(SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    if(!window)
    {
        throw std::runtime_error{"Window could not be created! SDL_Error: " + std::string{SDL_GetError()} + "\n"};
    }

    // Screen surface is owned by window and should not be freed
    screenSurface = SDL_GetWindowSurface(window.get());
}

SdlDrawer::~SdlDrawer()
{
    SDL_Quit();
}

// TODO template sizes?
void SdlDrawer::draw(const std::array<std::array<uint16_t, 64>, 32>& pixels) const
{
    SDL_Rect rect;

    rect.x = 0;
    rect.y = 0;
    int scale = 10;
    rect.h = 64*scale;
    rect.w = 64*scale;

    for (int i=0; i<64; i++)
    {
        for (int j=0; j<32; j++)
        {
            rect.x = i*scale;
            rect.y = j*scale;
            rect.w = scale;
            rect.h = scale;
            if (pixels[i][j] == 1)
            {
                SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 255, 255, 255));
            }
            else if (pixels[i][j] == 0)
            {
                SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0, 0));
            }
        }
    }
    //SDL_Flip(screenSurface);
    //Update the surface
    SDL_UpdateWindowSurface( window.get() );
}