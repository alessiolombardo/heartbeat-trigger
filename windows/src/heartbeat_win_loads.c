/*

CPU and RAM Load Functions for Windows - Version 1.0

Alessio Lombardo - 23/12/2019

*/

#define _WIN32_WINNT 0x0501
#include <Windows.h>

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
