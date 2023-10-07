#include "dm.h"
#include "util.h"
#include <stdio.h>
#include <time.h>

void
dm(Args *arg, char *buff, int bufflen)
{
        char *fmt = "%s%02d.%02d.%d";

        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        snprintf(buff, bufflen, fmt, getIcon(arg, 0), t->tm_mday,
                 t->tm_mon + 1, t->tm_year + 1900);
}
