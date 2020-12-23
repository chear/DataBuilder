
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cJSON.h"

#if 0  
char *message = 
"{                              \
        \"name\":\"mculover666\",   \
        \"age\": 22,                \
        \"weight\": 55.5,           \
        \"address\":                \
            {                       \
             \"country\": \"China\",\
             \"zip-code\": 111111   \
            },                      \
        \"skill\": [\"c\", \"Java\", \"Python\"],\
        \"student\": false          \
}";
#else
char *message = 
"{                                      \
        \"Static\":\"Production\",      \
        \"Pwd\":{                       \
            \"Prefix\":\"CMCC-\",       \
            \"Postifix\":\"-5G\",       \
            \"Length\": 8 ,             \
            \"SkipArry\":[\"i\",\"l\",\"2\"]    \
        },                              \
        \"Increase\": {                 \
            \"Prefix\":\"Protal\",      \
            \"Base\": \"000001\",       \
            \"Interval\":1              \
        }                               \
}";

#endif


#define BASE (1)
#define TYPE_STATIC      ((BASE) << 0)
#define TYPE_PWD         ((BASE) << 1)
#define TYPE_INCREASE    ((BASE) << 2)
#define TYPE_NULL        ((BASE) << 3)

#define MAX_SKIP 20
typedef struct Password {
    char* prefix;
    char* postifix;
    unsigned int length;
    char skip[MAX_SKIP];
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
}List ,*PList;

typedef struct headlist {
    PList _phead;
}HeaderList;
static HeaderList header;


void headerListInit(HeaderList* s) {
    printf("List Init \r\n");
    assert(s);
    s->_phead = NULL;
}

static Password* getpwdItem(cJSON* c_js){
    cJSON* item;
    struct Password* pwditem;
    if (c_js == NULL)
        return;  
    item = cJSON_GetObjectItem(c_js, "Prefix");    
    pwditem = (Password *)malloc(sizeof(Password));
//    pwditem-> prefix = item->valuestring;
    item = cJSON_GetObjectItem(c_js, "Prefix");    

    return pwditem;
}

void add2listTail(HeaderList* s, cJSON* dt_json)  {
    assert(s);
    PList new = malloc(sizeof(List));
    cJSON* temp;
    new->type = TYPE_NULL;
    if (dt_json->string !=NULL){
        if (strcmp(dt_json->string, "Pwd")){
            temp = cJSON_GetObjectItem(dt_json, "address");
            new->type = TYPE_PWD;
        }else if (strcmp(dt_json->string, "Static")){
            new->type = TYPE_STATIC;

        } else if (strcmp(dt_json->string, "Increase")){
            new->type = TYPE_INCREASE;
        }
    } 
    printf("chear:(%s)  dt_json->type = %d , new->type = %d \r\n",dt_json->string, dt_json->type, new->type);
    if (s->_phead == NULL){
        s->_phead = new;
        printf(" FIRST ADD \r\n");
    } 
    else {
        PList pnote = s->_phead;
        while (pnote->next) {
            pnote = pnote->next;
        }
        // point to last note
        pnote->next = new;
        printf(" ADD to Tail !!  \r\n");
    }

}


static void traveralJSON(cJSON* note ){
    unsigned int size = 0 , i = 0;;
    cJSON* item = NULL;
    if (note == NULL)
        return;
    
    add2listTail(&header, note);
    //printf("%s : \r\n ", note->string, note->valuestring);
    switch(note->type){
        case cJSON_String:
            printf("%s : %s \r\n",note->string, note->valuestring);
            break;
        case cJSON_Number:
            printf("%s : %d \r\n",note->string, note->valuedouble);
            break;
        case cJSON_Array:
            size = cJSON_GetArraySize(note);
            for(i=0;i<size;i++){
                item = cJSON_GetArrayItem(note, i);
                printf(" -%s-", item->valuestring);
            }
            break;
        case cJSON_Object:
        case cJSON_Raw:
        case cJSON_NULL:
            break;
        default:
            printf("defaul value for %s \r\n");
    }
    /* traveral child*/
    traveralJSON(note->child);
    /* traveral next*/
    traveralJSON(note->next);
}


int CJSON_CDECL main(void)
{
    /* print list for JSON object , and whole data within it.*/
    cJSON* cjson_test = NULL;
    cJSON* cj_child = NULL;
    cJSON* cj_next = NULL;

    cJSON* cjson_name = NULL;
    cJSON* cjson_age = NULL;
    cJSON* cjson_weight = NULL;
    cJSON* cjson_address = NULL;
    cJSON* cjson_address_country = NULL;
    cJSON* cjson_address_zipcode = NULL;
    cJSON* cjson_skill = NULL;
    cJSON* cjson_student = NULL;
    int    skill_array_size = 0, i = 0 , count = 0;
    cJSON* cjson_skill_item = NULL;
    char* str = NULL;
    
    /* print the version */
    printf("Version: %s\n", cJSON_Version());


    /* parsing JSON data. */
    cjson_test = cJSON_Parse(message);
    if(cjson_test == NULL)
    {
        printf("parse fail.\n");
        return -1;
    }
#if 0
    /* 依次根据名称提取JSON数据（键值对） */
    cjson_name = cJSON_GetObjectItem(cjson_test, "name");
    cjson_age = cJSON_GetObjectItem(cjson_test, "age");
    cjson_weight = cJSON_GetObjectItem(cjson_test, "weight");

    printf("name: %s\n", cjson_name->valuestring);
    printf("age:%d\n", cjson_age->valueint);
    printf("weight:%.1f\n", cjson_weight->valuedouble);

    /* 解析嵌套json数据 */
    cjson_address = cJSON_GetObjectItem(cjson_test, "address");
    cjson_address_country = cJSON_GetObjectItem(cjson_address, "country");
    cjson_address_zipcode = cJSON_GetObjectItem(cjson_address, "zip-code");
    printf("address-country:%s\naddress-zipcode:%d\n", cjson_address_country->valuestring, cjson_address_zipcode->valueint);

    /* parsing 'array' */
    cjson_skill = cJSON_GetObjectItem(cjson_test, "skill");
    skill_array_size = cJSON_GetArraySize(cjson_skill);
    printf("skill:[");
    for(i = 0; i < skill_array_size; i++)
    {
        cjson_skill_item = cJSON_GetArrayItem(cjson_skill, i);
        printf("%s,", cjson_skill_item->valuestring);

    }
    printf("\b]\n");

    /* parsing 'bool' item */
    cjson_student = cJSON_GetObjectItem(cjson_test, "student");
    if(cjson_student->valueint == 0)
    {
        printf("student: false\n");

    }
    else
    {
        printf("student:error\n");
    }
#else

    headerListInit( &header); 
    traveralJSON(cjson_test);

    printf("Start to print List !!!!\r\n");
    HeaderList *ph = &header;
    PList note = ph->_phead;
    if (note == NULL) 
        printf("list its null \r\n");
    else {
        while(note->next){
            printf("(%d) note->type = %d\r\n",count,note->type);
            note = note->next;
            count ++;        
        }
    }
#endif

    return 0;

}



