#include <iostream>
#include "Customer.h"
class Queue
{
protected:
    Customer ** custArr;
    int capacity;
    int front;
    int end;
    int rear;
    int count;
    int maxCount;
public:
    Queue(){};
    Queue(int size);
    void enqueue(Customer*);
    Customer* dequeue();
    int getMaxSize();
    Customer* peek();
    bool isEmpty();
    ~Queue();
};

Queue::Queue(int size)
{
    custArr = new Customer*[size];
    capacity = size;
    front =0;
    rear =-1;
    count = 0;
    maxCount=0;
}

Queue::~Queue()
{
    delete[] custArr;
}
void Queue::enqueue(Customer* cust)
{
    rear = (rear +1)%capacity;
    custArr[rear] = cust;
    count++;
    if(count>maxCount)
    {
        maxCount =count;
    }
}

Customer* Queue::dequeue()
{
    Customer* cust = custArr[front];
    front= (front+1)%capacity;
    count--;
    return cust;
}
int Queue::getMaxSize()
{
    return maxCount;
}
Customer* Queue::peek()
{
    return custArr[front];
}
bool Queue::isEmpty()
{
    return (count==0);
}


class PriorityQueue : public Queue
{
public:
    PriorityQueue(int size):Queue(size){};
    ~PriorityQueue(){};
    Customer* peek();
    Customer* dequeue();
};

Customer* PriorityQueue::peek()
{
    int maxPricedInd = 0;
    for(int i=1;i <count ; i++)
    {
        if(custArr[maxPricedInd]->coffeePrice<custArr[i]->coffeePrice){
            maxPricedInd = i;
        }
    }
    return custArr[maxPricedInd];
}
Customer* PriorityQueue::dequeue()
{   
    int maxPricedInd = 0;
    for(int i=1;i <count ; i++)
    {
        if(custArr[maxPricedInd]->coffeePrice<custArr[i]->coffeePrice){
            maxPricedInd = i;
        }
    }
    Customer* cust = custArr[maxPricedInd];
        for (int i = maxPricedInd; i < count; i++) 
    {
        custArr[i] = custArr[i + 1];
    }

    rear= (rear-1)%capacity;
    count--;
    
    return cust;
}