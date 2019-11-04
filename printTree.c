#include <stdio.h>

#include "ranges.h"
#include "printTree.h"

static void printComponent(node_t *root, int layer_width)
{
    if (!root)
        return;

    layer_width += 10;

    printComponent(root->right, layer_width);

    printf("\n");
    for (int i = 0; i < layer_width; i++)
        printf(" ");

    printf("(%d,%d)\n", root->range_start, root->range_end);

    printComponent(root->left, layer_width);
}

void printTree(node_t *root)
{
    printf("--------------------------------\n");
    printComponent(root, 0);
    printf("--------------------------------\n");
}