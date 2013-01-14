/******************HEAP SORT********************/
#include <iostream>
using namespace std;
//find the index of the left 'son'
//from the current heap node index
#define LEFT(i) (i*2+1)
#define RIGHT(i) ((i+1)*2)
#define UP(i) ((i-1)/2)
#define EXCHANGE(tab,i,j) {int TMP=tab[i]; tab[i]=tab[j]; tab[j]=TMP;}

/**A sub fonction which can make the tree whose root index is
* 'index' a max_heap. We assume that the two subtrees of root
* are already heapified. O(lg(n))
*@param tab The table containing the heap
*@param size_heap the size of the table
*@param index the index of the root node of the tree to be heapified
*/
void max_heapify(int *tab, int size_heap, int index)
{
    int l,r, max;
    l = LEFT(index);
    r = RIGHT(index);
    if(l<size_heap && tab[l]>tab[index]) max = l;
    else max = index;
    if(r<size_heap && tab[r]>tab[max]) max = r;
    if(max != index)
    {//Send down the root node
        EXCHANGE(tab, max, index);
        max_heapify(tab, size_heap, max);
    }
}

/**Heap sort. Using max_heap, sort the table to increasing order.
*@param tab The table to be sorted
*@param size the size of the table
*/
void sort_heap_max(int *tab, int size)
{
    //Build max_heap, O(n)
    int i = size/2 - 1;
    while(i>=0)
    {
        max_heapify(tab, size, i--);
    }

    //Sort : nlg(n)
    for(i=size-1; i>0; i--)
    {
        EXCHANGE(tab,0,i);
        max_heapify(tab, i, 0);
    }
}

template <class T>
void printTab(T*tab, int size)
{
    int i=0;
    while(i<size)
        cout<<tab[i++]<<" ";
    cout<<endl;
}
void testSort()
{
    //double tab[5] = {1.2,3.4,2.1,1.25,5};
    int itab[5] = {5,3,4,1,2};
    //sort_insertion(tab,5,true);
    //sort_selection(tab,5,true);
    //sort_selection(itab,5,true);
    //sort_merge(itab,5);
    sort_heap_max(itab,5);

    printTab(itab, 5);
    //int i = search_binary_iterative(itab,5,4);
//    cout<<"The index of 4 is : "<<i<<endl;
//    i = search_binary_recursive(itab,5,3);
//    cout<<"The index of 3 is : "<<i<<endl;
}
