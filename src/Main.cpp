#include <iostream>
#include "ProjectNuma.h"

int main(int argc, char** argv)
{
    cerr << endl << VERSION << endl
         << "Made by @MCUmbrella, licensed under MIT" << endl
         << "https://github.com/MCUmbrella/ProjectNuma" << endl << endl;
    string arg;
    for (int i = 1; i != argc; i++)
    {
        arg = argv[i];
        if (arg == "-h")
        {
            cerr << "\t'-h': Show this help message" << endl
                 << "\t'-d': Enable debug mode" << endl
                 << "\t'-f': Try to run the game in fullscreen" << endl
                 << "\t'-s': Force use software rendering" << endl;
            return 0;
        }
        else if (arg == "-d") flDebug = true;
        else if (arg == "-f") flFullscreen = true;
        else if (arg == "-s") flSoftwareRender = true;
    }
    unsigned long startTime = CommonUtil.currentTimeNanos();
    (new App())->startup();
    cerr << endl << "Uptime: " << (long double) (CommonUtil.currentTimeNanos() - startTime) / 1000000000.0L << " s"
         << endl;
}
