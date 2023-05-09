//
// Created by mcumbrella on 22-11-9.
//

#ifndef PROJECTNUMA_COMMONUTIL_H
#define PROJECTNUMA_COMMONUTIL_H

#include "GeneralDefinitions.h"

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))

class CommonUtil
{
public:
    /**
     * Check if two rectangles overlap.
     */
    static bool checkCollision(
            const double& x1, const double& y1, const double& w1, const double& h1,
            const double& x2, const double& y2, const double& w2, const double& h2
    )
    {
        return (MAX(x1, x2) < MIN(x1 + w1, x2 + w2)) && (MAX(y1, y2) < MIN(y1 + h1, y2 + h2));
    }

    /**
     * How many nanoseconds have passed since 0:00:00 on January 1, 1970?
     */
    static unsigned long currentTimeNanos()
    {
        using namespace chrono;
        return duration_cast<nanoseconds>(
                system_clock::now().time_since_epoch()
        ).count();
    }

    /**
     * Return the angle between the vector of point 1 to point 2 and the x-axis.
     */
    static double getDegreeBetween(const double& x1, const double& y1, const double& x2, const double& y2)
    {
        double dx = x2 - x1, dy = y2 - y1;
        return -atan2(dy, dx) * 57.2958;
    }
};

#endif //PROJECTNUMA_COMMONUTIL_H
