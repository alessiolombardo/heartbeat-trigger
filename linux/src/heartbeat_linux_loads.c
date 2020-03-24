/*

CPU and RAM Load Functions for Linux - Version 1.0

Alessio Lombardo - 24/03/2020

*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/sysinfo.h>

int old_tottime=0;
int old_idletime=0;

float GetCPULoad(){

    int values[7];
    int tottime=0;
    char result[128];
    FILE *cmd = popen("grep \"cpu\" /proc/stat | tr -d [:alpha:]", "r");
    fgets(result, sizeof(result), cmd);
    pclose(cmd);

    sscanf(result,"%d %d %d %d %d %d %d",&values[0],&values[1],&values[2],&values[3],&values[4],&values[5],&values[6]);

    for(int i=0;i<7;i++)
        tottime+=values[i];

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
