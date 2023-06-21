#ifndef ITEM_H
#define ITEM_H

typedef struct
{
    int idx;
    char* name;
} Item;

Item createItem(int idx, char* name);

#endif