#ifndef QUEUEARRAY_H
#define QUEUEARRAY_H

#include <Arduino.h>

template<typename T>
class QueueArray {
  public:
    QueueArray ();
    ~QueueArray ();
    void enqueue (const T i); 
    T* dequeue ();
    void push (const T i);
    T* pop ();
    bool isEmpty () const;
    bool isFull () const;
    void clear();
  private:
    void resize (const int s);
    static const int initialSize = 2;
    T * contents;    
    int size;        
    int items;      
    int head;       
    int tail;       
};
template<typename T>
QueueArray<T>::QueueArray () {
  size = 0;       
  items = 0;      
  head = 0;       
  tail = 0;     
  contents = (T *) malloc (sizeof (T) * initialSize);
  size = initialSize;
}

template<typename T>
QueueArray<T>::~QueueArray () {
  free (contents); 
  contents = NULL; 
  size = 0;       
  items = 0;       
  head = 0;        
  tail = 0;       
}

template<typename T>
void QueueArray<T>::resize (const int s) {
  if (s > 0){
    T * temp = (T *) malloc (sizeof (T) * s);
    if (temp != NULL){      
      for (int i = 0; i < items; i++)     temp[i] = contents[(head + i) % size];    
      free (contents);
      contents = temp;
      head = 0; tail = items;
      size = s;
    }
  }
}

template<typename T>
void QueueArray<T>::clear(){
    items = 0;
}

template<typename T>
void QueueArray<T>::enqueue (const T i) {
  if (isFull ())
    resize (size * 2);
  contents[tail++] = i;
  if (tail == size) tail = 0;
  items++;
}

template<typename T>
void QueueArray<T>::push (const T i) {
  enqueue(i);
}

template<typename T>
T* QueueArray<T>::dequeue () {
  if (isEmpty()) return NULL; 
    T *item = &contents[head++];
    items--;
    if (head == size) head = 0;
    if (!isEmpty () && (items <= size / 4))
      resize (size / 2);
  return item;
}

template<typename T>
T* QueueArray<T>::pop () {
  return dequeue();
}

template<typename T>
bool QueueArray<T>::isEmpty () const {
  return items == 0;
}

template<typename T>
bool QueueArray<T>::isFull () const {
  return items == size;
}

#endif
