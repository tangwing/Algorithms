#ifndef SORT_H
#define SORT_H



/**Heap sort. Using max_heap, sort the table to increasing order.
*@param tab The table to be sorted
*@param size the size of the table
*/
void sort_heap_max(int *tab, int size);

/**The INSERTION-SORT algorithm.
*@param tab A pointer to the table to sort
*@param length The length of the table
*@param isIncreasing A flag value which indicate the order,
            no-0 value means to sort in the increasing order.
*@note The data will be sorted in the same table
*/
template <typename T>
void sort_insertion(T *tab, int length, bool isIncreasing)
{
    if(length>1)
    {
        T key;//The element to be inserted.
        int iKey;//The index of the element to be inserted
        int iPos;//The index of the position to insert an element
        //The first element is obviously sorted already
        for(iKey = 1; iKey < length; iKey++)
        {

            key = tab[iKey];
            iPos = iKey-1;
            while(iPos >= 0 )
            {
                if (isIncreasing && tab[iPos] <= key)
                	break;
                else if (!isIncreasing && tab[iPos]>key)
                	break;
                else
                    tab[iPos+1] = tab[iPos];
                iPos--;
            }
            tab[iPos+1] = key;
        }
    }
}


#include<string.h>
/**A sub-function of merge-sort algo, merge two sorted sub-lists (connected)
* to one.
*@param tab The table to sort, entry and result
*@param indexBegin The index of the first sorted sub-list
*@param indexMid The index of the last element of the first sorted sub-list
*@param indexEnd The index of the last element of the second sorted sub-list
*@return tab The table (index from indexBegin to indexEnd),sorted in increasing order.
*/
template <class T>
void merge(T *tab, int indexBegin, int indexMid, int indexEnd)
{
    int length = indexEnd-indexBegin+1;
    T* tabMerged = new T[length];// (T*)malloc(sizeof(T)*length);
    int i, i1, i2;
    for(i=0, i1=indexBegin, i2=indexMid+1; i<length && i1 <= indexMid && i2 <= indexEnd; i++ )
    {
        if(tab[i1]<=tab[i2])
        {
            tabMerged[i] = tab[i1];
            i1++;
        }else
        {
            tabMerged[i] = tab[i2];
            i2++;
        }
    }
    if(i1 > indexMid)
    {
        //copy the rest of List2 to tabMerged
        while(i2<=indexEnd)
            tabMerged[i++]=tab[i2++];
    }
    else if(i2 > indexEnd)
    {
        //copy the rest of List1 to tabMerged
        while(i1<=indexMid)
            tabMerged[i++]=tab[i1++];
    }
    memcpy(tab+indexBegin, tabMerged, sizeof(T)*length);
    delete [] tabMerged;
}

/**The SELECTION-SORT algorithm.
*@param tab A pointer to the table to sort
*@param length The length of the table
*@param isIncreasing A flag value which indicate the order,
            no-0 value means to sort in the increasing order.
*@note The data will be sorted in the same table
*/
template <typename T>
void sort_selection(T *tab, int length, bool isIncreasing)
{
    if(length>1)
    {
        T key;//The element selected during each loop.
        int iKey;//The position of the current selected element
        int iLoop;//The index of the element to update
        int iLoop2;

        //The last element is obviously sorted already
        for(iLoop = 0; iLoop < length-1; iLoop++)
        {

            key = tab[iLoop];
            iKey = iLoop;
            for(iLoop2 = iLoop+1; iLoop2<length; iLoop2++)
            {
                if (isIncreasing && tab[iLoop2]<key ||
                    !isIncreasing && tab[iLoop2]>key)
                {
                	key = tab[iLoop2];
                    iKey = iLoop2;
                }
            }
            tab[iKey] = tab[iLoop];
            tab[iLoop] = key;
        }
    }
}



/**Merge-sort, increasing order
*@param tab The table to sort
*@param length The length of the table
*@return tab The sorted table in increasing order.
*/
template <class T>
void sort_merge(T *tab, int length)
{
    if(length<=1)return;
    int indexMid = (length-1)/2;
    sort_merge(tab, indexMid+1);
    sort_merge(tab+indexMid+1,length-indexMid-1);
    merge(tab,0,indexMid,length-1);
}

///I also try to write a version in Prolog
//%Merge sort (not finish)
//split(L2,0,_,L2):-!.
//split([E|L],N,[E|L1],L2):-
//      N2 is N-1,
//      split(L,N2,L1,L2).
//merge([E1|L1],[E2|L2], [E|L]):-
//    (E1 =< E2
//        ->
//     (E is E1),merge(L1,[E2|L2], L)
//     ;
//     (E is E2),merge([E1|L1],L2, L)).
//
//merge_sort([E], [E]):-!.
//merge_sort(L, LSorted):-
//    length(L,N),
//    N2 is N/2,
//    split(L, N2, L1, L2),
//    merge_sort(L1, L1Sorted),
//    merge_sort(L2, L2Sorted),
//    merge(L1Sorted,L2Sorted,LSorted).
#endif
