#include "bcm.h"
#include "bm.h"
#include "dm.h"
#include "gtm.h"
#include "mm.h"
#include "nvpn.h"
#include "plm.h"
#include "ptm.h"
#include "tm.h"
#include "ut.h"
#include "script.h"

/* structs */
typedef struct {
        void (*func)(Args *, char *, int);
        Args args;
} Module;

/*
 * Config for setting up the statusbar
 *
 */
static Module modules[] = {
    //{ nvpn, { 2, {1, 2}, { "", "" }}},
    //{ bcm, {2, {1, 2}, { "", "" }}},
    //{ bm,   { 4, {25, 50, 90},  { "", "", "", "" }}},
    {script, {0, {0}, {"/home/og/script.sh"}}},
    {ut, {1, {0}, {" "}}},
    //{nm, {.minArgs.icon = ""}, 1},
    {ptm, {1, {0}, {" "}}},
    {gtm, {1, {0}, {"󰢮  "}}},
    {plm, {1, {0}, {" "}}},
    {mm, {1, {0}, {"󰍛 "}}},
    {tm, {1, {0}, {" "}}},
    {dm, {1, {0}, {" "}}},
};

/* char between modules */
static char delimitter = ' ';

/*  */
static int refresh_interval_s = 1;
