#pragma once

#include <stdint.h>
#include "gm_array2.h"

 struct GmNode {
    void* data;
    struct GmNode* next;
} ;

 struct GmList{
   struct GmNode* head;
   struct GmNode* tail;
    uint32_t size;
} ;

void gmListInit(struct GmList* list) ;
bool gmListPushBack(struct GmList* list, void* data);

bool gmListPushFront(struct GmList* list, void* data);
void * gmListPopFront(struct GmList* list);
void gmListForeachFn(struct GmList* list, void (*fn)(void*));

void gmListFree(struct GmList* list);

bool gmListToGmArray(struct GmList* list, struct GmArray * out,uint32_t width,uint32_t alignment);