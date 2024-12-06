#include "../inc/header.h"

int main ()
{
    POINTERS pointers = {};

    ListCtor (&pointers);
    
    for (int i = 0; STANDART_LIST_SIZE * 2 > i; i++)
    {
        PhysInsertElem (&pointers, i * 10, i);
    }

    PhysInsertElem (&pointers, 888, 8);

    PhysDeletElem (&pointers, 7);
    PhysDeletElem (&pointers, 9);
    PhysDeletElem (&pointers, 13);

    LogicalDeletElem (&pointers, 7);

    ListDtor  (&pointers);
}