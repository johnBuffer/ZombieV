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
    static T& add(Args&&...);

    static T& add(const T& obj);
    static void remove(const T& obj);

    static std::list<Shared<T>>& getObjects();

protected:
    static std::list<Shared<T>> _classData;
};

template<class T> std::list<Shared<T>> ChainedObject<T>::_classData;

template<class T>
template<class ... Args>
T& ChainedObject<T>::add(Args&&... args)
{
    _classData.push_back(std::make_shared<T>(args...));
    return *(_classData.back());
}


template<class T>
T& ChainedObject<T>::add(const T& obj)
{
    _classData.push_back(obj);
    return _classData.back();
}

template<class T>
void ChainedObject<T>::remove(const T& obj)
{
    // hm hm
    auto it = _classData.begin();

    for (it; it != _classData.end(); it++)
    {
        T* ptr = &(*it);
        if (ptr == &obj)
        {
            _classData.remove(it);
            break;
        }
    }
}

template<class T>
std::list<Shared<T>>& ChainedObject<T>::getObjects()
{
    return _classData;
}

#endif // CHAINEDOBJECT_HPP_INCLUDED
