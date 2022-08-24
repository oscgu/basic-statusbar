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
char *ut(Args *, int);
char *dm(Args *, int);
char *tm(Args *, int);
char *mm(Args *, int);
char *plm(Args *, int);
char *ptm(Args *, int);
char *nvpn(Args *, int);
char *bm(Args *arg, int flag);
