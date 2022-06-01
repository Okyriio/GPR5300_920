
#pragma once
#include "scene.h"

namespace gpr5300
{

    class Engine
    {
    public:
        Engine(Scene* scene);
        void Run();
        SDL_Window* window_ = nullptr;
    private:
        void Begin();
        void End();
        Scene* scene_ = nullptr;



        SDL_GLContext glRenderContext_{};
    };

} // namespace gpr5300