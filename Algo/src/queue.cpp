#include "../inc/queue.h"
#include <assert.h>
#include <stdio.h>
//find the index of the left 'son'
//from the current heap node index
#define LEFT(i) (i*2+1)
#define RIGHT(i) ((i+1)*2)
#define UP(i) ((i-1)/2)
#define EXCHANGE(tab,i,j) \
{Job* TMP=tab[i]; tab[i]=tab[j]; tab[j]=TMP;\
tab[i]->heap_index = j; \
tab[j]->heap_index = i;}

/**A sub fonction which can make the tree whose root index is
* 'index' a max_heap. We assume that the two subtrees of root
* are already heapified. O(lg(n))
*@param tab The table containing the job heap
*@param size_heap the size of the table
*@param index the index of the root node of the tree to be heapified
*/
void max_heapify(Job **tab, int size_heap, int index)
{
    int l,r, max;
    l = LEFT(index);
    r = RIGHT(index);
    if(l<size_heap && tab[l]->priority>tab[index]->priority) max = l;
    else max = index;
    if(r<size_heap && tab[r]->priority>tab[max]->priority) max = r;
    if(max != index)
    {//Send down the root node
        EXCHANGE(tab, max, index);
        max_heapify(tab, size_heap, max);
    }
}


Job* QueueMaxHeap::extractMaxPriJob()
{
    if(size<=0)return NULL;
    Job * res = heap[0];
    heap[0] = heap[size-1];
    size --;
    max_heapify(heap, size, 0);
    return res;
}

void QueueMaxHeap::insertJob(Job* job)
{
    if(size < max_size)
    {
        heap[size] = job;
        job->heap_index = size;
        size++;
        increaseJobPri(job, job->priority);
    }
}

void QueueMaxHeap::increaseJobPri(Job* job, int largerPri)
{
    if(largerPri >= job->priority)
    {
        job->priority = largerPri;
        int i = job->heap_index;
        while(i>0 && heap[i]->priority>heap[UP(i)]->priority)
        {
            EXCHANGE(heap, i, UP(i));
            i = UP(i);
        }

    }
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
