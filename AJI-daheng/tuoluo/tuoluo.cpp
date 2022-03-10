#include "tuoluo.h"

Tuoluo::Tuoluo()
{

}
Tuoluo::~Tuoluo(){}

bool Tuoluo::start(size_t *time_count, size_t *tuoluo, size_t *tuoluo_count, bool *tuoluo_status, bool *tuoluo_start, double *sita_y, double *last_sita_y,
                   deque<double> *sita)
{
    this->time_count = *time_count;
    this->tuoluo = *tuoluo;
    this->tuoluo_count = *tuoluo_count;
    this->tuoluo_start = *tuoluo_start;
    this->tuoluo_status = *tuoluo_status;
    this->sita_y = *sita_y;
    this->last_sita_y = *last_sita_y;
    this->sita = *sita;
    iftuoluo();
    /*
    if(1 == *tuoluo_status)
    {
        tuoluo_();
        *sita_y = avg_sita;
        return 1;
    }*/
    *tuoluo_count = this->tuoluo_count;
    *time_count = this->time_count;
    *last_sita_y = this->sita_y;
    cout << "tuoluo_count" << *tuoluo_count << endl;
    cout << "time_count" << *time_count << endl;
    cout << "sita_y" << *sita_y << endl;
    cout << "*last_sita_y" << *last_sita_y << endl;
    return this->tuoluo_status;

}

void Tuoluo::iftuoluo()
{
    time_count +=1;
    if(abs(sita_y - last_sita_y) > 3 && abs(sita_y - last_sita_y) < 8)
    {
        tuoluo_count+=1;
    }
    if(time_count > 30)
    {
        time_count = 0;
        if(tuoluo_count > 5)
        {
            tuoluo_status = 1;
        }
        else
        {
            tuoluo_status = 0;
        }
        tuoluo_count = 0;
    }

    /*
    time_count +=1;
    if(sita.size() <= 10)
    {
        sita.push_back(sita_y);
    }
    else
    {
        sita.pop_front();
        sita.push_back(sita_y);
    }
    if(time_count >= 30 && 0 == tuoluo_status)
    {
        tuoluo_count = 0;
        tuoluo = 0;
        tuoluo_start = 0;
        time_count = 0;
    }
    if(time_count >= 30 && 1 == tuoluo_status)
    {
        tuoluo_count = 0;
        tuoluo = 0;
        tuoluo_start = 0;
        time_count = 0;
        tuoluo_status = 0;
    }
    if(!tuoluo_start)
    {
        if(abs(sita_y - last_sita_y) > 3 && abs(sita_y - last_sita_y) < 8)
        {
            tuoluo_start = 1;
        }
    }
    else
    {
        if(abs(sita_y - last_sita_y) < 1)   tuoluo += 1;
        else
        {
            tuoluo = 0;
            tuoluo_start = 0;
        }
        if(tuoluo >= 3)
        {
            tuoluo_count +=1;
            tuoluo = 0;
            tuoluo_start = 0;
        }
    }
    if(tuoluo_count >= 3)
    {
        tuoluo_status = 1;
        printf("tuoluo_status!\n");
        time_count = 0;
    }*/
}

bool Tuoluo::tuoluo_()
{
    if(1 == tuoluo_status)
    {
        for(size_t i =0; i< sita.size(); i++)
        {
             if(sita.at(i) > max)
             {
                 max = sita.at(i);
             }
             if(sita.at(i) < min)
             {
                 min = sita.at(i);
             }
        }
        if(max - min < 10)
        {
            avg_sita = (max+min)/2;
            return 1;
        }
        else
            return 0;
    }
    else
        return 0;
}
