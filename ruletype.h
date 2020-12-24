#define BASE (1)
#define TYPE_STATIC      ((BASE) << 0)
#define TYPE_PWD         ((BASE) << 1)
#define TYPE_INCREASE    ((BASE) << 2)
#define TYPE_NULL        ((BASE) << 3)

#define STR_TYPE_STATIC     "Static"
#define STR_TYPE_PWD        "Pwd"
#define STR_TYPE_INCREASE   "Increase"

#define MAX_SKIP 20
typedef struct Password {
    char prefix[20];
    char postifix[20];
    char skip[MAX_SKIP];
    unsigned int length;
}Password;


typedef struct Increase {
    char* prefix;
    char* base;
    unsigned int interval;
}Increase;


typedef struct list{
    struct List* prev;
    struct List* next;
    int type;
    struct Password* pwd;
    struct Increase* inc;
    char* s_data;
}List ,*PList;

typedef struct headlist {
    PList _phead;
}HeaderList;

