//
// Created by mcumbrella on 22-11-9.
//

#include <chrono>

#ifndef PROJECTNUMA_COMMONUTIL_H

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))
#define null nullptr

static class
{
public:
    static bool checkCollision(double x1, double y1, double w1, double h1, double x2, double y2, double w2, double h2)
    {
        return (MAX(x1, x2) < MIN(x1 + w1, x2 + w2)) && (MAX(y1, y2) < MIN(y1 + h1, y2 + h2));
    }

    static uint64_t currentTimeNanos()
    {
        using namespace std::chrono;
        return duration_cast<nanoseconds>(
                system_clock::now().time_since_epoch()
        ).count();
    }

    static double getDegreeBeteween(double x1, double y1, double x2, double y2)
    {
        double dx = x2 - x1, dy = y2 - y1;
        return -atan2(dy, dx) * 57.2958;
    }
} CommonUtil;

#define PROJECTNUMA_COMMONUTIL_H

#endif //PROJECTNUMA_COMMONUTIL_H
