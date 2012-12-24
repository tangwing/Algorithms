#include <iostream>
using namespace std;
#include "insertionSort.cpp"
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
    sort_insertion(tab,5,true);
    printTab(tab, 5);
    return 0;
}
