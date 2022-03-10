#include<stdio.h>
#include<stdlib.h>     
#include<sys/types.h>  
#include<sys/stat.h>   
#include<fcntl.h>      
#include<errno.h>      
#include<time.h>
#include<string.h>
#include<termios.h>    
#include<unistd.h>   
#include"base.h"
#include<iostream>
using namespace std;

typedef struct {
    unsigned char data_all[8] = { 0xED,0x00,0x00,0x00,0x00,0x00,0x00,0xEC };
}all_data;

bool port_RUN(double yaw, double pitch, double mode,Base *base);
int port_OPEN(char *port);//打开串口
bool port_SET(int fd);//设置串口
int port_SEND(int fd, all_data data);
bool port_RECV(int fd,char *data);
bool port_RECV_RUN(Base *base);

