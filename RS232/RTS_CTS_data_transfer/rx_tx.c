#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include<time.h>


#define SLEEP_TIME 100000
struct timespec tim1;


int open_port(void)
{
    int fd;
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd == -1)
        perror("open_port: Unable to open /dev/ttyS0 − ");

    else
        fcntl(fd, F_SETFL, 0);
    return (fd);

}


int set_rts(int fd,FILE *fp){
    //printf("hi");
    int status,i=0,val;
    struct timespec tim1,tim2;
    tim1.tv_sec=0;
    tim1.tv_nsec=800000000;
    status = TIOCM_RTS;
    ioctl(fd,TIOCMBIC,&status);
    nanosleep(&tim1,NULL);
    while(!feof(fp)){
        val = fgetc(fp);
        //printf("hello\n");
        do{
            if(val>>i&1){
                ioctl(fd,TIOCMBIS,&status);
            }
            else{
                ioctl(fd,TIOCMBIC,&status);
                //printf("0");
            }
            //sleep(SLEEP_TIME);
            nanosleep(&tim1,NULL);
            i++;
        }while(i<=8);
        i=0;
    }
    return 0;
}

int read_rts(int fd){

    struct timespec tim1,tim2;
    tim2.tv_sec=0;
    tim2.tv_nsec=100000000;
    tim1.tv_sec=0;
    tim1.tv_nsec=100000000;
    int status;
    int val=0;
    int i=0,r=0;
    //while(1){
      //  val=0;
        while(1){
            if (ioctl(fd, TIOCMGET, &status) == -1)
                printf("TIOCMGET failed: %s\n",
                        strerror(errno));
            else {
                //printf("%d ",status&TIOCM_CTS);
                if (!(status & TIOCM_CTS)){
                    break;
                }
            }
        }
        nanosleep(&tim2,NULL);
        do{
            if (ioctl(fd, TIOCMGET, &status) == -1)
                printf("TIOCMGET failed: %s\n",
                        strerror(errno));
            else {
                if (status & TIOCM_CTS)
                    val|=1<<i;
            }
            nanosleep(&tim1,NULL);
            i++;
        }while(i<8);
        i=0;
        printf("%c\n",val);
        fflush(stdout);
   // }
    return val;
}

int main(int argc,char *argv[]){
    int y,n,x=open_port(),status,fd;
    //FILE *fp;
    //fp = fopen(argv[1],"r");
    //scanf("%d",&y);
    //y=set_rts(x,fp);
    y=read_rts(x);
    //scanf("%d",&y);
    // printf("\n\n\n");
    return 0;
}
