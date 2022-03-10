#include "serial.h"

bool port_RUN(double yaw, double pitch, double mode,Base *base)
{
    int is_set=0;
    char data_name[] = { "/dev/ttyUSB0" };
    if(base->fd<=0){
        base->fd = port_OPEN(data_name);
    }
    if (base->fd == -1) return false;
    while (!is_set) {
        is_set=port_SET(base->fd);
        printf("port set success\n");
    }
    cout<<"serial=fd: "<<base->fd<<endl;
    all_data data;
    double send_data[3];
    send_data[0] = yaw * 100;
    send_data[1] = pitch * 1000;
    send_data[2] = mode;
    ushort* ptr = (ushort*)(data.data_all + 1);
    ptr[0] = (ushort)send_data[0];
    ptr[1] = (ushort)send_data[1];
    ptr[2] = (ushort)send_data[2];
    if (!port_SEND(base->fd, data)){
        printf("send false\n");
        close(base->fd);
        base->fd=port_OPEN(data_name);
        port_SET(base->fd);
    }
    char rd_data[14]={0};
    if(!port_RECV(base->fd,rd_data)){
        cout<<"recv false"<<endl;
        close(base->fd);
        base->fd=port_OPEN(data_name);
        port_SET(base->fd);
        return false;
    }
    if(rd_data[0]==0x78&&rd_data[13]==0x76){
        cout<<"recv success"<<endl;
        base->getYaw=(double(256*uint8_t(rd_data[1])+uint8_t(rd_data[2])))/100;
        base->getPitch=(double(256*uint8_t(rd_data[3])+uint8_t(rd_data[4])))/8192*360;
        base->getSpeed=(double(256*uint8_t(rd_data[5])+uint8_t(rd_data[6])))/100;
        double temp_yaw_angle=double(256*uint8_t(rd_data[7])+uint8_t(rd_data[8]));
        double temp_pitch_angle=double(256*uint8_t(rd_data[9])+uint8_t(rd_data[10]));
        if(temp_yaw_angle>32767){
            base->getYaw_angle=(temp_yaw_angle-65536)/10000;
        }
        else  base->getYaw_angle=temp_yaw_angle/10000;
        if(temp_pitch_angle>32767){
            base->getPitch_angle=(temp_pitch_angle-65536)/10000;
        }
        else  base->getPitch_angle=temp_pitch_angle/10000;
        if(rd_data[11]==0x11){
            base->getState=1;
        }
        else if(rd_data[11]=0x22){
            base->getState=2;
        }
        cout<<base->getYaw<<endl;
        cout<<base->getPitch<<endl;
        return true;
    }
    return false;
}
bool port_RECV_RUN(Base *base){

    int is_set=0;
    char data_name[] = { "/dev/ttyUSB0" };
    int fd = port_OPEN(data_name);
    if (fd == -1) return false;
    while (!is_set) {
        is_set=port_SET(base->fd);
        printf("port set success\n");
    }
    char rd_data[20];
    if(!port_RECV(fd,rd_data)){
        cout<<"recv false"<<endl;
        return false;
    }
    base->getYaw=float((256*int8_t(rd_data[1])+int8_t(rd_data[2]))/8192*360);
    base->getPitch=float((256*int8_t(rd_data[3])+int8_t(rd_data[4]))/8192*360);
    base->getSpeed=float((256*int8_t(rd_data[5])+int8_t(rd_data[6]))/100);
    if(rd_data[7]==0x11){
        base->getState=1;
    }
    else if(rd_data[7]=0x22){
        base->getState=2;
    }
    cout<<base->getYaw<<endl;
    cout<<base->getPitch<<endl;
    cout<<base->getSpeed<<endl;
    cout<<base->getState<<endl;
//            printf("recv false");
    close(fd);

    return true;
}
/*
	���ڴ򿪣�fd�ļ������֣������ҵ������ļ�
	port ������
*/
int port_OPEN(char *port) {
	
	//�ɶ�д������Ϊ�նˣ���ʱ��
    int fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);//
    printf("%d\n",fd);
    if (fd < 0) {
        printf("open false\n");
        return -1;
    }
    //if(fcntl(fd, F_SETFL, 0)<0)
    //printf("open success");
    if(fcntl(fd, F_SETFL, 0) < 0)
     {
         printf("fcntl failed!\n");
         return -1;
     }
     else
     {
         printf("Open Serial Port Succeed!\n");
     }
	return fd;
}
/*
	���ڳ�ʼ��
*/

bool port_SET(int fd) {
	struct termios options;
	if (tcgetattr(fd, &options)!=0) {
		printf("set false");
		return false;
	}

	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);

	//�޸Ŀ���ģʽ����֤���򲻻�ռ�ô���
	options.c_cflag |= CLOCAL;
	//�޸Ŀ���ģʽ��ʹ���ܹ��Ӵ����ж�ȡ��������
	options.c_cflag |= CREAD;

	options.c_cflag &= ~CRTSCTS;//��ʹ��������
	options.c_cflag &= ~PARENB;//����żУ��λ
	options.c_iflag &= ~INPCK;
	options.c_cflag &= ~CSTOPB;//ֹͣλ
	options.c_cflag &= ~CSIZE;//����
	options.c_cflag |= CS8;//��������λ
    options.c_cc[VMIN] = 0;//
    options.c_cc[VTIME] = 0;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    tcflush(fd,TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);//���޸ĺ��termios���õ�������
	return true;
}
/*
	��������
*/
int port_SEND(int fd, all_data data)
{
	int len = write(fd, data.data_all, 8);
	if (len == 8) {
        printf("send success\n");
		return len;
	}
	return 0;
}
bool port_RECV(int fd,char *data)
{

    fd_set rd_fd;

    FD_ZERO(&rd_fd);
    FD_SET(fd,&rd_fd);

    struct timeval times;
    times.tv_sec=0;
    times.tv_usec=5000;

    int rd_read;
    int sel=select(fd+1,&rd_fd,NULL,NULL,&times);
   // printf("%d\n",sel);

    if(sel)
    {

        char temp_data[28]={0};
        rd_read=read(fd,temp_data,28);

        for(int recv_i=0;recv_i<15;recv_i++){

            if(temp_data[recv_i]==0x78&&temp_data[recv_i+13]==0x76){

                for(int temp_i=0;temp_i<14;temp_i++){
                    data[temp_i]=temp_data[temp_i+recv_i];
                }
                printf("rd_read=%d\n",rd_read);
                return true;
            }
        }

    }
    return false;
}




