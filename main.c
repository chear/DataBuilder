
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cJSON.h"
#include "ruletype.h"
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


static HeaderList header;


void headerListInit(HeaderList* s) {
    printf("List Init \r\n");
    assert(s);
    s->_phead = NULL;
}

static void getpwdItem(Password* pd, cJSON* c_js){
    unsigned int size = 0 , i = 0;
    cJSON* item = NULL;
    cJSON* ary = NULL;
    char skp[MAX_SKIP] = {0};
    if (c_js == NULL)
        return;  
    if(pd == NULL){
        pd = (Password *)malloc(sizeof(Password));
        printf(" Password is null, re-malloc \n");
    }

    item = cJSON_GetObjectItem(c_js, "Prefix");
    memset(pd->prefix, 0 ,sizeof(pd->prefix));
    strcpy(pd->prefix , item->valuestring);

//    item = cJSON_GetObjectItem(c_js, "Postifix"); 
//    memset(pd->postifix, 0 ,sizeof(pd->postifix));
//    strcpy(pd->postifix , item->valuestring);
//
//    item = cJSON_GetObjectItem(c_js, "Length");
//    pd->length = item->valuedouble;
//
//    item = cJSON_GetObjectItem(c_js, "SkipArry");
//    memset(pd->skip, 0 ,sizeof(pd->skip));
//    size = cJSON_GetArraySize(item);
//    for(i=0;i<size;i++){
//        ary = cJSON_GetArrayItem(item, i);
//        printf(" -%s-", ary->valuestring);
// //       skp[i] = (char)ary->valuestring;
////        pd->skip[i] = skp[i];
//    }
    free(pd);
}

void add2listTail(HeaderList* s, cJSON* dt_json, int type)  {
    assert(s);
    PList new = malloc(sizeof(List));
    cJSON* temp,item;
    printf("add2listTail calling \r\n");
    new->type = type;
 
    printf("chear:(%s)  dt_json->type = %d , new->type = %d",dt_json->string, dt_json->type, new->type);
    
    if (type == TYPE_STATIC) {
        new->s_data = dt_json->valuestring;
    } else if (type == TYPE_PWD) {
        Password* pd = (Password *)malloc(sizeof(Password));
        new->pwd = pd;
        getpwdItem(pd, dt_json);
    } else if (type == TYPE_INCREASE){
        /* get & set 'Increase' data */
    }


//    switch(dt_json->type){
//        case cJSON_String:
////            printf("%s : %s \r\n",dt_json->string, dt_json->valuestring);
//            break;
//        case cJSON_Number:
////            printf("%s : %d \r\n", dt_json->string, dt_json->valuedouble);
//            break;
//        case cJSON_Array:
////            size = cJSON_GetArraySize(dt_json);
////            for(i=0;i<size;i++){
////                item = cJSON_GetArrayItem(dt_json, i);
////                printf(" -%s-", item->valuestring);
////            }
//            break;
//        case cJSON_Object:
//        case cJSON_Raw:
//        case cJSON_NULL:
//            break;
//        default:
////            printf("defaul value for %s \r\n");
//            break;
//    }

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

    /* Only operate data format like 'Static', 'Pwd', 'Increase' */
    printf("traveral %s : %s \r\n ", note->string, note->valuestring);
    if (note->string != NULL) {
        if (strcmp(note->string, STR_TYPE_PWD) ==0 ){
            add2listTail(&header, note, TYPE_PWD);
        }
        else if (strcmp(note->string, STR_TYPE_STATIC) ==0 ){
            add2listTail(&header, note, TYPE_STATIC);
        }
        else if (strcmp(note->string, STR_TYPE_INCREASE) ==0 ){
            add2listTail(&header, note, TYPE_INCREASE);
        }
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
    Password* pd;
   
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
    /* init */
    headerListInit( &header); 
    /* parsring json */ 
    traveralJSON(cjson_test);

    printf("Start to print List !!!!\r\n");
    PList note = ((HeaderList *)&header)->_phead;
        
    if (note == NULL) 
        printf("list its null \r\n");
    else {
        while(note){
            printf("(%d) note->type = %d",count,note->type);
            switch(note->type){
                case TYPE_STATIC:
                    printf(" value = %s \r\n",note->s_data);
                    break;
                case TYPE_PWD:
                    pd = note->pwd;
                    printf(" value = \"%s\" , \"%s\" , %d \r\n",pd->prefix, pd->postifix , pd->length);
                    break;
            }
            note = note->next;
            printf("xxxx\r\n");
            count ++;        
        }
    }
#endif

    return 0;

}



