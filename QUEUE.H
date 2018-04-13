// QUEUE.H
// Definition of class Queue
#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <cassert>
#include "queuend.h"

using std::cout;

template <class T>
class Queue {
public:
   Queue();              // default constructor
   ~Queue();             // destructor
   void enqueue( T & );  // insert item in queue
   T dequeue();          // remove item from queue
   int isEmpty() const;  // is the queue empty?
   void print() const;   // output the queue
private:
   QueueNode<T> *headPtr;  // pointer to first QueueNode
   QueueNode<T> *tailPtr;  // pointer to last QueueNode
};

// Member function definitions for class Queue
template <class T>
Queue<T>::Queue() { headPtr = tailPtr = 0; }

template <class T>
Queue<T>::~Queue()
{
   QueueNode<T> *tempPtr, *currentPtr = headPtr;

   while ( currentPtr != 0 ) {
      tempPtr = currentPtr;
      currentPtr = currentPtr->nextPtr;
      delete tempPtr;
   }
}

template <class T>
void Queue<T>::enqueue( T &d )
{
   QueueNode<T> *newPtr = new QueueNode<T>( d );
   assert( newPtr != 0 );  // was memory allocated?

   if ( isEmpty() )
      headPtr = tailPtr = newPtr;
   else {
      tailPtr->nextPtr = newPtr;
      tailPtr = newPtr;
   }
}

template <class T>
T Queue<T>::dequeue()
{
   assert( !isEmpty() );

   QueueNode<T> *tempPtr = headPtr;

   headPtr = headPtr->nextPtr;
   T value = tempPtr->data;
   delete tempPtr;

   if ( headPtr == 0 )
      tailPtr = 0;

   return value;
}

template <class T>
int Queue<T>::isEmpty() const { return headPtr == 0; }

template <class T>
void Queue<T>::print() const
{
   QueueNode<T> *currentPtr = headPtr;

   if ( isEmpty() )          // Queue is empty
      cout << "Queue is empty" << endl;
   else {                    // Queue is not empty
      cout << "The queue is:" << endl;

      while ( currentPtr != 0 ) {
//         cout << currentPtr->data << ' ';
         cout << currentPtr<< '\n';
         currentPtr = currentPtr->nextPtr;
      }

      cout << endl;
   }
}

#endif

