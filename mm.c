#include <stdio.h>
#include <string.h>
#include "util.h"
#include "mm.h"

void
mm(Args *arg, char *buff, int bufflen)
{
        char buffer[1024] = "";

        char *fmt = "%s%3.2fGb";

        int memTotal = 0;
        int memAvailable = 0;

        FILE *file = fopen("/proc/meminfo", "r");
        if (file == NULL) return;

        while (fscanf(file, " %1023s", buffer) == 1) {
                if (strcmp(buffer, "MemTotal:") == 0) {
                        fscanf(file, " %d", &memTotal);
                }
                if (strcmp(buffer, "MemAvailable:") == 0) {
                        fscanf(file, " %d", &memAvailable);
                }
        }
        fclose(file);

        float usage = (float) (memTotal - memAvailable) * 1e-6;

        snprintf(buff, bufflen, fmt,
                 getIcon(arg, usage), usage);
}

