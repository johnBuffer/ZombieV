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
    int  next;

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

    size_t size() {return m_size;}

    std::vector<PoolItem<T>>& getObjects() {return m_data;}

    void remove(int i);

private:
    size_t m_size;
    int m_firstFree;
    std::vector<PoolItem<T>> m_data;

};

template<class T> using Vector = std::vector<PoolItem<T>>;
template<class T> using Ptr    = PoolItem<T>;

template<class T>
Pool<T>::Pool(size_t size) :
    m_size(0)
{
    m_firstFree = -1;
    m_data.resize(size);

    for (size_t i(0); i<size; ++i)
    {
        m_data[i].index = i;

        m_data[i].isAviable = true;
        m_data[i].next = i+1;
    }

    if (size)
    {
        m_data.back().next = -1;
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
        m_firstFree = m_data[m_firstFree].next;

        new(&m_data[index].object) T(args...);
        m_data[index].isAviable = false;
    }
    else
    {
        m_data.push_back(PoolItem<T>());

        PoolItem<T>& newPoolItem = m_data.back();
        newPoolItem.index = m_data.size()-1;
        newPoolItem.isAviable = false;

        new(&(newPoolItem.object)) T(args...);
        index = newPoolItem.index;
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
    --m_size;
    m_data[i].next = m_firstFree;
    m_data[i].isAviable = true;
    m_firstFree = i;
}

#endif // CHAINEDOBJECT_HPP_INCLUDED
