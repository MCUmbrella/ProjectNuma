#include <iostream>
#include "ProjectNuma.h"

int main(int argc, char** argv)
{
    if (argc > 1 && strcmp(argv[1], "-d") == 0)
        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
    cerr << endl << VERSION << endl
         << "Made by @MCUmbrella, licensed under MIT" << endl
         << "https://github.com/MCUmbrella/ProjectNuma" << endl << endl;
    unsigned long startTime = CommonUtil.currentTimeNanos();
    (new App())->startup();
    cerr << endl << "Uptime: " << (long double) (CommonUtil.currentTimeNanos() - startTime) / 1000000000.0L << " s"
         << endl;
}
