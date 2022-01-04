/*
 * Config file for setting up the statusbar
 *
 * Multi-Icon Status
 * LowVal: Everything below it gets the LowIcon
 * HighVal: Everything above it gets the HighIcon
 * Values between LowVal and HighVal get the MidIcon
 * Mode: 0
 *
 * Static-Icon Status
 * .minArgs.icon = "your Icon or text"
 * Mode: 1
 *
 */
static Date datearr[10] = {
        /*      Date    Icon */
        { "24.12.2022", "🎅" }
};

static Module modules[] = {

        /* Modules        LowVal        HighVal        LowIcon          MidIcon        HighIcon        Mode */
        { ptm, {            33,           66,             "🧊",           "🌡️",           "🔥" },        0},
        { plm, {            33,           66,             "🧊",           "🔥",           "🧯" },        0},

        /* Modules      StaticIcon                                                                     Mode */
        { mm, {   .minArgs.icon = "💾" },                                                                1},
        { tm, {             10,            19,             "☕",           "⏳",           "🌙" },       0},
        /* Modules      Dates */
        { dm, {   .minArgs.icon = "🗓️" },                                                                1},
};

/* .dates = datearr */

/* char between modules */
static char delimitter = ' ';
