/*
 * Config file for setting up the statusbar
 *
 */

static Module modules[] = {

    { nvpn, { 2, {0, 1}, { "", "" }}},
    { bcm, {2, {1, 2}, { "", "" }}},
    { bm,   { 4, {25, 50, 90},  { "", "", "", "" }}},
    { ut,  { 1, {0},   {" "}}},  
    //{nm, {.minArgs.icon = ""}, 1},  
    {ptm,  { 1, {0},  {" "}}},
    {plm,  { 1, {0},  {" "}}}, 
    {mm,  { 1, {0},  {"󰍛 "}}},
    {tm,  { 1, {0},  {" "}}},  
    {dm,  { 1, {0},  {" "}}},
};

/* char between modules */
static char delimitter = ' ';
