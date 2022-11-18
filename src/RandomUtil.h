//
// Created by mcumbrella on 22-11-9.
//

#ifndef PROJECTNUMA_RANDOMUTIL_H
#define PROJECTNUMA_RANDOMUTIL_H

#include <random>

class Random
{
private:
    mt19937_64 r = mt19937_64(CommonUtil.currentTimeNanos());
public:
    /**
     * Return an random int value in [0, n).
     */
    int nextInt(int n)
    {
        return r() % n;
    }

    /**
     * Return a random double value in [0, n).
     */
    double nextDouble(double n)
    {
        uniform_real_distribution<double> unif(0, n);
        return unif(r);
    }
};

#endif //PROJECTNUMA_RANDOMUTIL_H
