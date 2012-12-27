#include "../src/sort/sort_insertion.cpp"
#include "../src/sort/sort_selection.cpp"
#include "../src/sort/sort_merge.cpp"

/**Binary search, iterative version
*@param tab The table to search from, sorted in increasing order.
*@param n The number of elements in tab
*@param target The element to search
*@return The index of target in tab if it exists, -1 if not.
*@note This is the iterative version
*/
int search_binary_iterative(int*tab, int n, int target);

/**Binary search, recursive version
*@param tab The table to search from, sorted in increasing order.
*@param n The number of elements in tab
*@param target The element to search
*@return The index of target in tab if it exists, -1 if not.
*@note This is the recursive version
*/
int search_binary_recursive(int*tab, int n, int target);
