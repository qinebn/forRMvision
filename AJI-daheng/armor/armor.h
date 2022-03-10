/*
    该文件用来：
            判断装甲板的真与假
            选择装甲板
*/
#ifndef ARMOR_H
#define ARMOR_H
#pragma once
#include "base.h"

class Light;

class Armor {
public:
    Armor()
    {

    }
    ~Armor()
    {
        armor_rect.clear();
    }
    void start(Light& light, Base& base);
    bool isnumber(RotatedRect rect, Base& base);
    bool istruetemple(Light& light , Base& base);
    RotatedRect choosetemple(Base *base);
    bool getlasttemple(Base &base);
    bool islargetemple(RotatedRect rect);

public:
    vector<RotatedRect> armor_rect;
    RotatedRect true_rect;
    bool flag = 0;
    double distance_= 99999;
    size_t gamma_changenumber = 0;
    size_t bai = 0;
    size_t sum = 0;
};

#endif // ARMOR_H
