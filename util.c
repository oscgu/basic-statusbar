#include "util.h"
#include <stdio.h>

char *
getIcon(Args *args, int value)
{
        int i;
        for (i = 0; i < args->len; i++) {
                if (args->vals[i] > value) { return args->icons[i]; }
        }

        return args->icons[args->len - 1];
}

int
read_temp(char *path)
{
        int temperature = 0;

        FILE *fp = fopen(path, "r");
        if (!fp) return -1;
        fscanf(fp, "%d", &temperature);
        fclose(fp);

        return temperature / 1000;
}
