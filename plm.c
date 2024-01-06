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

        char *fmt = "%s%5.2f%%";

        char line[128];

        FILE *file = fopen("/proc/stat", "r");
        if (file == NULL) return;

        fgets(line, sizeof(line), file);
        fclose(file);

        line[strlen(line) - 1] = '\0';

        char *token = strtok(line, " ");
        while (token != NULL && i < 7) {
                cpuTotal += atoi(token);
                if (i < 4) cpuWork += atoi(token);

                token = strtok(NULL, " ");
                i++;
        }
        float cpuLoad = fabs((float) (cpuWork - cpuWorkCache) /
                             (float) (cpuTotal - cpuTotalCache) * 100);

        cpuWorkCache = cpuWork;
        cpuTotalCache = cpuTotal;

        snprintf(buff, bufflen, fmt, getIcon(arg, cpuLoad), cpuLoad);
}
