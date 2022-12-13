#ifndef POOL_POOLBALL_H
#define POOL_POOLBALL_H

#include "circle.h"

class PoolBall : public Circle {
private:
    int value;
public:
    PoolBall(double x, double y, double radius, int value);

    int getValue() const;

    void draw() const override;
};


#endif //POOL_POOLBALL_H
