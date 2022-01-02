#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "modules.h"

/* variables */
long int cpuWorkCache = 0;
long int cpuTotalCache = 0;

/* function declarations */
static char *moduleFormatter(int lowVal, int highVal, char *lowIcon, char *midIcon, char *highIcon, float formatVal);

/* function implementations */
char *
getDateTime(Args *args, int flag)
{
        char *dateTime = malloc(sizeof(char) * 72);
        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        sprintf(dateTime, " %s%02d:%02d %s%d-%d-%d",t->tm_hour >= 22 ? "🌙" : t->tm_hour < 12 ? "☕" :"🌞",  t->tm_hour, t->tm_min,(t->tm_mon + 1) == 12 ? "🎄" : "🗓️", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
        return dateTime;
}

char *
getMem(Args *arg, int flag)
{
        char buffer[1024] = "";
        char *memUsage = malloc(sizeof(char) * 12);
        int memTotal = 0;
        int memAvailable = 0;

        FILE *file = fopen("/proc/meminfo", "r");
        if (file == NULL) return "";

        while (fscanf(file, " %1023s", buffer) == 1)
        {
                if (strcmp(buffer, "MemTotal:") == 0)
                {
                        fscanf(file, " %d", &memTotal);
                }

                if (strcmp(buffer, "MemAvailable:") == 0)
                {
                        fscanf(file, " %d", &memAvailable);
                }
        }
        fclose(file);
        float usage = (float)(memTotal - memAvailable) * 1e-6;
        
        if (flag == 0)
        {
                sprintf(memUsage, " %s%.2fGb",moduleFormatter(arg->maxArgs.lowVal, arg->maxArgs.highVal, arg->maxArgs.lowIcon, arg->maxArgs.midIcon, arg->maxArgs.highIcon, usage), usage);
                return memUsage;
        }
        sprintf(memUsage, " %s%.2fGb", arg->minArgs.icon, usage);
        return memUsage;
}

char *
getCpuLoad(Args *arg, int flag)
{
        long int cpuTotal = 0;
        long int cpuWork = 0;
        int i=0;

        char *result = malloc(sizeof(char) * 25);
        char line[1][128];

        FILE *file = fopen("/proc/stat", "r");
        if (file == NULL) return "";

        fgets(line[0], 100, file);
        line[0][strlen(line[0]) - 1] = '\0';
        fclose(file);

        char *token = strtok(line[0], " ");

        while (token != NULL)
        {
                i++;
                token = strtok(NULL, " ");
                if (i>0 && i<7)
                {
                        cpuTotal += atoi(token);
                }
                if (i>0 && i<4)
                {
                        cpuWork += atoi(token);
                }
        }
        float cpuLoad = fabs((float)(cpuWork - cpuWorkCache) / (float)(cpuTotal - cpuTotalCache) * 100);

        
        if (flag == 0)
        {
                sprintf(result, " %s%.2f%%", moduleFormatter(arg->maxArgs.lowVal, arg->maxArgs.highVal, arg->maxArgs.lowIcon, arg->maxArgs.midIcon, arg->maxArgs.highIcon, cpuLoad), cpuLoad );
        }
        else
        {
                sprintf(result, " %s%.2f%%", arg->minArgs.icon, cpuLoad);
        
        }
        cpuWorkCache = cpuWork;
        cpuTotalCache = cpuTotal;
        return result;
}

char *
getCpuTemp(Args *arg, int flag)
{
        int temperature = 0;
        char *cpuTemp = malloc(sizeof(char) * 20);

        FILE *file = fopen("/sys/class/hwmon/hwmon0/temp1_input", "r");
        if (file == NULL) return "";

        fscanf(file, "%d", &temperature);
        fclose(file);
        int temp = temperature / 1000;

        if (flag == 0)
        {
                sprintf(cpuTemp, " %s%d°C", moduleFormatter(arg->maxArgs.lowVal, arg->maxArgs.highVal, arg->maxArgs.lowIcon, arg->maxArgs.midIcon, arg->maxArgs.highIcon, (float)temp), temp);
                return cpuTemp;
        }
        sprintf(cpuTemp, " %s%d°C", arg->minArgs.icon, temp);
        return cpuTemp;
}

static char *
moduleFormatter(int lowVal, int highVal, char *lowIcon, char *midIcon, char *highIcon, float formatVal)
{
        return formatVal < lowVal ? lowIcon : formatVal > lowVal && formatVal < highVal ? midIcon : lowIcon;
}
