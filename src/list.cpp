#include "../inc/header.h"
#include "../inc/Errors.h"

static int FillList    (POINTERS * pointers, size_t start_pos);
static int Verificator (POINTERS * pointers);
static int ListRealloc (POINTERS * pointers);

int ListCtor (POINTERS * pointers)
{
    pointers->leaf = (LIST_ELEM *) calloc (STANDART_LIST_SIZE, sizeof (LIST_ELEM));
    my_assert (pointers->leaf);

    pointers->size = STANDART_LIST_SIZE;

    FillList (pointers, START_POS);

    return 0;
}

int FillList (POINTERS * pointers, size_t start_pos)
{
    my_assert (pointers);

    for (int i = start_pos; pointers->size > i; i++)
    {
        pointers->leaf[i].list_elem = POISON;
        pointers->leaf[i].next      = i + 1;
        pointers->leaf[i].prev      = -1;
    }

    pointers->leaf[INDICATORS].list_elem        = POISON;
    pointers->leaf[INDICATORS].prev             = pointers->size - 1;
    pointers->leaf[STANDART_LIST_SIZE - 1].next = START_POS;

    pointers->free = start_pos;

    return 0;
}

int ListDtor (POINTERS * pointers)
{
    VERIFY (pointers)
    
    free (pointers->leaf);

    pointers->leaf = NULL;
    pointers->size = 0;
    pointers->free = 0;
    pointers->linear_realloc = false;

    return 0;
}

int PhysInsertElem (POINTERS * pointers, int add_elem, size_t anchor)
{
    VERIFY (pointers)

    int free_cell = pointers->free;

    pointers->free = pointers->leaf[free_cell].next;

    pointers->leaf[free_cell].list_elem = add_elem;

    pointers->leaf[pointers->leaf[anchor].next].prev = free_cell;

    pointers->leaf[free_cell].next = pointers->leaf[anchor].next;

    pointers->leaf[free_cell].prev = anchor;

    pointers->leaf[anchor].next = free_cell;

    if (pointers->leaf[pointers->free].prev != -1)
        ListRealloc (pointers);

    VERIFY (pointers)

    return 0;
}

int PhysDeletElem (POINTERS * pointers, int anchor)
{
    VERIFY (pointers)

    int next_free_cell = pointers->free;

    pointers->free = pointers->leaf[anchor].next;

    pointers->leaf[anchor].next = pointers->leaf[pointers->free].next;

    pointers->leaf[pointers->leaf[anchor].next].prev = anchor;

    pointers->leaf[pointers->free].list_elem = POISON;
    pointers->leaf[pointers->free].prev      = CELL_IS_FREE;
    pointers->leaf[pointers->free].next      = next_free_cell;

    VERIFY (pointers)

    return 0;
}

int LogicalInsertElem (POINTERS * pointers, listElem value, int index)
{
    VERIFY (pointers)

    int ip = GetElemByIndex (pointers, index);

    LogicalInsertElem (pointers, value, ip);

    VERIFY (pointers)

    return 0;
}

int LogicalDeletElem (POINTERS * pointers, int index)
{
    VERIFY (pointers)

    int ip = GetElemByIndex (pointers, index);

    LogicalDeletElem (pointers, ip);

    VERIFY (pointers)

    return 0;
}

int FindElemIndex (POINTERS * pointers, listElem value)
{
    VERIFY (pointers)

    for (int ip = 0; pointers->size > ip; )
    {
        if (pointers->leaf[ip].list_elem == value)
            return ip;
        ip = pointers->leaf[ip].next;
    }

    return 0;
}

int GetElemByIndex (POINTERS * pointers, size_t index)
{
    VERIFY (pointers)

    int ip = 0;

    if (pointers->linear_realloc)
    {
        ip = index;
    }
    else
    {
        if (index == 0 || index == 1)
            ip = index;
        else
        {
            for (int i = 0; index - 1 > i; i++)
                ip = pointers->leaf[ip].next;
        }
    }

    VERIFY (pointers)

    return ip;
}

int ListRealloc (POINTERS * pointers)
{
    size_t old_size = pointers->size;

    if (!(abs(pointers->leaf[pointers->free].next + pointers->leaf[INDICATORS].prev) % pointers->size))
        pointers->size *= 2;
    else
    {
        LinearRealloc (pointers);
        pointers->linear_realloc = true;
    }
    
    pointers->leaf = (LIST_ELEM *) realloc (pointers->leaf, pointers->size * sizeof (LIST_ELEM));
    my_assert (pointers->leaf);

    FillList (pointers, old_size);

    VERIFY (pointers)

    return 0;
}


