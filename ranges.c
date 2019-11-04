#include <stdlib.h>
#include <stdio.h>

#include "ranges.h"
#include "printTree.h"

extern node_t *root;

int compare_ranges(node_t *incoming, node_t *existing)
{
    // smaller range
    if (incoming->range_end < existing->range_start)
    {
        return -1;
    }
    // bigger range
    if (incoming->range_start > existing->range_end)
    {
        return 1;
    }
    //overlapping!
    return 0;
}

void insert_range(int start, int end)
{
    int cmp;

    // range to compare against during search
    node_t *n = (node_t *)calloc(1, sizeof(node_t));
    n->range_start = start;
    n->range_end = end;

    if (root == NULL)
    {
        n->color = RB_COLOR_BLACK;
        (root) = n;
    }
    else
    {
        node_t *old_curr = NULL;
        node_t *curr = (root);

        //binary search for range to insert
        while (curr != NULL)
        {
            old_curr = curr;
            cmp = compare_ranges(n, curr);
            if (cmp == 0)
            {
                // merge into existing range
                curr->range_start = MIN(curr->range_start, n->range_start);
                curr->range_end = MAX(curr->range_end, n->range_end);
                return;
            }
            else if (cmp < 0)
            {
                curr = curr->left;
            }
            else
            {
                curr = curr->right;
            }
        }

        //insert location found, adding leaf
        n->parent = old_curr;
        cmp = compare_ranges(n, old_curr);
        if (cmp > 0)
        {
            old_curr->right = n;
        }

        else
        {
            old_curr->left = n;
        }

        n->color = RB_COLOR_RED;

        insert_fix(n);
    }
}

void remove_range(int start, int end)
{
    int cmp;

    if (root == NULL)
    {
        return;
    }

    //initiate node for comparisons
    node_t *n = (node_t *)calloc(1, sizeof(node_t));
    n->range_start = start;
    n->range_end = end;

    node_t *curr = (root);

    //binary search for overlapping ranges
    while (curr != NULL)
    {
        cmp = compare_ranges(n, curr);

        if (cmp == 0)
        {
            //overlapping range found
            break;
        }
        else if (cmp < 0)
        {
            //current range is smaller
            curr = curr->left;
        }
        else
        {
            //current range is larger
            curr = curr->right;
        }
    }

    //node with a matching range not found
    if (!curr)
    {
        //Nothing more needs to be done!
        return;
    }
    else
    {
        //if range completly absorbed
        if (start <= curr->range_start && end >= curr->range_end)
        {
            delete_node(curr);
        }
        else
        {
            //otherwise just truncate the range
            if (start > curr->range_start)
            {
                curr->range_end = MIN(start, curr->range_end);
            }
            else
            {
                curr->range_start = MAX(end, curr->range_start);
            }
        }
    }
}
void get_range_r(node_t * curr, int start, int end)
{
    if(!curr)
    {
        return;
    }

    node_t *n = (node_t *)calloc(1, sizeof(node_t));
    n->range_start = start;
    n->range_end = end;

    int cmp = compare_ranges(n, curr);

    //search and given range overlap
    if (cmp == 0)
    {
        //print found range
        if(n->range_start < n->range_end && curr->range_start != end)
        {
            printf("(%d,%d)\n", curr->range_start, curr->range_end);
        }

        //the requested "get" range overlaps this range on the left
        if(start<=curr->range_start)
        {
            get_range_r(curr->left,start,end);
        }
        
        //the requested "get" range overlaps this range on the right
        if(end>=curr->range_end)
        {
            get_range_r(curr->right,start,end);
        }
        return;
    }
    else if (cmp < 0)
    {
        //current range is smaller
        curr = curr->left;
    }
    else
    {
        //current range is larger
        curr = curr->right;
    }
    get_range_r(curr,start,end);
}

void get_range(int start, int end)
{
    int cmp;

    if (root == NULL)
    {
        return;
    }

    //initiate node for comparisons
    node_t *n = (node_t *)calloc(1, sizeof(node_t));
    n->range_start = start;
    n->range_end = end;

    node_t *curr = (root);

    //binary search for highest range in the tree that overlaps the searched range
    while (curr != NULL)
    {
        cmp = compare_ranges(n, curr);

        if (cmp == 0)
        {
            get_range_r(curr,start,end);
            return;
        }
        else if (cmp < 0)
        {
            //current range is smaller
            curr = curr->left;
        }
        else
        {
            //current range is larger
            curr = curr->right;
        }
    }
}

//testing
void main()
{
    printf("\nTrees Grow in This Direction ------>\n\n");
    printf("--------------------------------\n");
    printf("PROVIDED INSERT 1\n");

    insert_range(1, 2);
    insert_range(3, 5);
    printTree(root);

    printf("PROVIDED INSERT 2\n");
    root = NULL;
    insert_range(1, 6);
    insert_range(3, 5);
    printTree(root);

    printf("PROVIDED INSERT 3\n");
    root = NULL;
    insert_range(1, 4);
    insert_range(3, 5);
    printTree(root);

    printf("PROVIDED REMOVE 1\n");
    root = NULL;
    insert_range(1, 6);
    remove_range(-3, -1);
    printTree(root);

    printf("PROVIDED REMOVE 2\n");
    root = NULL;
    insert_range(1, 6);
    remove_range(-1, 10);
    printTree(root);

    printf("PROVIDED REMOVE 3\n");
    root = NULL;
    insert_range(1, 6);
    remove_range(4, 10);
    printTree(root);

    printf("PROVIDED GET 1\n");
    root = NULL;
    insert_range(1, 3);
    insert_range(5, 7);
    get_range(4, 5);

    printf("PROVIDED GET 2\n");
    root = NULL;
    insert_range(1, 3);
    insert_range(5, 6);
    get_range(4, 6);

    printf("PROVIDED GET 3\n");
    root = NULL;
    insert_range(1, 3);
    insert_range(5, 6);
    get_range(2, 9);

    printf("BALANCING TEST\n");
    root = NULL;
    insert_range(1, 2);
    insert_range(3, 4);
    insert_range(5, 6);
    insert_range(7, 8);
    insert_range(9, 10);
    insert_range(11, 12);
    insert_range(13, 14);
    printTree(root);

    printf("DELETE TEST\n");
    root = NULL;
    insert_range(1, 6);
    insert_range(8, 12);
    insert_range(-3, -1);
    insert_range(16, 46);
    remove_range(-2, -1);
    remove_range(1, 4);
    remove_range(7, 13);
    printTree(root);
}