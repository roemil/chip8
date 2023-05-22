#pragma once

#include "IDrawer.h"

#include "SDL.h"
#include <memory>

class SdlDrawer : public IDrawer
{
    public:
    SdlDrawer();
    ~SdlDrawer() override;

    void draw() const override;

    private:
        struct WindowDestroyer
        {
            void operator()(SDL_Window* w) const
            {
                SDL_DestroyWindow(w);
            }
        };
        std::unique_ptr<SDL_Window, WindowDestroyer> window = nullptr;
        SDL_Surface* screenSurface = nullptr;
};