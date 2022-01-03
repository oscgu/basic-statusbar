/* structs */
typedef struct {
        char *icon;
} MinimalArgs;

typedef struct {
        int lowVal;
        int highVal;
        char *lowIcon;
        char *midIcon;
        char *highIcon;
} MaxArgs;

typedef struct {
        union {
                MaxArgs maxArgs;
                MinimalArgs minArgs;
        };
        int flag;
} Args;

/* function declarations */
char *getDateTime(Args *, int);
char *getMem(Args *, int);
char *getCpuLoad(Args *, int);
char *getCpuTemp(Args *, int);
