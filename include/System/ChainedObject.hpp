#ifndef CHAINEDOBJECT_HPP_INCLUDED
#define CHAINEDOBJECT_HPP_INCLUDED

#include <iostream>

template<class T>
class ChainedObject
{
public:
    static T* getHead();
    static T* getTail();

    T* next;
    T* prev;

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
        _head->next = obj;
        obj->prev = _head;
        obj->next = nullptr;
        _head = obj;
    }
    else
    {
        _head = obj;
        _head->next = nullptr;
        _head->prev = nullptr;
    }
}

template<class T>
void ChainedObject<T>::remove(T* obj)
{
    T* next = static_cast<T*>(obj->next);
    T* prev = static_cast<T*>(obj->prev);

    if (next)
    {
        next->prev = prev;
    }
    else
    {
        _head = prev;
    }

    if (prev)
    {
        prev->next = next;
    }
}

template<class T>
T* ChainedObject<T>::getHead()
{
    return _head;
}


#endif // CHAINEDOBJECT_HPP_INCLUDED
