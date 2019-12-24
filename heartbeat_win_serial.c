/*

Serial Port Heartbeat Trigger for Windows - Version 1.0

Alessio Lombardo - 23/12/2019

*/

#include "heartbeat_win_loads.c"

int main(int argc, char *argv[]){
    
    int port=1;
    int limit=0;
    char component='m';
    
    float value;
    
    if(argc==1){
        printf("Dafault Configuration: Mixed Load, Port COM1, Limit 0\n");       
    }else{
        if(!strcmp(argv[1],"-h")||!strcmp(argv[1],"--help")){
            char help_string[500];
            sprintf(help_string,"USAGE: \n   heartbeat [m|c|r] [-p <PORT_NUMBER>] [-l <LIMIT>]\nOptions:\n   m\t\tMixed Load (50%% CPU, 50%% RAM) (First parameter, default)\n   c\t\tCPU Load (First parameter)\n   r\t\tRAM Load (First parameter)\n   -p, --port\tCOM Port Number (Default: 1)\n   -l, --limit\tActive when this limit is exceeded (from 0 to 100, default 0)\n   -h, --help\tShow this help.\n");
            printf("%s",help_string);
            system("PAUSE");
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

    char port_str[20] = "\\\\.\\COM";
    snprintf(port_str, 20, "%s%d", port_str,port);   
    
    HANDLE hComm = CreateFile(port_str, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hComm == INVALID_HANDLE_VALUE){
        printf("SERIAL PORT ERROR\n");
        //system("PAUSE");
        exit(-1);
    }
    EscapeCommFunction(hComm, CLRDTR);

    while(1){

        if(component=='m')
            value = (GetCPULoad() + GetRAMLoad()) / 2.0f;
        else if(component=='c')        
            value = GetCPULoad();
        else if(component=='r')
            value = GetRAMLoad();
        if(value<limit){
            sleep(1000);
            continue;
        }

        EscapeCommFunction(hComm, SETDTR);
        sleep( (int) ( 100 - value*0.3) );
        EscapeCommFunction(hComm, CLRDTR);
        sleep( (int) ( 200 - value*1.5) );        
        EscapeCommFunction(hComm, SETDTR);
        sleep( (int) ( 100 - value*0.3) );
        EscapeCommFunction(hComm, CLRDTR);
        sleep( (int) (1600 - value*12 ) );

    }
}


