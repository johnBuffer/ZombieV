#ifndef CHAINEDOBJECT_HPP_INCLUDED
#define CHAINEDOBJECT_HPP_INCLUDED

#include <iostream>
#include <list>
#include <memory>

template<class T>
using Shared = std::shared_ptr<T>;

template<class T>
using ListPtr = std::list<Shared<T>>;

template<class T>
class ChainedObject
{
public:

    template<class ... Args>
    static T* add(Args&&...);

    void remove();
    static ListPtr<T>& getObjects();

protected:
    typename ListPtr<T>::iterator m_iterator;

    static ListPtr<T> _classData;
};

/// Init static container
template<class T> std::list<Shared<T>> ChainedObject<T>::_classData;

/// Allows to create and store new objects
template<class T>
template<class... Args>
T* ChainedObject<T>::add(Args&&... args)
{
    Shared<T> newShared = std::make_shared<T>(args...);
    _classData.push_front(newShared);
    newShared->m_iterator = _classData.begin();
    return &(*_classData.front());
}

/// Removes an objects
template<class T>
void ChainedObject<T>::remove()
{
    _classData.erase(m_iterator);
}

/// Returns the container
template<class T>
std::list<Shared<T>>& ChainedObject<T>::getObjects()
{
    return _classData;
}

#endif // CHAINEDOBJECT_HPP_INCLUDED
