Ranges Problem Implementation
*by Robert Kuramshin*

About:
This program strives to solve the problem of creating a datastructure for storing integer ranges. It has capabilities for adding, deleting and getting ranges. The language used was C. 

Implementation Description:
The primary backing data structure selected was a red-black tree. A red-black tree offers O(logn) access time due to it's self balancing nature (as provided by rotations and use of an extra color byte). Sources and references sued during the implementation can be found at the bottom of this document. 
    Add:
    A binary search is done to either:
        1. Find an existing range that overlaps or partially overlaps the incoming range. In this case they are merge based on these conditions:
            existing->range_start = MIN(existing->range_start, incoming->range_start);
            existing->range_end = MAX(existing->range_end, incoming->range_end);
        2. If no overlapping range is found, then a new leaf is inserted into the tree and the red-black tree properties are evaluated and tree is fixed if needed

    Delete:
        1. Find an existing range that overlaps or partially overlaps the incoming range. In this case the existing range is truncated as so:
            if (start > existing->range_start)
            {
                existing->range_end = MIN(start, existing->range_end);
            }
            else
            {
                existing->range_start = MAX(end, existing->range_start);
            }
        2. If no overlapping range is found then nothing is done! 

    Get:

    1. A binary search is done to find a range that overlaps the provided range. If one is found, the range is returned and is used as basis of recursion.
    2. The descendants of that node are searched to see if they also overlap, based on a left side or a right side overlap. This is determined as so:
        //the requested "get" range overlaps this range on the left
        if(start<=existing->range_start)
        {
            get_range_r(existing->left,start,end);
        }
        
        //the requested "get" range overlaps this range on the right
        if(end>=existing->range_end)
        {
            get_range_r(existing->right,start,end);
        }

Assumptions:
    The provided descriptions left out a few cases for possible range insertions or deletions. most of them concern operations that affect more than one range at once. Since these cases were not required in the problem description, I deliberately chose to leave them out. Here are more details:

    - Add
        - The problem descriptions covers three cases: range exists, is contained within another range, or is partially covered by another range. These specific cases were implemented. Cases where the provided range may overlap several existing ranges in the data structure was not implemented. Ex: Original state [(1,2),(4,5)], ADD(1,5), New State [(1,5)]
    - Delete
        - Similarly, cases where deletion range overlaps several existing ranges were not implemented. Ex: Original state [(1,2),(4,5)], Delete(1,5), New State []
    - Get
        - Since examples of a provided range overlapping several existing ones was provided, all the described cases where covered. 

Runtimes
     -  Add     O(logn)     Binary search of the red-black tree is performed, hence logarithmic time  
     -  Delete  O(logn)     Binary search of the red-black tree is performed, hence logarithmic time 
     -  Get     O(mlogn)    Binary search is initially performed, children of a maximum of m desccendant nodes are recursively searched
    
    *m describes the the length of the range searched Ex: Get(a,b) m=b-a

Compiling and Running
    Compilation Environment:  
        gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
    
    Command used  
        gcc ranges.c printTree.c rbTree.c -o ranges` 

Sample Output Results:
    Trees Grow in This Direction ------>

    --------------------------------
    PROVIDED INSERT 1
    --------------------------------

                        (3,5)

            (1,2)
    --------------------------------
    PROVIDED INSERT 2
    --------------------------------

            (1,6)
    --------------------------------
    PROVIDED INSERT 3
    --------------------------------

            (1,5)
    --------------------------------
    PROVIDED REMOVE 1
    --------------------------------

            (1,6)
    --------------------------------
    PROVIDED REMOVE 2
    --------------------------------
    --------------------------------
    PROVIDED REMOVE 3
    --------------------------------

            (1,4)
    --------------------------------
    PROVIDED GET 1  
    --------------------------------
    PROVIDED GET 2  
    --------------------------------
        (5,6)  
    --------------------------------
    PROVIDED GET 3  
    --------------------------------
        (1,3)  
        (5,6)  
    --------------------------------
    BALANCING TEST
    --------------------------------

                                            (13,14)

                                (11,12)

                                            (9,10)

                        (7,8)

                                (5,6)

            (3,4)

                        (1,2)
    --------------------------------
    DELETE TEST
    --------------------------------

                        (16,46)

            (4,6)

                        (-3,-2)
    --------------------------------

Sources/References Used
    http://staff.ustc.edu.cn/~csli/graduate/algorithms/book6/chap14.htm
    https://www.cs.auckland.ac.nz/software/AlgAnim/red_black.html  
    https://en.wikipedia.org/wiki/Red%E2%80%93black_tree  