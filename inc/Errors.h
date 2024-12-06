///@file Errors.h
#ifndef ERROR_H
#define ERROR_H

char * errors_interp[] = {
                                 "Pointer to list is bad\n",
                                 "Pointer to struct is bad\n",
                                 "Free is out of size\n",
                                 "Size is under 0\n",
                                 "Wrong connection between next and previous elements\n",
                                 "Next is out of size\n",
                                 "Tail is out of size\n",
                                 "Head is under of size\n",
                                 "Bed previous sell\n"
                                };

const char * INITED_CELL_COLOR      = "#C9FEB9";
const char * NOT_INITED_CELL_COLOR  = "#A1A1A1";
const char * SMTHNG_BAD_COLOR       = "#CC0605";
const char * SPECIAL_COLOR          = "#6495ED";

const char * GRAPH_DUMP_FILE        = "../List/logs/graphdump.dot";
const char * DUMP_FILE              = "../List/logs/dump.txt";

enum ERRORS
{
    OK                   = 0,
    LIST_IS_NULL         = 1 << 0,
    STRUCT_ARRAY_IS_NULL = 1 << 1,
    FREE_OUT_OF_LIST     = 1 << 2,
    WRONG_SIZE           = 1 << 3,
    WRONG_NEXT_PREV_CONN = 1 << 4, 
    NEXT_OUT_OF_SIZE     = 1 << 5,
    TAIL_OUT_OF_SIZE     = 1 << 6,
    HEAD_IS_UNDER_LIST   = 1 << 7,
    BED_CELL_PREV        = 1 << 8
};

enum ERRORS_IN_ARRAY
{
    NULL_LIST = 0,
    NULL_ARRAY = 1,
    FREE_OUT,
    SIZE_WRONG,
    NEXT_PREV_WRONG,
    NEXT_OUT,
    TAIL_OUT,
    HEAD_UNDER,
    BED_CELL
};

#endif