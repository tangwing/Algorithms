#include <iostream>
using namespace std;
#include "../include/algo.h"

template <class T>
void printTab(T*tab, int size)
{
    int i=0;
    while(i<size)
        cout<<tab[i++]<<" ";
    cout<<endl;
}

int main()
{
    double tab[5] = {1.2,3.4,2.1,1.25,5};
    int itab[5] = {5,3,4,1,2};
    //sort_insertion(tab,5,true);
    //sort_selection(tab,5,true);
    //sort_selection(itab,5,true);
    sort_merge(itab,5);
    printTab(itab, 5);
    int i = search_binary_iterative(itab,5,4);
    cout<<"The index of 4 is : "<<i<<endl;
    i = search_binary_recursive(itab,5,3);
    cout<<"The index of 3 is : "<<i<<endl;

    return 0;
}
