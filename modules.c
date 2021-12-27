#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void
getDateTime(char *dateTime)
{
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        strftime(dateTime, 35, " ⌛%H:%M 🗓️%d-%m-%Y", t);
}

void
getMem(char *memUsage)
{
        char buffer[1024] = "";
        char result[12];
        int memTotal = 0;
        int memAvailable = 0;

        FILE *file = fopen("/proc/meminfo", "r");

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
        sprintf(memUsage, " 💾%.2fGb", (float)(memTotal - memAvailable) * 1e-6);
}

void
getLoadAvg(char *loadavg)
{
        char result[10] = " 🖥️";
        int n = 5;
        int i;

        FILE *file = fopen("/proc/loadavg", "r");
        
        while ((i = fgetc(file)) != ' ')
        {
                result[n++] = (char) i;
        }
        fclose(file);
        strcpy(loadavg, result);
}

void
getCpuLoad(char *cpuCurrentLoad, long int *cpuWorkCache, long int *cpuTotalCache)
{
        long int cpuTotal = 0;
        long int cpuWork = 0;
        int i=0;

        char result[20];
        char line[1][128];

        FILE *file = fopen("/proc/stat", "r");

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
        float cpuLoad = fabs((float)(cpuWork - *cpuWorkCache) / (float)(cpuTotal - *cpuTotalCache) * 100);

        sprintf(cpuCurrentLoad, " %s%.2f%%",  cpuLoad < 60 ? "🧊": "🔥", cpuLoad );
        *cpuWorkCache = cpuWork;
        *cpuTotalCache = cpuTotal;
}
