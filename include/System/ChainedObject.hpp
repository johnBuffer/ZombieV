#ifndef CHAINEDOBJECT_HPP_INCLUDED
#define CHAINEDOBJECT_HPP_INCLUDED

#include <iostream>
#include <list>
#include <memory>

template<class T>
struct PoolItem
{
    T    object;
    int  index;
    bool isAviable;

    int nextFree;
    int nextObject;
    int prevObject;

    T* operator->();
    T& operator*();
};

template<class T>
T* PoolItem<T>::operator->()
{
    return &object;
};

template<class T>
T& PoolItem<T>::operator*()
{
    return object;
};

template<class T>
class Pool
{
public:
    Pool();
    Pool(size_t size);

    template<class...Args>
    size_t createObject(Args&&...);

    T& operator[](size_t i);

    bool getFirstItem(PoolItem<T>*& item);
    size_t size() {return m_size;}
    bool getNextItem(PoolItem<T>* item, PoolItem<T>*& nextItem);
    std::vector<PoolItem<T>>& getObjects() {return m_data;}

    void remove(int i);
    void resize(size_t size);

private:
    size_t m_size;
    int m_firstFree;
    int m_firstObject;
    std::vector<PoolItem<T>> m_data;

};

template<class T> using Vector = std::vector<PoolItem<T>>;
template<class T> using Ptr    = PoolItem<T>;

template<class T>
Pool<T>::Pool(size_t size) :
    m_size(0)
{
    resize(size);
}

template<class T>
void Pool<T>::resize(size_t size)
{
    m_firstFree   = -1;
    m_firstObject = -1;

    m_data.resize(size);

    for (size_t i(0); i<size; ++i)
    {
        m_data[i].index = i;

        m_data[i].isAviable = true;
        m_data[i].nextFree = i+1;
    }

    if (size)
    {
        m_data.back().nextFree = -1;
        m_firstFree = 0;
    }
}

template <class T>
template<class...Args>
size_t Pool<T>::createObject(Args&&... args)
{
    ++m_size;
    size_t index;
    if (m_firstFree != -1)
    {
        index = m_firstFree;
        m_firstFree = m_data[m_firstFree].nextFree;

        PoolItem<T>& poolItem = m_data[index];
        new(&poolItem.object) T(args...);
        poolItem.isAviable = false;

        poolItem.nextObject = m_firstObject;
        poolItem.prevObject = -1;

        if (m_firstObject != -1)
            m_data[m_firstObject].prevObject = index;

        m_firstObject = index;
    }
    else
    {
        m_data.push_back(PoolItem<T>());

        PoolItem<T>& newPoolItem = m_data.back();
        newPoolItem.index = m_data.size()-1;
        newPoolItem.isAviable = false;

        new(&(newPoolItem.object)) T(args...);
        index = newPoolItem.index;

        m_firstObject = index;
        newPoolItem.nextObject = -1;
        newPoolItem.prevObject = -1;
    }

    return index;
}

template<class T>
T& Pool<T>::operator[](size_t i)
{
    return m_data[i].object;
}

template<class T>
void Pool<T>::remove(int i)
{
    PoolItem<T>& poolItem = m_data[i];
    --m_size;
    poolItem.nextFree = m_firstFree;
    poolItem.isAviable = true;
    m_firstFree = i;

    if (poolItem.prevObject != -1)
    {
        m_data[poolItem.prevObject].nextObject = poolItem.nextObject;
    }

    if (poolItem.nextObject != -1)
    {
        m_data[poolItem.nextObject].prevObject = poolItem.prevObject;
    }
}

template<class T>
bool Pool<T>::getNextItem(PoolItem<T>* item, PoolItem<T>*& nextItem)
{
    if (item->nextObject != -1)
    {
        nextItem = &m_data[item->nextObject];
        return true;
    }

    return false;
}

template<class T>
bool Pool<T>::getFirstItem(PoolItem<T>*& item)
{
    if (m_firstObject != -1)
    {
        item = &m_data[m_firstObject];
        return true;
    }

    return false;
}


#endif // CHAINEDOBJECT_HPP_INCLUDED