int LinearRealloc (POINTERS * pointers)
{
    VERIFY (pointers)

    size_t ip = 0;

    while (pointers->free > pointers->leaf[ip].next)
        ip++;
    
    while (pointers->free < pointers->leaf[ip].next)
    {
        size_t buf = pointers->leaf[pointers->free].next;

        pointers->leaf[pointers->free].list_elem = pointers->leaf[ip].list_elem;
        pointers->leaf[pointers->free].next = pointers->leaf[ip].next;
        pointers->leaf[pointers->free].prev = pointers->leaf[ip].prev;

        pointers->leaf[pointers->leaf[pointers->free].prev].next = pointers->free;

    }

    VERIFY (pointers)
}

int ValElemSwap (POINTERS * pointers, listElem first_cell, listElem second_cell)
{
    VERIFY (pointers)

    listElem buf = pointers->leaf[first_cell].list_elem;

    pointers->leaf[first_cell].list_elem = pointers->leaf[second_cell].list_elem;

    pointers->leaf[second_cell].list_elem = buf;

    VERIFY (pointers)

    return 0;
}

int CellSwap (POINTERS * pointers, listElem first_cell, listElem second_cell)
{
    VERIFY (pointers)
}

int Verificator (POINTERS * pointers)
{
    errType error = 0;

    if (!pointers)
    {
        error |= LIST_IS_NULL;
        return error;
    }

    if (!pointers->leaf)
    {
        error |= STRUCT_ARRAY_IS_NULL;
        return error;
    }

    if (pointers->size < 0)
        error |= WRONG_SIZE;

    if (pointers->free >= pointers->size)
    {
        error |= FREE_OUT_OF_LIST;
        Dump (pointers, error);
        return error;
    }
    
    if (pointers->leaf[INDICATORS].next >= pointers->size || pointers->leaf[INDICATORS].next < 0)
        error |= TAIL_OUT_OF_SIZE;
    
    if (pointers->leaf[INDICATORS].prev < 0)
        error |= HEAD_IS_UNDER_LIST;

    for (int i = 1; pointers->free > i; i++)
    {
        if (pointers->leaf[i].next >= pointers->size)
            error |= NEXT_OUT_OF_SIZE;
    }

    int counter = pointers->free;

    if (error)
        GraphDump (pointers, error);

    return error;
}

int Dump (POINTERS * pointers, errType error)
{
    FILE * dump_file = fopen (DUMP_FILE, "a+");

    if (!dump_file)
    {
        printf ("Couldn`t open dump file to write errors\n");
        return 0;
    }

    if (error & LIST_IS_NULL)
    {
        
    }
}

