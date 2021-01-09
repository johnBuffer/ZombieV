#ifndef CHAINEDOBJECT_HPP_INCLUDED
#define CHAINEDOBJECT_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <list>
#include <memory>


/* This classes describes a collection of items chained together
but in contiguous memory.

So each used slot is connected to its next and previous
and each free slot is just added in a simple chain of free
slots

O(1) access, deletion and insertion */

////////////////////////////////////////////////////////////

/* A PoolItem is a slot of the Pool, containing
chain information and T object data */
template<class T>
struct PoolItem
{
    PoolItem() :
        nextFree(-1),
        nextObject(-1),
        prevObject(-1)
    {}

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

//////////////////////////////////////////////////////////

/* The pool itself containing all the PoolItems
It exposes methods to create, access and delete elements */
template<class T>
class Pool
{
public:
    Pool();
    Pool(uint32_t size);

    template<class...Args>
    uint32_t createObject(Args&&...);

    T&   operator[](uint32_t i);
	T*   getItemAt(uint32_t i);
    T*   getFirstItem();
	T*   getNext(T*& item);
    bool getNextItem(PoolItem<T>* item, PoolItem<T>*& nextItem);
    
	uint32_t size() {return m_size;}
    PoolItem<T>* getPoolItemAt(uint32_t i);
    std::vector<PoolItem<T>>& getObjects() {return m_data;}

    void remove(int i);
    void resize(uint32_t size);

private:
    uint32_t m_size; // Number of used slots
    int m_firstFree; // Index of the first free slot that can be used
    int m_firstObject; // Index of the first used slot -> start point to iterate only on used slots
    std::vector<PoolItem<T>> m_data; // Raw contiguous data
};

// Some syntax sugar
template<class T> using Vector = std::vector<PoolItem<T>>;
template<class T> using Ptr    = PoolItem<T>;

template<class T>
Pool<T>::Pool(uint32_t size) :
    m_size(0)
{
    resize(size);
}

// Initialize data
template<class T>
void Pool<T>::resize(uint32_t size)
{
    m_firstFree   = -1;
    m_firstObject = -1;

    m_data.resize(size);

    for (uint32_t i(0); i<size; ++i)
    {
        m_data[i].index = i;
        m_data[i].isAviable = true;
        m_data[i].nextFree = i+1;
        m_data[i].nextObject = -1;
    }

    if (size)
    {
        m_data.back().nextFree = -1;
        m_firstFree = 0;
    }
}

// Handles object creation
template <class T>
template<class...Args>
uint32_t Pool<T>::createObject(Args&&... args)
{
    ++m_size; // Update size
    uint32_t index;
    PoolItem<T>* newPoolItem = nullptr;

    if (m_firstFree != -1) // If empty slot available
    {
        index = m_firstFree; // Get a free slot
        m_firstFree = m_data[index].nextFree; // Update next free slot
        newPoolItem = &m_data[index];
    }
    else // We need to create a new one
    {
        std::cout << "Capacity overflow" << std::endl;
        m_data.push_back(PoolItem<T>()); // Create new slot

        newPoolItem = &m_data.back();
        newPoolItem->index = static_cast<int>(m_data.size())-1;
        index = newPoolItem->index; // Update index
    }

    new(&newPoolItem->object) T(args...); // Create new T object in its slot
    newPoolItem->isAviable = false; // Update status
    newPoolItem->nextObject = m_firstObject; // Add it to the chain
    newPoolItem->prevObject = -1; // No previous since first

    if (m_firstObject != -1) // If first ever
        m_data[m_firstObject].prevObject = index;

    m_firstObject = index; // New head for iteration

    return index;
}


// Simple T object access
template<class T>
T& Pool<T>::operator[](uint32_t i)
{
    return m_data[i].object;
}

template<class T>
inline T* Pool<T>::getItemAt(uint32_t i)
{
	return &(m_data[i].object);
}

// Removes a T object from the pool
template<class T>
void Pool<T>::remove(int i)
{
    --m_size;
    PoolItem<T>& poolItem = m_data[i];
    poolItem.nextFree = m_firstFree;
    poolItem.isAviable = true;
    m_firstFree = i;

    // Update the chain
    int nextObject = poolItem.nextObject;
    int prevObject = poolItem.prevObject;

    if (prevObject != -1)
    {
        m_data[prevObject].nextObject = nextObject;
    }
    else
    {
        m_firstObject = poolItem.nextObject;
    }

    if (nextObject != -1)
    {
        m_data[nextObject].prevObject = prevObject;
    }
}

// Allows iteration on used slots
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

// Returns Head of used slots chain
template<class T>
T* Pool<T>::getFirstItem()
{
    T* item = nullptr;
    if (m_firstObject != -1)
    {
        item = &(m_data[m_firstObject].object);
    }

    return item;
}

// Simple PoolItem access
template<class T>
PoolItem<T>* Pool<T>::getPoolItemAt(uint32_t i)
{
    PoolItem<T>* item = &m_data[i];
    if (item->isAviable)
        return nullptr;

    return item;
}

template<class T>
T* Pool<T>::getNext(T*& item)
{
	// If we have a start point
	if (item)
	{
		// Search for the next
		PoolItem<T>& poolItem = *getPoolItemAt(item->m_index);

		int nextIndex = poolItem.nextObject;
		if (nextIndex != -1)
		{
			item = &(m_data[nextIndex]->object);
		}
		else // Does not have next
			item = nullptr;
	}
	else // No start point -> returns the first object
	{
		item = getFirstItem();
	}

	return item;
}


#endif // CHAINEDOBJECT_HPP_INCLUDED
