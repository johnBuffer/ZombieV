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

    static void removeObject(size_t i);
    static Vector<T>& getObjects();

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
Vector<T>& PoolObject<T>::getObjects()
{
    return s_pool.getObjects();
}

#endif // POOLOBJECT_HPP_INCLUDED
