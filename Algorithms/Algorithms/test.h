#ifndef TEST_H
#define TEST_H
#include "sort.h"
#include "pqueue.h"

template <class T>
void printTab(T*tab, int size)
{
    int i = 0;
    while (i<size)
        cout << tab[i++] << " ";
    cout << endl;
}

void testSort()
{
    //double tab[5] = {1.2,3.4,2.1,1.25,5};
    int itab[5] = { 5, 3, 4, 1, 2 };
    //sort_insertion(tab,5,true);
    //sort_selection(tab,5,true);
    //sort_selection(itab,5,true);
    //sort_merge(itab,5);
    sort_heap_max(itab, 5);

    printTab(itab, 5);
    //int i = search_binary_iterative(itab,5,4);
    //    cout<<"The index of 4 is : "<<i<<endl;
    //    i = search_binary_recursive(itab,5,3);
    //    cout<<"The index of 3 is : "<<i<<endl;
}

void testQueue()
{
    Job job1(3, "Job 1");
    Job job2(5, "Job 2");
    Job job3(2, "Job 3");
    Job job4(6, "Job 4");
    Job* tmp;
    QueueMaxHeap q(5);
    tmp = q.getMaxPriJob();
    //assert(NULL == q.getMaxPriJob());

    q.insertJob(&job1);
    tmp = q.getMaxPriJob();
    assert(&job1 == tmp);
    printf("%s\n", tmp->desp);

    q.insertJob(&job2);
    printf("%s\n", q.getMaxPriJob()->desp);

    q.insertJob(&job3);
    q.insertJob(&job4);
    printf("%s\n", q.getMaxPriJob()->desp);

    q.increaseJobPri(&job3, 10);
    printf("%s\n", q.extractMaxPriJob()->desp);
    printf("%s\n", q.extractMaxPriJob()->desp);
    printf("%s\n", q.extractMaxPriJob()->desp);
    printf("%s\n", q.extractMaxPriJob()->desp);
}
#endif