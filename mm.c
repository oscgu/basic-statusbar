#include "mm.h"
#include "util.h"
#include <stdio.h>
#include <string.h>

void mm(Args *arg, char *buff, int bufflen) {
    char buffer[256];
    int memTotal = 0, memAvailable = 0;
    FILE *file = fopen("/proc/meminfo", "r");
    if (file == NULL) return;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strncmp(buffer, "MemTotal:", 9) == 0) {
            sscanf(buffer + 9, "%d", &memTotal);
        } else if (strncmp(buffer, "MemAvailable:", 13) == 0) {
            sscanf(buffer + 13, "%d", &memAvailable);
        }
    }
    fclose(file);

    float usage = (float)(memTotal - memAvailable) / 1024.0 / 1024.0; // Convert to GB

    snprintf(buff, bufflen, "%s%4.2fGb", getIcon(arg, usage), usage);
}
