#ifndef RANGES_H_
#define RANGES_H_

//standard max and min macros
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

//red and black colors for tree
#define RB_COLOR_RED 0
#define RB_COLOR_BLACK 1

typedef struct node
{
    //range data info
    int range_start;
    int range_end;

    //red-black tree properties
    char color;
    struct node *left;
    struct node *right;
    struct node *parent;
} node_t;

//functions from rbTree made accessible
void insert_fix(node_t *n);
void delete_node(node_t *n);

#endif