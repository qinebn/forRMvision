#ifndef SENTRY_H
#define SENTRY_H
#include"base.h"

class sentry
{
public:
    sentry(){tha_x=0;armor_width=0;};
    bool sentry_fire(Base *base);
    double tha_x=0;//the sub between armor.center and img.center
    double armor_width=0;//for get fire size
};

#endif // SENTRY_H
