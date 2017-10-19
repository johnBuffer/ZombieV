#ifndef POOLOBJECT_HPP_INCLUDED
#define POOLOBJECT_HPP_INCLUDED

#include "System/ChainedObject.hpp"

template<class T>
class PoolObject
{
public:
    template<class...Args>
    static T* add(Args&&...);

    size_t getIndex() const {return m_index;}
    void remove();

    static size_t     size() {return s_pool.size();}
    static T*         getNext(T*& item);
    static T*         getObjectAt(size_t i);
    static Vector<T>& getObjects();
    static void       removeObject(size_t i);
    static void       resize(size_t size);

protected:
    size_t m_index;

private:
    static Pool<T> s_pool;
};

template<class T>
Pool<T> PoolObject<T>::s_pool(1);

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
T* PoolObject<T>::getObjectAt(size_t i)
{
    return &(s_pool.getPoolItemAt(i).object);
}

template<class T>
T* PoolObject<T>::getNext(T*& item)
{
    if (item)
    {
        PoolItem<T>& poolItem = s_pool.getPoolItemAt(item->m_index);

        int nextIndex = poolItem.nextObject;
        if (nextIndex != -1)
        {
            item = &(s_pool.getPoolItemAt(nextIndex).object);
        }
        else
            item = nullptr;
    }
    else
    {
        item = s_pool.getFirstItem();
    }

    //std::cout << item << " ptr" << std::endl;

    return item;
}

#endif // POOLOBJECT_HPP_INCLUDED
