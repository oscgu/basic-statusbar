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

static Module modules[] = {

        /* Modules        LowVal        HighVal        LowIcon          MidIcon        HighIcon        Mode*/
        { getCpuTemp, {    33,           66,             "🧊",           "🌡️",           "🔥" },        0},
        { getCpuLoad, {    33,           66,             "🧊",           "🔥",           "🧯" },        0},

        /* Modules      StaticIcon                 Mode */
        { getMem, {     .minArgs.icon = "💾" },     1},

        /* No custom formatting yet */
        { getDateTime,  {   0,            0,               "",             "",             "" }, 0},
};
