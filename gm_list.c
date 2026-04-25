#include <stdlib.h>
#include "gm_list.h"
#include "gm_array2.h"
#include <assert.h>




void gmListInit(struct GmList* list)  {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

bool gmListPushBack(struct GmList* list, void* data){
    struct GmNode* node = (struct GmNode*)malloc(sizeof(struct GmNode));
    if (!node) return 0;

    node->data = data;
    node->next = NULL;

    if (!list->head) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;
    return 1;
}

bool gmListPushFront(struct GmList* list, void* data)  {
    struct GmNode* node = (struct GmNode*)malloc(sizeof(struct GmNode));
    if (!node) return 0;

    node->data = data;
    node->next = list->head;
    list->head = node;

    if (!list->tail)
        list->tail = node;

    list->size++;
    return 1;
}
void * gmListPopFront(struct GmList* list) {
    if (!list->head) return NULL;

   	struct  GmNode* node = list->head;
    void* data = node->data;

    list->head = node->next;
    if (!list->head)
        list->tail = NULL;

    free(node);
    list->size--;

    return data;
}
void gmListForeach(struct GmList* list, void (*fn)(void*)){
    for (struct GmNode* cur = list->head; cur; cur = cur->next) {
        fn(cur->data);
    }
}
void gmListFree(struct GmList* list){
    struct GmNode* cur = list->head;

    while (cur) {
        struct GmNode* next = cur->next;
        free(cur);
        cur = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}



bool gmListToGmArray(struct GmList* list, struct GmArray * out,uint32_t width,uint32_t alignment){

	assert(list && out && width >0);
		
	
       

    uint32_t count = (uint32_t)list->size;
    uint32_t total_size = (uint32_t)count * width;

    gmArrayInit(out,  width, count, alignment);

   	struct GmNode * cur = list->head;
 

    while (cur) {
		
		gmArrayPushValue(out, cur);
        cur = cur->next;
    }
    return 1;
}