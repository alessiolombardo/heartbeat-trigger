/*

Heartbeat Trigger for Linux - Version 1.1

Alessio Lombardo - 18/12/2020

*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <fcntl.h>


int old_tottime=0;
int old_idletime=0;

int DTR = TIOCM_DTR;

float GetCPULoad(){

    int values[7];
    int tottime=0;
    char result[128];
    FILE *cmd = popen("grep \"cpu\" /proc/stat | tr -d [:alpha:]", "r");
    fgets(result, sizeof(result), cmd);
    pclose(cmd);

    sscanf(result,"%d %d %d %d %d %d %d",&values[0],&values[1],&values[2],&values[3],&values[4],&values[5],&values[6]);

    for(int i=0;i<7;i++) tottime+=values[i];

    float return_value = 100.0f - (float)(values[3]-old_idletime)/(float)(tottime-old_tottime) * 100.0f;

    old_tottime=tottime;
    old_idletime=values[3];

    return return_value;
   
}

float GetRAMLoad(){

    struct sysinfo memInfo;
    sysinfo(&memInfo);
    char result[32];
    FILE *cmd = popen("grep \"MemAvailable\" /proc/meminfo | tr -d -c [:digit:]", "r");
    fgets(result, sizeof(result), cmd);
    pclose(cmd);
    return 100.0f - (float)((long)atoi(result)*1024l)/(float)memInfo.totalram  * 100.0f;

}


int KeyboardSetup(){

    int hKeybd = open("/dev/console", O_NOCTTY);
    if(hKeybd == -1){
        printf("KEYBOARD ERROR\n");
        exit(-1);
    }

    return hKeybd;

}

int SerialPortSetup(int port){
    char port_str[40];
    sprintf(port_str,"/dev/ttyS%d", port);
    int hComm = open(port_str, O_RDWR | O_NOCTTY | O_NDELAY);
    if (hComm == -1){
        printf("SERIAL PORT ERROR\n");
        exit(-1);
    }
    
    ioctl(hComm, TIOCMBIC, &DTR);
    return hComm;
}

void SetKeyboardLed(int hKeybd, int wFlags, int state){

    if(hKeybd==-1) return;
    
    if(state==1) ioctl(hKeybd, KDSETLED, wFlags);
    else ioctl(hKeybd, KDSETLED, ~wFlags);

}

void SetSerialPort(int hComm, int state){

    ioctl(hComm, state, &DTR);

}

void SetBeep(int beep){

    if(!beep) return;
    
    printf("\a");
    fflush(stdout);    
}

int main(int argc, char *argv[]){
    int port=0;
    int keyboard_led=1;
    int beep=0;
    int limit=0;
    char component='m';
    
    int hKeybd;
    int hComm;
    
    float value;
      
    if(argc==1){
        printf("Dafault Configuration\n");       
    }else{
        
        int argument_index=1;
        
        while(argument_index<argc){
            
            if(!strcmp(argv[argument_index],"-h")||!strcmp(argv[argument_index],"--help")){
                char help_string[850];
                sprintf(help_string,"USAGE: \n   heartbeat [m|c|r] [-k <LED>|-nk] [-s <PORT>|-ns] [-b|-nb] [-l <LIMIT>]\nOptions:\n   m\t\t\tMixed Load (50%% CPU, 50%% RAM) (default)\n   c\t\t\tCPU Load\n   r\t\t\tRAM Load\n   -k,  --kled\t\tEnable KEYBOARD Mode (choise indicator led, default: %d)\n\t\t\t\t1\tScroll Lock Led\n\t\t\t\t2\tNum Lock Led\n\t\t\t\t3\tScroll+Num Lock Led\n\t\t\t\t4\tCaps Lock Led\n\t\t\t\t5\tScroll+Caps Lock Led\n\t\t\t\t6\tNum+Caps Lock Led\n\t\t\t\t7\tScroll+Num+Caps Lock Led\n   -nk, --no-kled\tDisable KEYBOARD Mode\n   -s,  --serial\tEnable SERIAL Mode (choise ttyS port, default: %d)\n   -ns, --no-serial\tDisable SERIAL Mode\n   -b,  --beep\t\tEnable BEEP Mode\n   -nb, --no-beep\tDisable BEEP Mode (default)\n   -l,  --limit\t\tActive only when this limit is exceeded (0 to 100, default %d)\n   -h,  --help\t\tShow this help\n\nAUTHOR: Alessio Lombardo\nVERSION: 1.1\n",1,0,0);
                printf("%s",help_string);
                exit(0);           
            }else if(!strcmp(argv[argument_index],"m")){
                component='m';
            }else if(!strcmp(argv[argument_index],"c")){
                component='c';
            }else if(!strcmp(argv[argument_index],"r")){
                component='r';                
            }else if(!strcmp(argv[argument_index],"-k")||!strcmp(argv[argument_index],"--kled")){
                argument_index++;
                keyboard_led=atoi(argv[argument_index]);
            }else if(!strcmp(argv[argument_index],"-nk")||!strcmp(argv[argument_index],"--no-kled")){
                keyboard_led=-1;
            }else if(!strcmp(argv[argument_index],"-s")||!strcmp(argv[argument_index],"--serial")){
                argument_index++;
                port=atoi(argv[argument_index]);
            }else if(!strcmp(argv[argument_index],"-ns")||!strcmp(argv[argument_index],"--no-serial")){
                port=-1;
            }else if(!strcmp(argv[argument_index],"-b")||!strcmp(argv[argument_index],"--beep")){
                beep=1;
            }else if(!strcmp(argv[argument_index],"-nb")||!strcmp(argv[argument_index],"--no-beep")){
                beep=0;
            }else if(!strcmp(argv[argument_index],"-l")||!strcmp(argv[argument_index],"--limit")){
                argument_index++;
                limit=atoi(argv[argument_index]);
            }else{
                printf("Warning: INVALID ARGUMENT (position %d)\n",argument_index);    
            }
   
            argument_index++;
        }
        
    }

    if(keyboard_led!=-1) hKeybd=KeyboardSetup();
    else hKeybd=-1;

    if(port!=-1) hComm=SerialPortSetup(port);
    else hComm=-1;
    
    printf("Heartbeat Trigger ACTIVE. Ctrl-C to quit.\n");

    while(1){

        if(component=='m')
            value = (GetCPULoad() + GetRAMLoad()) / 2.0f;
        else if(component=='c')        
            value = GetCPULoad();
        else if(component=='r')
            value = GetRAMLoad();
        if(value<limit){
            usleep(1000*1000);
            continue;
        }

        SetKeyboardLed(hKeybd, keyboard_led,1);
        SetSerialPort(hComm, TIOCMBIS);
        SetBeep(beep);
        usleep(1000 * (int) ( 100 - value*0.3) );
        SetKeyboardLed(hKeybd, keyboard_led,0);
        SetSerialPort(hComm, TIOCMBIC);
        usleep(1000 * (int) ( 200 - value*1.5) );        
        SetKeyboardLed(hKeybd, keyboard_led,1);
        SetSerialPort(hComm, TIOCMBIS);
        SetBeep(beep);
        usleep(1000 * (int) ( 100 - value*0.3) );
        SetKeyboardLed(hKeybd, keyboard_led,0);
        SetSerialPort(hComm, TIOCMBIC);
        usleep(1000 * (int) (1600 - value*12 ) );

    }
}
