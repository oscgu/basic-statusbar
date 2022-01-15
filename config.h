/*
 * Config file for setting up the statusbar
 *
 * Multi-Icon Status
 * LowVal: Everything below it gets the LowIcon
 * HighVal: Everything above it gets the HighIcon
 * Values between LowVal and HighVal get the MidIcon
 * Mode: 0
 *
 * Example:
  { ptm, {            33,           66,             "🧊",           "🌡️",           "🔥" },        0},
 *
 * Static-Icon Status
 * .minArgs.icon = "your Icon or text"
 * Mode: 1
 *
 * Example:
  { mm, {   .minArgs.icon = "Memory: " },       1},
 */

static Module modules[] = {

        /* Modules      StaticIcon     Mode */
        { nvpn, { .minArgs.icon = "" }, 1},
        { ptm,  { .minArgs.icon = "" }, 1},
        { plm,  { .minArgs.icon = "" }, 1},
        { mm,   { .minArgs.icon = "" }, 1},
        { tm,   { .minArgs.icon = "" }, 1},
        { dm,   { .minArgs.icon = "" }, 1},
};

/* char between modules */
static char delimitter = ' ';
