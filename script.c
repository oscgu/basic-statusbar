#include "util.h"
#include <stdio.h>

void
script(Args *arg, char *buff, int bufflen)
{
        char *script = arg->icons[0];

        FILE *f = popen(script, "r");
        if (f == NULL) {
                printf("failed to run script at: %s", script);
                return;
        }

        fgets(buff, bufflen, f);
        pclose(f);
}
