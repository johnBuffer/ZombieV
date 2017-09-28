#ifndef CONTAINER_HPP_INCLUDED
#define CONTAINER_HPP_INCLUDED

#include <iostream>
#include <array>

template <class T, size_t N>
class Container
{
public:
    Container();
    void add(T newObj);
    void remove(int i);

    size_t size() const;
    int getHead() const;
    int getPrev(int i) const;
    int getNext(int i) const;
    T& operator[](int i);

private:
    size_t _freeHead;
    size_t _freeTail;
    size_t _size;
    int _head;

    T*   _data;
    int* _next;
    int* _prev;
};

template <class T, size_t N>
Container<T, N>::Container() :
    _freeHead(0),
    _freeTail(N-1),
    _size(0),
    _head(0)
{
    std::cout << "init" << std::endl;

    _data = new T[N];
    _prev = new int[N];
    _next = new int[N];

    for (size_t i(N); i--;)
    {
        _next[i] = i+1;
        _prev[i] = i-1;
    }

    _prev[0] = N-1;
    _next[N-1] = 0;

    std::cout << "init ok" << std::endl;
}

template <class T, size_t N>
void Container<T, N>::add(T newObj)
{
    _data[_freeHead] = newObj;
    _freeHead = _next[_freeHead];
    _size = std::min(_size+1, N);
}

template <class T, size_t N>
void Container<T, N>::remove(int i)
{
    --_size;

    /// Updates indexes
    if (i == _head)
        _head = _next[i];

    _next[_prev[i]] = _next[i];
    _prev[_next[i]] = _prev[i];

    /// Updates free cells
    _next[_freeTail] = i;
    _prev[i] = _freeTail;
    _freeTail = i;
}

template <class T, size_t N>
size_t Container<T, N>::size() const
{
    return _size;
}

template <class T, size_t N>
int Container<T, N>::getHead() const
{
    return _head;
}

template <class T, size_t N>
int Container<T, N>::getPrev(int i) const
{
    return _prev[i];
}

template <class T, size_t N>
int Container<T, N>::getNext(int i) const
{
    return _next[i];
}

template <class T, size_t N>
T& Container<T, N>::operator[](int i)
{
    return _data[i];
}

#endif // CONTAINER_HPP_INCLUDED
