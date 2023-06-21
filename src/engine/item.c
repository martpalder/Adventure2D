#include "./item.h"

Item createItem(int idx, char* name)
{
    Item item;
    item.idx = idx;
    item.name = name;
    return item;
}