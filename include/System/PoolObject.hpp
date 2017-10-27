#ifndef POOLOBJECT_HPP_INCLUDED
#define POOLOBJECT_HPP_INCLUDED

#include "System/ChainedObject.hpp"

/* Wrapper to allow creation of "Pooled object"
e.g. objects that statically handle the storage of their own instances*/

///////////////////////////////////////////////////////////////////////

template<class T>
class PoolObject
{
public:
    template<class...Args>
    static T* add(Args&&...);

    size_t   getIndex() const {return m_index;}
    uint64_t getGlobalIndex() const;

    void remove();

    static size_t     size() {return s_pool.size();}
    static T*         getNext(T*& item);
    static T*         getObjectAt(size_t i);
    static Vector<T>& getObjects();
    static void       removeObject(size_t i);
    static void       resize(size_t size);

protected:
    size_t m_index;
    static size_t m_classID;

private:
    static Pool<T> s_pool;
};

template<class T>
Pool<T> PoolObject<T>::s_pool(10000);

template<class T>
size_t PoolObject<T>::m_classID;

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
    return &(s_pool.getPoolItemAt(i)->object);
}

// Returns the next object in the chain
template<class T>
T* PoolObject<T>::getNext(T*& item)
{
    if (item) // If we have a start point
    {
        // Search for the next
        PoolItem<T>& poolItem = *(s_pool.getPoolItemAt(item->m_index));

        int nextIndex = poolItem.nextObject;
        if (nextIndex != -1)
        {
            item = &(s_pool.getPoolItemAt(nextIndex)->object);
        }
        else // Does not have next
            item = nullptr;
    }
    else // No start point -> returns the first object
    {
        item = s_pool.getFirstItem();
    }

    return item;
}

// Returns the global index of the T object
// Global means that this index is unique even considering other PoolObject<T>

template<class T>
uint64_t PoolObject<T>::getGlobalIndex() const
{
    uint64_t globalID = ((uint64_t) m_classID << 32) | m_index;

    return globalID;
}



#endif // POOLOBJECT_HPP_INCLUDED
