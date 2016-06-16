#ifndef PQUEUE_H
#define PQUEUE_H

#include<cstring>
#include <assert.h>
#include <stdio.h>
#define LEFT(i) (i*2+1)
#define RIGHT(i) ((i+1)*2)
#define UP(i) ((i-1)/2)

/***********Priority Queue, using max_heap*******************/
/**Represent a job in the priority queue. This should be
*the base-class of all objects who want to use priority queue.
*/
class Job
{
public:
    int heap_index;       //The index in the queue, should only be used by the queue.
    int priority;         //The priority of this job.
    char desp[100];
    Job(int prio)
    {
        priority = (prio);
        memset(desp, 0, 100);
    }
    Job(int prio, const char* des)
    {
        priority = (prio);
        strncpy_s(desp, des, 100);
    }
};

class QueueMaxHeap
{
private:
    Job** heap;      //The table which stores the heap
    unsigned int max_size;   //The max size of this heap
    unsigned int size;       //Current heap size.
public:
    QueueMaxHeap(unsigned int max)
    {
        max_size = max;
        size = 0;
        heap = new Job*[max_size];
    }
    ~QueueMaxHeap()
    {
        if (heap != NULL)delete[] heap;
    }
    Job* getMaxPriJob()
    {
        if (size == 0)
            return NULL;
        return heap[0];
    }
    Job* extractMaxPriJob()
    {
        if (size <= 0)return NULL;
        Job * res = heap[0];
        heap[0] = heap[size - 1];
        size--;
        max_heapify(heap, size, 0);
        return res;
    }
    void insertJob(Job* job)
    {
        if (size < max_size)
        {
            heap[size] = job;
            job->heap_index = size;
            size++;
            increaseJobPri(job, job->priority);
        }
    }
    void increaseJobPri(Job* job, int largerPri)
    {
        if (largerPri >= job->priority)
        {
            job->priority = largerPri;
            int i = job->heap_index;
            while (i>0 && heap[i]->priority>heap[UP(i)]->priority)
            {
                swap(heap, i, UP(i));
                i = UP(i);
            }

        }
    }
    /**A sub fonction which can make the tree whose root index is
    * 'index' a max_heap. We assume that the two subtrees of root
    * are already heapified. O(lg(n))
    *@param tab The table containing the job heap
    *@param size_heap the size of the table
    *@param index the index of the root node of the tree to be heapified
    */
    void max_heapify(Job **tab, int size_heap, int index)
    {
        int l, r, max;
        l = LEFT(index);
        r = RIGHT(index);
        if (l<size_heap && tab[l]->priority>tab[index]->priority) max = l;
        else max = index;
        if (r<size_heap && tab[r]->priority>tab[max]->priority) max = r;
        if (max != index)
        {//Send down the root node
            swap(tab, max, index);
            max_heapify(tab, size_heap, max);
        }
    }

    void swap(Job **tab, int i, int j)
    {
        Job* TMP = tab[i]; tab[i] = tab[j]; tab[j] = TMP;
        tab[i]->heap_index = j;
        tab[j]->heap_index = i;
    }
};

#endif

