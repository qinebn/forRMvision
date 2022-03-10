#include "sentry.h"

bool sentry::sentry_fire(Base *base){
    tha_x=base->lastarmor.center.x-320;
    armor_width=base->lastarmor.size.width/2;
    if(tha_x>(-1.5*armor_width)&&tha_x<(armor_width*1.5)){
        cout<<"fire sure"<<endl;
        return true;

    }
    cout<<"fire false"<<endl;
    return false;
}
