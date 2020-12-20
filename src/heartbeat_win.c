/*

Heartbeat Trigger for Windows - Version 1.1

Alessio Lombardo - 18/12/2020

*/

#define _WIN32_WINNT 0x0501
#include <Windows.h>
#include <winioctl.h>

static unsigned long long FileTimeToInt64(const FILETIME *ft){
    ULARGE_INTEGER _time;
    _time.LowPart = ft->dwLowDateTime;
    _time.HighPart = ft->dwHighDateTime;
    return _time.QuadPart;
}

float GetCPULoad(){
    
    static unsigned long long _previousTotalTicks = 0;
    static unsigned long long _previousIdleTicks = 0;
    
    FILETIME idleTime, kernelTime, userTime;
    GetSystemTimes(&idleTime, &kernelTime, &userTime);

    unsigned long long idleTicks=FileTimeToInt64(&idleTime);
    unsigned long long totalTicks=FileTimeToInt64(&kernelTime)+FileTimeToInt64(&userTime);
    
    unsigned long long totalTicksSinceLastTime = totalTicks-_previousTotalTicks;
    unsigned long long idleTicksSinceLastTime  = idleTicks-_previousIdleTicks;

    _previousTotalTicks = totalTicks;
    _previousIdleTicks  = idleTicks;
    
    return (1.0f-((float)idleTicksSinceLastTime)/totalTicksSinceLastTime) * 100.0f;
   
}

float GetRAMLoad(){
    static MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx (&statex);
    
    return (float) statex.dwMemoryLoad;
}

HANDLE KeyboardSetup(){
    DefineDosDevice(DDD_RAW_TARGET_PATH, "Keybd", "\\Device\\KeyboardClass0");
    HANDLE hKeybd = CreateFile("\\\\.\\Keybd", GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if (hKeybd == INVALID_HANDLE_VALUE){
        printf("KEYBOARD ERROR\n");
        exit(-1);
    }
    return hKeybd;
}

HANDLE SerialPortSetup(int port){
    char port_str[20] = "\\\\.\\COM";
    snprintf(port_str, 20, "%s%d", port_str,port);   
    
    HANDLE hComm = CreateFile(port_str, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hComm == INVALID_HANDLE_VALUE){
        printf("SERIAL PORT ERROR\n");
        exit(-1);
    }
    EscapeCommFunction(hComm, CLRDTR);
    return hComm;
}

void SetKeyboardLed(HANDLE hKeybd, DWORD wFlags){
    
    if(hKeybd==NULL) return;
    
    DWORD retlen;
    WORD keyboard_buffer[2];
    DeviceIoControl(hKeybd, CTL_CODE(FILE_DEVICE_KEYBOARD, 0x10, METHOD_BUFFERED, FILE_ANY_ACCESS), 0, 0, &keyboard_buffer, sizeof(keyboard_buffer), &retlen, 0);
    if(wFlags & 0x80)
        keyboard_buffer[1] |= wFlags & 0xF;
    else if(wFlags & 0x40)
        keyboard_buffer[1] =~ (WORD)wFlags & 0xF;
    else
        keyboard_buffer[1] ^= wFlags & 0xF;
    DeviceIoControl(hKeybd, CTL_CODE(FILE_DEVICE_KEYBOARD, 0x2, METHOD_BUFFERED, FILE_ANY_ACCESS), &keyboard_buffer, sizeof(keyboard_buffer), 0, 0, &retlen, 0);

}

void SetSerialPort(HANDLE hComm, DWORD state){
    
    if(hComm==NULL) return;
    
    EscapeCommFunction(hComm, state);
    
}

void SetBeep(int beep){
    
    if(!beep) return;
    
    printf("\a");
   
}

int main(int argc, char *argv[]){

    int port=1;
    int keyboard_led=1;
    int beep=0;
    int limit=0;
    char component='m';
    
    HANDLE hKeybd;
    HANDLE hComm;
    
    float value;
    
    if(argc==1){
        printf("Dafault Configuration\n");       
    }else{
        
        int argument_index=1;
        
        while(argument_index<argc){
            
            if(!strcmp(argv[argument_index],"-h")||!strcmp(argv[argument_index],"--help")){
                char help_string[850];
                sprintf(help_string,"USAGE: \n   heartbeat [m|c|r] [-k <LED>|-nk] [-s <PORT>|-ns] [-b|-nb] [-l <LIMIT>]\nOptions:\n   m\t\t\tMixed Load (50%% CPU, 50%% RAM) (default)\n   c\t\t\tCPU Load\n   r\t\t\tRAM Load\n   -k,  --kled\t\tEnable KEYBOARD Mode (choise indicator led, default: %d)\n\t\t\t\t1\tScroll Lock Led\n\t\t\t\t2\tNum Lock Led\n\t\t\t\t3\tScroll+Num Lock Led\n\t\t\t\t4\tCaps Lock Led\n\t\t\t\t5\tScroll+Caps Lock Led\n\t\t\t\t6\tNum+Caps Lock Led\n\t\t\t\t7\tScroll+Num+Caps Lock Led\n   -nk, --no-kled\tDisable KEYBOARD Mode\n   -s,  --serial\tEnable SERIAL Mode (choise COM port, default: %d)\n   -ns, --no-serial\tDisable SERIAL Mode\n   -b,  --beep\t\tEnable BEEP Mode\n   -nb, --no-beep\tDisable BEEP Mode (default)\n   -l,  --limit\t\tActive only when this limit is exceeded (0 to 100, default %d)\n   -h,  --help\t\tShow this help\n\nAUTHOR: Alessio Lombardo\nVERSION: 1.1\n",1,1,0);
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
    else hKeybd=NULL;

    if(port!=-1) hComm=SerialPortSetup(port);
    else hComm=NULL;
    
    printf("Heartbeat Trigger ACTIVE. Ctrl-C to quit.\n");

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

        SetKeyboardLed(hKeybd, keyboard_led);
        SetSerialPort(hComm, SETDTR);
        SetBeep(beep);
        sleep( (int) ( 100 - value*0.3) );
        SetKeyboardLed(hKeybd, keyboard_led);
        SetSerialPort(hComm, CLRDTR);
        sleep( (int) ( 200 - value*1.5) );        
        SetKeyboardLed(hKeybd, keyboard_led);
        SetSerialPort(hComm, SETDTR);
        SetBeep(beep);
        sleep( (int) ( 100 - value*0.3) );
        SetKeyboardLed(hKeybd, keyboard_led);
        SetSerialPort(hComm, CLRDTR);
        sleep( (int) (1600 - value*12 ) );

    }
}
