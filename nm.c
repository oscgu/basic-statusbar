#include "arg.h"
#include <stdio.h>
#include "nm.h"

#define TO_KBITS(b) ((float)(b) / 125)

static unsigned long long int netReCache = 0;
static unsigned long long int netSeCache = 0;

void
nm(Args *arg, char *buff, int bufflen)
{
    (void)arg;
    unsigned long long int bytes_re, bytes_se;
    char *fmt = "⬆ %.2fKi ⬇ %.2fKi";

    FILE *fr = fopen("/sys/class/net/eth0/statistics/rx_bytes", "r");
    if (!fr) return;

    fscanf(fr, "%llu", &bytes_re);
    fclose(fr);

    FILE *fs = fopen("/sys/class/net/eth0/statistics/tx_bytes", "r");
    if (!fs) return;

    fscanf(fs, "%llu", &bytes_se);
    fclose(fs);

    unsigned long long int reDiff = bytes_re - netReCache;
    unsigned long long int seDiff = bytes_se - netSeCache;

    snprintf(buff, bufflen, fmt, TO_KBITS(seDiff), TO_KBITS(reDiff));

    netReCache = bytes_re;
    netSeCache = bytes_se;
}

