// QUEUEND.H
// Definition of template class QueueNode
#ifndef QUEUEND_H
#define QUEUEND_H

template<class T> class Queue; // forward declaration

template <class T>
class QueueNode {
   friend class Queue<T>;
public:
   QueueNode( const T & = 0 );
   T getData() const;
private:
   T data;
   QueueNode *nextPtr;
};

// Member function definitions for class QueueNode
template <class T>
QueueNode<T>::QueueNode( const T &d )
{
   data = d;
   nextPtr = 0;
}

template <class T>
T QueueNode<T>::getData() const { return data; }

#endif

