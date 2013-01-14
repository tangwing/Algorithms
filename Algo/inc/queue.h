#ifndef QUEUE_H
#define QUEUE_H

#include<cstring>
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
      priority=(prio);
      memset(desp, 0, 100);
  }
  Job(int prio, const char* des)
  {
      priority=(prio);
      strncpy(desp, des, 100);
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
        if(heap!=NULL)delete[] heap;
    }
    Job* getMaxPriJob()
    {
        if(size==0)
            return NULL;
        return heap[0];
    }
    Job* extractMaxPriJob();
    void insertJob(Job*);
    void increaseJobPri(Job* job, int largerPri);
};
void testQueue();

#endif
