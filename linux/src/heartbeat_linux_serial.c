/*

Serial Port Heartbeat Trigger for Linux - Version 1.0

Alessio Lombardo - 24/03/2020

*/

#include "heartbeat_linux_loads.c"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

int main(int argc, char *argv[]){
    
    int port=0;
    int limit=0;
    char component='m';
    
    float value;
    
    if(argc==1){
        printf("Dafault Configuration: Mixed Load, Port ttyS0, Limit 0\n");       
    }else{
        if(!strcmp(argv[1],"-h")||!strcmp(argv[1],"--help")){
            char help_string[600];
            sprintf(help_string,"USAGE: \n   heartbeat [m|c|r] [-p <PORT_NUMBER>] [-l <LIMIT>]\nOptions:\n   m\t\tMixed Load (50%% CPU, 50%% RAM) (First parameter, default)\n   c\t\tCPU Load (First parameter)\n   r\t\tRAM Load (First parameter)\n   -p, --port\tttyS Port Number (Default: 0)\n   -l, --limit\tActive when this limit is exceeded (from 0 to 100, default 0)\n   -h, --help\tShow this help\n\nAUTHOR: Alessio Lombardo\nVERSION: 1.0\n");
            printf("%s",help_string);
            exit(0);
        }
        
        if(!strcmp(argv[1],"-p")||!strcmp(argv[1],"--port"))
            port=atoi(argv[2]);
        else if(argc>=3 &&(!strcmp(argv[2],"-p")||!strcmp(argv[2],"--port")))
            port=atoi(argv[3]);
        else if(argc>=4 &&(!strcmp(argv[3],"-p")||!strcmp(argv[3],"--port")))
            port=atoi(argv[4]);
        else if(argc>=5 &&(!strcmp(argv[4],"-p")||!strcmp(argv[4],"--port")))
            port=atoi(argv[5]);
            
        if(!strcmp(argv[1],"-l")||!strcmp(argv[1],"--limit"))
            limit=atoi(argv[2]);
        else if(argc>=3 &&(!strcmp(argv[2],"-l")||!strcmp(argv[2],"--limit")))
            limit=atoi(argv[3]);
        else if(argc>=4 &&(!strcmp(argv[3],"-l")||!strcmp(argv[3],"--limit")))
            limit=atoi(argv[4]);
        else if(argc>=5 &&(!strcmp(argv[4],"-l")||!strcmp(argv[4],"--limit")))
            limit=atoi(argv[5]);
            
        if(!strcmp(argv[1],"m"))
            component='m';
        else if(!strcmp(argv[1],"c"))
            component='c';
        else if(!strcmp(argv[1],"r"))
            component='r';
            
    }

    char port_str[20] = "/dev/ttyS";
    sprintf(port_str, "%s%d", port_str, port);

    int fd = open(port_str, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1){
        printf("SERIAL PORT ERROR\n");
        exit(-1);
    }
    int DTR = TIOCM_DTR;
    ioctl(fd, TIOCMBIC, &DTR);

    while(1){

        if(component=='m')
            value = (GetCPULoad() + GetRAMLoad()) / 2.0f;
        else if(component=='c')        
            value = GetCPULoad();
        else if(component=='r')
            value = GetRAMLoad();
        if(value<limit){
            usleep(1000 * 1000);
            continue;
        }

        value=50;

        ioctl(fd, TIOCMBIS, &DTR);
        usleep(1000 * (int) ( 100 - value*0.3) );
        ioctl(fd, TIOCMBIC, &DTR);
        usleep(1000 * (int) ( 200 - value*1.5) );        
        ioctl(fd, TIOCMBIS, &DTR);
        usleep(1000 * (int) ( 100 - value*0.3) );
        ioctl(fd, TIOCMBIC, &DTR);
        usleep(1000 * (int) (1600 - value*12 ) );

    }
}



