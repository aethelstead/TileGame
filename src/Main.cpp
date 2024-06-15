#include "GameApp.h"

#ifdef _WIN32
    #define SDL_MAIN_HANDLED
#endif

int main(int argc, char* argv[])
{
    // Suppress any unused variable warnings
    static_cast<void>(argc);
	static_cast<void>(argv);

    // Allocated on the heap to avoid stack overflow warning.
    auto pGameApp = std::make_unique<Gin::GameApp>();
    if (!pGameApp->Init())
    {
        LOGINFO("Failed to initialise GameApp. Exiting...");
        return 1;
    }

    pGameApp->Loop();

    pGameApp->Quit();

    return 0;
}
