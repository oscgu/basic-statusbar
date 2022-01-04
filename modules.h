/* structs */
typedef struct {
        char *datearr;
        char *icon;
} Date;

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
                Date *dates[10];
        };
        int flag;
} Args;

/* function declarations */
char *dm(Args *, int);
char *tm(Args *, int);
char *mm(Args *, int);
char *plm(Args *, int);
char *ptm(Args *, int);
