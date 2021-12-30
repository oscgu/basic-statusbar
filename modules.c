#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* variables */
long int cpuWorkCache = 0;
long int cpuTotalCache = 0;

char *
getDateTime(void)
{
        char *dateTime = malloc(35);
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        sprintf(dateTime, " %s%02d:%02d %s%d-%d-%d",t->tm_hour >= 22 ? "🌙" : t->tm_hour < 12 ? "☕" :"🌞",  t->tm_hour, t->tm_min,(t->tm_mon + 1) == 12 ? "🎄" : "🗓️", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);

        return dateTime;
}

char *
getMem(void)
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
        sprintf(memUsage, " 💾%.2fGb", (float)(memTotal - memAvailable) * 1e-6);

        return memUsage;
}

char *
getLoadAvg(void)
{
        char result[10] = " 🖥️";
        char *loadavg = malloc(sizeof(char) * 20);
        int n = 5;
        int i;

        FILE *file = fopen("/proc/loadavg", "r");
        if (file == NULL) return "";
        
        while ((i = fgetc(file)) != ' ')
        {
                result[n++] = (char) i;
        }
        fclose(file);
        strcpy(loadavg, result);
        return loadavg;
}

char *
getCpuLoad(void)
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

        sprintf(result, " %s%.2f%%",  cpuLoad <= 30 ? "🧊": cpuLoad < 80 ? "🔥" : "🧯", cpuLoad );
        cpuWorkCache = cpuWork;
        cpuTotalCache = cpuTotal;

        return result;
}

char *
getCpuTemp(void)
{
        int temperatue = 0;
        char *cpuTemp = malloc(sizeof(char) * 20);

        FILE *file = fopen("/sys/class/hwmon/hwmon0/temp1_input", "r");
        if (file == NULL) return "";

        fscanf(file, "%d", &temperatue);
        sprintf(cpuTemp, " %s%d", temperatue / 1000 <= 30 ? "🧊": temperatue < 80 ? "🌡️" : "🔥", temperatue / 1000);

        fclose(file);
        return cpuTemp;
}
