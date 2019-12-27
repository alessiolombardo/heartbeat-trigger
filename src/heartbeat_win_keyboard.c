/*

Keyboard Led Heartbeat Trigger for Windows - Version 1.0

Alessio Lombardo - 23/12/2019

*/

#include "heartbeat_win_loads.c"
#include <winioctl.h>

void SetKeyboardLed(HANDLE hKeybd, DWORD wFlags){
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

int main(int argc, char *argv[]){

    int keyboard_led=1;
    int limit=0;
    char component='m';
    
    float value;
    
    if(argc==1){
        printf("Dafault Configuration: Mixed Load, SCROLL LOCK Key, Limit 0\n");       
    }else{
        if(!strcmp(argv[1],"-h")||!strcmp(argv[1],"--help")){
            char help_string[500];
            sprintf(help_string,"USAGE: \n   heartbeat [m|c|r] [-k <KEYBOARD_LED_NUMBER>] [-l <LIMIT>]\nOptions:\n   m\t\tMixed Load (50%% CPU, 50%% RAM) (First parameter, default)\n   c\t\tCPU Load (First parameter)\n   r\t\tRAM Load (First parameter)\n   -k, --k-led\tKeyboard Led Number (Default: 1)\n\t\t\t1\tScroll Lock Led\n\t\t\t2\tNum Lock Led\n\t\t\t3\tScroll+Num Lock Led\n\t\t\t4\tCaps Lock Led\n\t\t\t5\tScroll+Caps Lock Led\n\t\t\t6\tNum+Caps Lock Led\n\t\t\t7\tScroll+Num+Caps Lock Led\n   -l, --limit\tActive when this limit is exceeded (from 0 to 100, default 0)\n   -h, --help\tShow this help.\n");
            printf("%s",help_string);
            system("PAUSE");
            exit(0);
        }
        
        if(!strcmp(argv[1],"-k")||!strcmp(argv[1],"--k-led"))
            keyboard_led=atoi(argv[2]);
        else if(argc>=3 &&(!strcmp(argv[2],"-k")||!strcmp(argv[2],"--k-led")))
            keyboard_led=atoi(argv[3]);
        else if(argc>=4 &&(!strcmp(argv[3],"-k")||!strcmp(argv[3],"--k-led")))
            keyboard_led=atoi(argv[4]);
        else if(argc>=5 &&(!strcmp(argv[4],"-k")||!strcmp(argv[4],"--k-led")))
            keyboard_led=atoi(argv[5]);
            
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
    
    DefineDosDevice(DDD_RAW_TARGET_PATH, "Keybd", "\\Device\\KeyboardClass0");
    HANDLE hKeybd = CreateFile("\\\\.\\Keybd", GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if (hKeybd == INVALID_HANDLE_VALUE){
        printf("KEYBOARD ERROR\n");
        //system("PAUSE");
        exit(-1);
    }
    
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
        sleep( (int) ( 100 - value*0.3) );
        SetKeyboardLed(hKeybd, keyboard_led);
        sleep( (int) ( 200 - value*1.5) );        
        SetKeyboardLed(hKeybd, keyboard_led);
        sleep( (int) ( 100 - value*0.3) );
        SetKeyboardLed(hKeybd, keyboard_led);
        sleep( (int) (1600 - value*12 ) );

    }
}
