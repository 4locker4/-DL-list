#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Utils.h"
#include "Asserts.h"

const int STANDART_LIST_SIZE        = 10;

const int INDICATORS                = 0;

const int START_POS                 = 1;

const int CELL_IS_FREE              = -1;

const int POISON                    = -0xE1DA;

#define VERIFY(pointers) if (Verificator (pointers)) exit (-1);

typedef int errType;

typedef int listElem;

typedef struct
{
    listElem next      = 0;                                          // Addres next elem, head
    listElem prev      = 0;                                          // Addres previos elem, tail

    listElem list_elem = 0;                                          // Elem val
} LIST_ELEM;

typedef struct
{
    int    free = 0;                                                // Free elem in next
    size_t size = STANDART_LIST_SIZE;

    LIST_ELEM * leaf = NULL;

    bool linear_realloc = false;
} POINTERS;     // rename

#define NAME(name_elem) #name_elem

int ListDtor            (POINTERS * pointers);
int ListCtor            (POINTERS * pointers);
int GraphDump           (POINTERS * pointers, errType error);
int PhysInsertElem      (POINTERS * pointers, int add_elem, size_t anchor);
int PhysDeletElem       (POINTERS * pointers, int anchor);
int LogicalDeletElem    (POINTERS * pointers, int index);
int LogicalInsertElem   (POINTERS * pointers, listElem value, int index);
int GetElemByIndex      (POINTERS * pointers, size_t index);
int FindElemIndex       (POINTERS * pointers, listElem value);

#endif