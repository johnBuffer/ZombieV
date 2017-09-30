#ifndef CHAINEDOBJECT_HPP_INCLUDED
#define CHAINEDOBJECT_HPP_INCLUDED

#include <iostream>

template<class T>
class ChainedObject
{
public:
    static T* getHead();
    static T* getTail();

protected:
    static T* _head;

    static void add(T*);
    static void remove(T*);
};

template<class T> T* ChainedObject<T>::_head = nullptr;

template<class T>
void ChainedObject<T>::add(T* obj)
{
    if (_head)
    {
        _head->setNext(obj);
        obj->setPrev(_head);
        obj->setNext(nullptr);
        _head = obj;
    }
    else
    {
        _head = obj;
        _head->setPrev(nullptr);
        _head->setNext(nullptr);
    }
}

template<class T>
void ChainedObject<T>::remove(T* obj)
{
    T* next = static_cast<T*>(obj->getNext());
    T* prev = static_cast<T*>(obj->getPrev());

    if (next)
    {
        next->setPrev(prev);
    }
    else
    {
        _head = prev;
    }

    if (prev)
    {
        prev->setNext(next);
    }
}

template<class T>
T* ChainedObject<T>::getHead()
{
    return _head;
}


#endif // CHAINEDOBJECT_HPP_INCLUDED
