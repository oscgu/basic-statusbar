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
void ut(Args *, int, char *, int);
void dm(Args *, int, char *, int);
void tm(Args *, int, char *, int);
void mm(Args *, int, char *, int);
void plm(Args *, int, char *, int);
void ptm(Args *, int, char *, int);
void nvpn(Args *, int, char *, int);
void bm(Args *arg, int flag, char *, int);
