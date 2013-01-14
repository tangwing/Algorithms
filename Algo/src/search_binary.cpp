
/**Binary search, iterative version
*@param tab The table to search from, sorted in increasing order.
*@param n The number of elements in tab
*@param target The element to search
*@return The index of target in tab if it exists, -1 if not.
*@note This is the iterative version
*/
int search_binary_iterative(int*tab, int n, int target)
{
    int begin = 0, end = n-1;
    int mid;
    while(begin<=end)
    {
        mid = (begin+end)/2;
        if(tab[mid]== target)
            return mid;
        else if(tab[mid] > target)
            end = mid - 1;
        else
            begin = mid + 1;
    }
    return -1;
}

/**Binary search, recursive version
*@param tab The table to search from, sorted in increasing order.
*@param n The number of elements in tab
*@param target The element to search
*@return The index of target in tab if it exists, -1 if not.
*@note This is the recursive version
*/
int search_binary_recursive(int*tab, int n, int target)
{
    if(n<1)return -1;
    int mid = (n-1)/2;
    if(tab[mid]==target)
        return mid;
    else if (tab[mid]>target)
    {
        int res = search_binary_recursive(tab, mid+1, target);
        return res==-1?-1:res;
    }
    else
    {
        int res = search_binary_recursive(tab+mid, n-mid, target);
        return res==-1?-1:res+mid;
    }

}
