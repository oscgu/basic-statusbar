#include "plm.h"
#include "util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long int cpuWorkCache = 0;
static long int cpuTotalCache = 0;

void
plm(Args *arg, char *buff, int bufflen)
{
        long int cpuTotal = 0;
        long int cpuWork = 0;
        int i = 0;

        char *fmt = "%s%3.2f%%";

        char line[1][128];

        FILE *file = fopen("/proc/stat", "r");
        if (file == NULL) return;

        fgets(line[0], 100, file);
        line[0][strlen(line[0]) - 1] = '\0';
        fclose(file);

        char *token = strtok(line[0], " ");

        while (token != NULL) {
                i++;
                token = strtok(NULL, " ");
                if (i > 0 && i < 7) { cpuTotal += atoi(token); }
                if (i > 0 && i < 4) { cpuWork += atoi(token); }
        }
        float cpuLoad = fabs((float) (cpuWork - cpuWorkCache) /
                             (float) (cpuTotal - cpuTotalCache) * 100);

        cpuWorkCache = cpuWork;
        cpuTotalCache = cpuTotal;

        snprintf(buff, bufflen, fmt, getIcon(arg, cpuLoad), cpuLoad);
}
