#include <stdio.h>
#include "list.h"
#include "list_types.h"

#define SIZE 16


static list_type lst; /* word list (in array form) */
static buf_type buf; /* buffer that stores current word*/

static int sizebuf;  /* buffer size of current word*/
static int sizelist; /* size of current word list (equals to the number of words it can store)*/

static int curbuf;   /* index of letter we are about to write (equals to number of letters stored)*/
static int curlist;  /* index of current word in the list(equals to the number of words
                                            already stored in the list*/

//procedure from sort.c file
void sort(list_type *list,int arr_len);


//removes all list data from memory, sets global variables related to list parameters to 0 
void clear_list() {
    int i;
    
    if (lst == NULL)
        return;
    //deleting words first
    for (i = 0; lst[i] != NULL; i++)
        free(lst[i]);

    //deleting the block in which list is stored
    free(lst);

    null_list();
    return;
}

void clear_buf(){
    if (buf != NULL)
        free(buf);

    null_buf();
    return;
}

// nulls all buf params
void null_buf() {
    buf = NULL;
    sizebuf = 0;
    curbuf = 0;
    return;
}


//sets global variables related to list params to 0 
void null_list() {
    sizelist = 0;
    curlist = 0;
    lst = NULL;
    return;
}

// puts null at the end of the list
void term_list() {
    if (lst == NULL)
        return;

    //if there is insufficient space to put NULL at the end
    if (curlist > sizelist - 1)
        lst = realloc(lst, (sizelist + 1) * sizeof(*lst));
    lst[curlist] = NULL;
    //cutting loose bytes at the end of the list
    lst = realloc(lst, (sizelist = curlist + 1) * sizeof(*lst));
    return;
}

//adds symbol to current word, if where is insufficient space increases buf by SIZE bytes
void add_sym(char c) {
    if (curbuf > sizebuf - 1)
        buf = realloc(buf, sizebuf += SIZE); 
    buf[curbuf++] = c;
    return;
}

void add_word() {
    if (curbuf > sizebuf - 1)
        buf = realloc(buf, sizebuf += 1); /* increasing buff (if necessary) to write \0 */
    buf[curbuf++] = '\0';
    buf = realloc(buf, sizebuf = curbuf);
    if (curlist > (sizelist - 1))
        lst = realloc(lst, (sizelist += SIZE) * sizeof(*lst)); 
    lst[curlist++] = buf;
    return;
}


//prints list
void print_list() {
    int i;
    if (lst == NULL)
        return;
    printf("%d\n",sizelist-1);
    for (i = 0; i < sizelist - 1; i++)
        printf("%s\n", lst[i]);
}

//sorts lst global var 
//params: pointer to list and number of elements in the list
void sort_list(){
    //subtracting 1 from sizelist since we have null at the end 
    sort(&lst,sizelist-1);
    return;
}

//prints list, sorts list, prints list one more time and cleans list
void print_sort_clear_list(){
    term_list();
    print_list();
    sort_list();
    print_list();
    clear_list();
    return;
}