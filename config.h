#include "nvpn.h"
#include "bcm.h"
#include "bm.h"
#include "ut.h"
#include "ptm.h"
#include "plm.h"
#include "mm.h"
#include "tm.h"
#include "dm.h"
#include "gtm.h"

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
    { nvpn, { 2, {1, 2}, { "", "" }}},
    //{ bcm, {2, {1, 2}, { "", "" }}},
    //{ bm,   { 4, {25, 50, 90},  { "", "", "", "" }}},
    { ut,  { 1, {0},   {" "}}},  
    //{nm, {.minArgs.icon = ""}, 1},  
    {ptm,  { 1, {0},  {" "}}},
    //{gtm, {1, {0}, {"GPU: "}}},
    {plm,  { 1, {0},  {" "}}}, 
    {mm,  { 1, {0},  {"󰍛 "}}},
    {tm,  { 1, {0},  {" "}}},  
    {dm,  { 1, {0},  {" "}}},
};

/* char between modules */
static char delimitter = ' ';
