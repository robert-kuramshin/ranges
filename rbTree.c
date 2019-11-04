#include <stdlib.h>
#include <stdio.h>

#include "ranges.h"

//macros for getting node siblings
#define PARENT(x) ((x)->parent)
#define L_SIBLING(x) (!PARENT(x) ? NULL : PARENT(x)->left)
#define R_SIBLING(x) (!PARENT(x) ? NULL : PARENT(x)->right)
#define SIBLING(x) (x->parent && x->parent == L_SIBLING(x) ? R_SIBLING(x) : L_SIBLING(x))

//macros for getting granpdarents/uncles
#define GRANDPARENT(x) (!PARENT(x) ? NULL : (x)->parent->parent)
#define L_UNCLE(x) (!GRANDPARENT(x) ? NULL : GRANDPARENT(x)->left)
#define R_UNCLE(x) (!GRANDPARENT(x) ? NULL : GRANDPARENT(x)->right)
#define UNCLE(x) (x->parent && x->parent == L_UNCLE(x) ? R_UNCLE(x) : L_UNCLE(x))

//swapping colors
#define SWAP_COLORS(a, b)        \
    {                            \
        char c = (a)->color;     \
        (a)->color = (b)->color; \
        (b)->color = c;          \
    }

node_t *root = NULL;

//red-black tree rotation
static void left_rotation(node_t *n)
{
    if (!n || !n->right)
        return;

    node_t *r = n->right;

    n->right = r->left;

    if (n->right)
        n->right->parent = n;

    r->parent = n->parent;

    if (!n->parent)
    {
        root = r;
    }
    else if (n == n->parent->left)
    {
        n->parent->left = r;
    }
    else
    {
        n->parent->right = r;
    }

    r->left = n;
    n->parent = r;
}

//red-black tree rotation
static void right_rotation(node_t *n)
{
    if (!n || !n->left)
        return;

    node_t *l = n->left;

    n->left = l->right;

    if (l->right)
    {
        l->right->parent = n;
    }
    l->parent = n->parent;
    if (!l->parent)
    {
        root = l;
    }
    else if (n == n->parent->left)
    {
        n->parent->left = l;
    }
    else
    {
        n->parent->right = l;
    }
    l->right = n;
    n->parent = l;
}

void insert_fix(node_t *n)
{
    if (!n->parent) //  CASE 1
    {
        if (n->parent == NULL)
        {
            n->color = RB_COLOR_BLACK;
        }
    }
    else if (n->parent->color == RB_COLOR_BLACK) //  CASE 2
    {
        //Tree is correct so return
        return;
    }
    else if (UNCLE(n) != NULL && UNCLE(n)->color == RB_COLOR_RED) //  CASE 3
    {
        n->parent->color = RB_COLOR_BLACK;
        UNCLE(n)->color = RB_COLOR_BLACK;
        GRANDPARENT(n)->color = RB_COLOR_RED;
        insert_fix(GRANDPARENT(n));
    }
    else //  CASE 4
    {
        //CASE 4A
        node_t *p = n->parent;
        node_t *g = GRANDPARENT(n);

        if (n == p->right && p == g->left)
        {
            left_rotation(p);
            n = n->left;
        }
        else if (n == p->left && p == g->right)
        {
            right_rotation(p);
            n = n->right;
        }
        //CASE 4B
        p = n->parent;
        g = GRANDPARENT(n);

        if (n == p->left)
        {
            right_rotation(g);
        }
        else
        {
            left_rotation(g);
        }
        p->color = RB_COLOR_BLACK;
        g->color = RB_COLOR_RED;
    }
}

static void delete_node_case1(node_t *n);
static void delete_node_case2(node_t *n);
static void delete_node_case3(node_t *n);
static void delete_node_case4(node_t *n);
static void delete_node_case5(node_t *n);
static void delete_node_case6(node_t *n);

static void delete_node_case1(node_t *n)
{
    if (n->parent != NULL)
    {
        delete_node_case2(n);
    }
}
static void delete_node_case4(node_t *n)
{
    node_t *s = SIBLING(n);

    if ((n->parent->color == RB_COLOR_RED) && (s->color == RB_COLOR_BLACK) &&
        (s->left->color == RB_COLOR_BLACK) && (s->right->color == RB_COLOR_BLACK))
    {
        s->color = RB_COLOR_RED;
        n->parent->color = RB_COLOR_BLACK;
    }
    else
    {
        delete_node_case5(n);
    }
}
static void delete_node_case5(node_t *n)
{
    node_t *s = SIBLING(n);

    if (s->color == RB_COLOR_BLACK)
    {
        if ((n == n->parent->left) && (s->right->color == RB_COLOR_BLACK) &&
            (s->left->color == RB_COLOR_RED))
        {
            s->color = RB_COLOR_RED;
            s->left->color = RB_COLOR_BLACK;
            right_rotation(s);
        }
        else if ((n == n->parent->right) && (s->left->color == RB_COLOR_BLACK) &&
                 (s->right->color == RB_COLOR_RED))
        {
            s->color = RB_COLOR_RED;
            s->right->color = RB_COLOR_BLACK;
            left_rotation(s);
        }
    }
    delete_node_case6(n);
}
static void delete_node_case6(node_t *n)
{
    node_t *s = SIBLING(n);

    s->color = n->parent->color;
    n->parent->color = RB_COLOR_BLACK;

    if (n == n->parent->left)
    {
        s->right->color = RB_COLOR_BLACK;
        left_rotation(n->parent);
    }
    else
    {
        s->left->color = RB_COLOR_BLACK;
        right_rotation(n->parent);
    }
}
static void delete_node_case3(node_t *n)
{
    node_t *s = SIBLING(n);

    if ((n->parent->color == RB_COLOR_BLACK) && (s->color == RB_COLOR_BLACK) &&
        (s->left->color == RB_COLOR_BLACK) && (s->right->color == RB_COLOR_BLACK))
    {
        s->color = RB_COLOR_RED;
        delete_node_case1(n->parent);
    }
    else
    {
        delete_node_case4(n);
    }
}
static void delete_node_case2(node_t *n)
{
    node_t *s = SIBLING(n);

    if (s->color == RB_COLOR_RED)
    {
        n->parent->color = RB_COLOR_RED;
        s->color = RB_COLOR_BLACK;
        if (n == n->parent->left)
        {
            left_rotation(n->parent);
        }
        else
        {
            right_rotation(n->parent);
        }
    }
    delete_node_case3(n);
}

void delete_node(node_t *n)
{
    if (n == root)
    {
        free(n);
        root = NULL;
        return;
    }

    node_t *child = (n->right == NULL) ? n->left : n->right;

    if (child)
    {
        child->parent = n->parent;
        if (n == n->parent->left)
        {
            n->parent->left = child;
        }
        else
        {
            n->parent->right = child;
        }
        if (n->color == RB_COLOR_BLACK)
        {
            if (child->color == RB_COLOR_RED)
            {
                child->color = RB_COLOR_BLACK;
            }
            else
            {
                delete_node_case1(child);
            }
        }
    }
    else
    {
        if (n == n->parent->left)
        {
            //node is left child of parent
            n->parent->left = NULL;
        }
        else
        {
            //node is right child of parent
            n->parent->right = NULL;
        }
    }
    free(n);
}