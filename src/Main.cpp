#include <iostream>
#include "App.h"
#include "CommonUtil.h"

int main(int argc, char** argv)
{
    unsigned long startTime = CommonUtil::currentTimeNanos();
    App::getInstance().startup(argc, argv);
    cerr << endl << "Uptime: " << (long double) (CommonUtil::currentTimeNanos() - startTime) / 1000000000.0L << " s"
         << endl;
}