int GraphDump (POINTERS * pointers, errType error)
{
    if (pointers == NULL)
    {
        COLOR_PRINT (RED, "Pointer to struct 'pointers' is NULL\n");
        return 0;
    }

    FILE * dump_file = fopen (GRAPH_DUMP_FILE, "w+");
    my_assert (dump_file);

/*  write a head   */
    fprintf (dump_file, "digraph list{\n"
                        "\trankdir=LR;\n"
                        "\tbgcolor=\"#EEBEF1\";"
                        "\tnode[color=\"black\", fontsize=14];\n"
                        "\tfixedsize=true;\n");

    int last_inited = pointers->size - 1;

    if (error & LIST_IS_NULL)
    {
        fprintf (dump_file, "%s\n", errors_interp[LIST_IS_NULL]);
        return 0;
    }

    if (error & STRUCT_ARRAY_IS_NULL)
    {

    }

    for (int i = 1; pointers->size - 1 > i; i++)
    {
        if (pointers->leaf[i + 1].prev == CELL_IS_FREE)
        {
            last_inited = i++;

            while (pointers->leaf[i].prev == CELL_IS_FREE)
            {
                fprintf (dump_file, "\t%d [style=filled, shape=Mrecord, fillcolor=\"%s\", width=2, label =\""
                                    "{№%d | {{list elem | %d} | {next | <f-1> %d} | {prev | %d}}}\" ]\n", i, NOT_INITED_CELL_COLOR, i,
                                    pointers->leaf[i].list_elem, pointers->leaf[i].next, pointers->leaf[i].prev);
                i++;
            }

            if (pointers->size > i)
                fprintf (dump_file, "\t%d -> %d\n", last_inited, pointers->leaf[last_inited].next);
            
            i--;
        }
        else 
        {
            fprintf (dump_file, "\t%d -> %d\n", i, pointers->leaf[i].next);
        }
    }

    fprintf (dump_file, "\t%s [style=filled, shape=record, fillcolor=\"%s\", width=2, label =\""
                        "{%s | {{list elem | %s} | {free_elem | <f-1> %d}}}\" ]\n", "FREE", SPECIAL_COLOR,
                        "FREE", "free elem", pointers->free);
    fprintf (dump_file, "\t%s -> %d\n", "FREE", pointers->free);

    fprintf (dump_file, "\t%d: <f-1> -> %d [weight=1000];\n", last_inited, pointers->leaf[last_inited].next);
    fprintf (dump_file, "\n");

    fprintf (dump_file, "\t%s [style=filled, shape=record, fillcolor=\"%s\", width=2, label =\""
                        "{%s | {{list elem | %s} | {next | <f-1> %d} | {prev | %d}}}\" ]\n", "INDICATORS", SPECIAL_COLOR,
                        "INDICATORS", "elda", pointers->leaf[INDICATORS].next, pointers->leaf[INDICATORS].prev);

    for (int i = 1; pointers->size > i; i++)
    {
        if (pointers->leaf[i].prev != CELL_IS_FREE)
        {
            fprintf (dump_file, "\t%d [style=filled, shape=Mrecord, fillcolor=\"%s\", width=2, label =\""
                                "{№%d | {{list elem | %d} | {next | <f-1> %d} | {prev | %d}}}\" ]\n", pointers->leaf[pointers->leaf[i].prev].next, INITED_CELL_COLOR, i,
                                pointers->leaf[i].list_elem, pointers->leaf[i].next, pointers->leaf[i].prev);
            if (pointers->leaf[pointers->leaf[i].prev].next != pointers->leaf[pointers->leaf[i].next].prev)
                fprintf (dump_file, "\tedge[color=\"%s\"] %d: <f-1> -> %d [weight=1000];\n\tedge[color=\"black\"]\n",
                                     SMTHNG_BAD_COLOR, pointers->leaf[pointers->leaf[i].prev].next, pointers->leaf[i].prev);
        }
    }

    if (pointers->leaf[pointers->leaf[INDICATORS].next].prev != -1)
        fprintf (dump_file, "\t%s: <f-1> -> %d [weight=1000];\n", "INDICATORS", pointers->leaf[INDICATORS].next);

    fprintf (dump_file, "}");

    my_assert (!fclose (dump_file));

    system ("dot -Tsvg ../List/logs/dump.dot -o ../List/logs/dump.svg");

    size_t file_size = 0;

    char * svg_text = FileToStr ("../List/logs/dump.svg", &file_size);

    FILE * debug_html = fopen ("../List/logs/dump_html.html", "a+");

    if (error & LIST_IS_NULL)
        fprintf (debug_html, "%s\n", errors_interp[NULL_LIST]);
    if (error & STRUCT_ARRAY_IS_NULL)
        fprintf (debug_html, "%s\n", errors_interp[NULL_ARRAY]);
    if (error & FREE_OUT_OF_LIST)
        fprintf (debug_html, "%s\n", errors_interp[FREE_OUT]);
    if (error & WRONG_SIZE)
        fprintf (debug_html, "%s\n", errors_interp[SIZE_WRONG]);
    if (error & WRONG_NEXT_PREV_CONN)
        fprintf (debug_html, "%s\n", errors_interp[NEXT_PREV_WRONG]);
    if (error & NEXT_OUT_OF_SIZE)
        fprintf (debug_html, "%s\n", errors_interp[NEXT_OUT]);
    if (error & TAIL_OUT_OF_SIZE)
        fprintf (debug_html, "%s\n", errors_interp[TAIL_OUT]);
    if (error & HEAD_IS_UNDER_LIST)
        fprintf (debug_html, "%s\n", errors_interp[HEAD_UNDER]);
    if (error & BED_CELL_PREV)
        fprintf (debug_html, "%s\n", errors_interp[BED_CELL]);

    fwrite (svg_text, sizeof (char), file_size, debug_html);

    my_assert (!fclose (debug_html));

    return 0;
}
