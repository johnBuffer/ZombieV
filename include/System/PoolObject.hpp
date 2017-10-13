#ifndef POOLOBJECT_HPP_INCLUDED
#define POOLOBJECT_HPP_INCLUDED

#include "System/ChainedObject.hpp"

template<class T>
class PoolObject
{
public:
    template<class...Args>
    static T* add(Args&&...);

    void remove();

    static size_t     size() {return s_pool.size();}
    static bool       getFirst(PoolItem<T>*& item);
    static bool       getNext(PoolItem<T>* item, PoolItem<T>*& nextItem);
    static Vector<T>& getObjects();
    static void       removeObject(size_t i);
    static void       resize(size_t size);

protected:
    size_t m_index;

private:
    static Pool<T> s_pool;
};

template<class T>
Pool<T> PoolObject<T>::s_pool(10000);

template<class T>
template<class...Args>
T* PoolObject<T>::add(Args&&... args)
{
    size_t index = s_pool.createObject(args...);
    T* newObject = &s_pool[index];
    newObject->m_index = index;

    return newObject;
}

template<class T>
void PoolObject<T>::removeObject(size_t i)
{
    s_pool.remove(i);
}

template<class T>
void PoolObject<T>::remove()
{
    removeObject(m_index);
}

template<class T>
void PoolObject<T>::resize(size_t size)
{
    s_pool.resize(size);
}

template<class T>
Vector<T>& PoolObject<T>::getObjects()
{
    return s_pool.getObjects();
}

template<class T>
bool PoolObject<T>::getFirst(PoolItem<T>*& item)
{
    return s_pool.getFirstItem(item);
}

template<class T>
bool PoolObject<T>::getNext(PoolItem<T>* item, PoolItem<T>*& nextItem)
{
    return s_pool.getNextItem(item, nextItem);
}

#endif // POOLOBJECT_HPP_INCLUDED
