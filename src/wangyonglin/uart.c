#include <wangyonglin/config.h>
#include <wangyonglin/core.h>
 

typedef struct {
    int fd;
    void * (*recv)(unsigned char * buf,int len);
}wangyonglin_uart_callback_t;

int fd;
 void * __recv(unsigned char * buf,int len);
void *__callback(void *args);

int wangyonglin_uart_attr(int fd,int nSpeed, int nBits, char nEvent, int nStop)
    {
        struct termios newtio, oldtio;
        /*保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/
        if (tcgetattr(fd, &oldtio) != 0)
        {
            perror("SetupSerial 1");
            return -1;
        }
        bzero(&newtio, sizeof(newtio));
     
        /*步骤一，设置字符大小*/
        newtio.c_cflag |= CLOCAL | CREAD;
        newtio.c_cflag &= ~CSIZE;
     
        /*设置停止位*/
        switch (nBits)
        {
            case 7:
                newtio.c_cflag |= CS7;
                break;
            case 8:
                newtio.c_cflag |= CS8;
                break;
        }
     
        /*设置奇偶校验位*/
        switch (nEvent)
        {
            case 'O':    //奇数
                newtio.c_cflag |= PARENB;
                newtio.c_cflag |= PARODD;
                newtio.c_iflag |= (INPCK | ISTRIP);
                break;
            case 'E':    //偶数
                newtio.c_iflag |= (INPCK | ISTRIP);
                newtio.c_cflag |= PARENB;
                newtio.c_cflag &= ~PARODD;
                break;
            case 'N':    //无奇偶校验位
                newtio.c_cflag &= ~PARENB;
                break;
        }
     
        /*设置波特率*/
        switch (nSpeed)
        {
            case 2400:
                cfsetispeed(&newtio, B2400);
                cfsetospeed(&newtio, B2400);
                break;
            case 4800:
                cfsetispeed(&newtio, B4800);
                cfsetospeed(&newtio, B4800);
                break;
            case 9600:
                cfsetispeed(&newtio, B9600);
                cfsetospeed(&newtio, B9600);
                break;
            case 115200:
                cfsetispeed(&newtio, B115200);
                cfsetospeed(&newtio, B115200);
                break;
            case 460800:
                cfsetispeed(&newtio, B460800);
                cfsetospeed(&newtio, B460800);
                break;
            default:
                cfsetispeed(&newtio, B9600);
                cfsetospeed(&newtio, B9600);
                break;
        }
     
        /*设置停止位*/
        if (nStop == 1)
        {
            newtio.c_cflag &= ~CSTOPB;
        }
        else if (nStop == 2)
        {
            newtio.c_cflag |= CSTOPB;
        }
     
        /*设置等待时间和最小接收字符*/
        newtio.c_cc[VTIME] = 0;
        newtio.c_cc[VMIN]  = 0;
     
        /*处理未接收字符*/
        tcflush(fd, TCIFLUSH);
     
        /*激活新配置*/
        if ((tcsetattr(fd, TCSANOW, &newtio)) != 0)
        {
            perror("com set error");
            return -1;
        }
     
        printf("set done!\n");
        return 0;
    }
     
    /*打开串口函数*/
int wangyonglin_uart_open(const char* device){
        fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
        if (-1 == fd)
        {
            perror("Can't Open Serial Port");
            return (-1);
        }
     
        /*恢复串口为阻塞状态*/
        if (fcntl(fd, F_SETFL, 0) < 0)
        {
            printf("fcntl failed!\n");
        }
        else
        {
            printf("fcntl=%d\n", fcntl(fd, F_SETFL, 0));
        }
     
        /*测试是否为终端设备*/
        if (isatty(STDIN_FILENO) == 0)
        {
            printf("standard input is not a terminal device\n");
        }
        else
        {
            printf("isatty success!\n");
        }
     
        printf("fd-open=%d\n", fd);
        return fd;
    }
     
  
int  wangyonglin_uart_write(void* arg){
    int rc;
        char     buf[1024] = {0};
    rc = write(fd,buf,sizeof(buf));
    if(rc<0){
        fprintf(stderr,"wangyonglin_uart_write failed \n");
        rc = -1;
    }
        return rc;
}
int __uart_isready(int fd){
    int rc;
    fd_set fds;
    struct timeval tv;
    FD_ZERO(&fds);
    FD_SET(fd,&fds);
    tv.tv_sec = tv.tv_usec = 0;    
    rc = select(fd+1, &fds, NULL, NULL, &tv);
    if(rc<0) return -1;
    return FD_ISSET(fd,&fds) ? 1 : 0;
}
 
void *__callback(void *args){
    wangyonglin_uart_callback_t *call = (wangyonglin_uart_callback_t*)args;
        int fd = call->fd;
        char     buff[1024] = {0};
        uint32_t count      = 0;
        int rc;
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        for(;;){
            if(__uart_isready){
                rc = read(fd, buff, sizeof(buff));
                if (rc > 0)
                {
                    count++;
                    call->recv(buff,rc);
                }
            }
            
          
        }
    
}
 void * __recv(unsigned char * buf,int len){
     printf("%s   -> %d \n",buf,len);
 }
void wangyonglin_uart_init(const char * device){
    pthread_t id_t;
    wangyonglin_uart_callback_t __uart;
    fd = wangyonglin_uart_open(device);
    if(fd==-1){
        fprintf(stderr,"wangyonglin_uart_open failed\n");
        return;
    }
      printf("fd %d\n",fd);
    if(wangyonglin_uart_attr(fd,115200, 8, 'N', 1)!=0){
        fprintf(stderr,"wangyonglin_uart_attr failed\n");
        return;
    }
    __uart.fd=fd;
    __uart.recv=__recv;
    pthread_create(&id_t,NULL,__callback,&__uart);
    pthread_join(id_t,NULL);
}

