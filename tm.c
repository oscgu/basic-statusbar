#include "tm.h"
#include "util.h"
#include <stdio.h>
#include <time.h>

void
tm(Args *arg, char *buff, int bufflen)
{
        char *fmt = "%s%02d:%02d";

        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        snprintf(buff, bufflen, fmt, getIcon(arg, 0), t->tm_hour, t->tm_min);
}
