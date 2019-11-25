#ifndef MAP_HPP____
#define MAP_HPP____

#include "Core.hpp"
#include "VM.hpp"

#include <map>
#include <exception>
#include <stdexcept>

template<typename K, typename V>
class Map;

template<typename K, typename V>
struct MapEntry {
public:
    MapEntry() = default;
    MapEntry(K k, V v): m_K(k), m_V(v){}
    MapEntry(const MapEntry<K,V>& rhs);
    MapEntry& operator=(const MapEntry<K,V>& rhs);
    friend class Map<K,V>;
    K m_K;
    V m_V;
};

template<typename K, typename V>
MapEntry<K,V>::MapEntry(const MapEntry<K,V> &rhs)
{
    this->m_K = rhs.m_K;
    this->m_V = rhs.m_V;
}

template<typename K, typename V>
MapEntry<K,V>& MapEntry<K,V>::operator=(const MapEntry<K,V>& rhs)
{
    this->m_K = rhs.m_K;
    this->m_V = rhs.m_V;
}

template<typename _Tp>
class _Map_iterator {
public:
    typedef _Tp  value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
    typedef _Tp *const _BasePtr;
    typedef typename std::ptrdiff_t difference_type;

    typedef _Map_iterator<_Tp>  _Self;
public:
    explicit
    _Map_iterator(_BasePtr owner, int cursor=0)
        :owner_(owner), cursor_(cursor)
    {}
    _Self& 
    operator++() _GLIBCXX_NOEXCEPT
    {
        cursor_ += 1;
        return *this;
    } 
    _Self 
    operator++(int) _GLIBCXX_NOEXCEPT
    {
        _Self tmp = *this;
        cursor_ += 1;
        return tmp;
    }
    _Self& 
    operator--() _GLIBCXX_NOEXCEPT
    {
        cursor_ -= 1;
        return *this;
    }
    _Self 
    operator--(int) _GLIBCXX_NOEXCEPT
    {
        _Self tmp = *this;
        cursor_ -= 1;
        return tmp;
    }

    bool
    operator==(const _Self &__x) _GLIBCXX_NOEXCEPT
    {
        return owner_ == __x.owner_ 
            && cursor_ == __x.cursor_;
    }

    bool 
    operator!=(const _Self &__x) _GLIBCXX_NOEXCEPT
    {
        return owner_ != __x.owner_ 
        || cursor_ != __x.cursor_;
    }
    _Tp
    operator*() _GLIBCXX_NOEXCEPT
    {
        return *(owner_ + cursor_);
    }
    pointer
    operator->() _GLIBCXX_NOEXCEPT
    {
        return (pointer)(owner_ + cursor_);
    }
private:
    _BasePtr  owner_;
    int       cursor_;
};

#define DEFAULT_MAP_INIT_CAP 8

template<typename K, typename V>
class Map {
public:
    typedef K key_type;
    typedef V mapped_type;
    typedef _Map_iterator<MapEntry<K,V>> iterator;
public:
    Map();
    Map(int default_cap);
    Map(const Map<K,V>& rhs);
    Map(Map<K,V>&& rhs);
    Map<K,V>& operator=(const Map<K,V>& rhs);
    Map<K,V>& operator=(Map<K,V>&& rhs);
    V& operator[](const K &k);
    ~Map();
    int     size()     const {  return m_Size;}
    int     capacity() const {  return m_Capacity;}
    bool    empty()    const {  return m_Size == 0;}
    void    put(const K &k, const V &v);
    V       get(const K &k);
    K       get_key(int index);
    bool    contains_key(const K &k);
    bool    has_key(const K &k);       // alias contains_key
    void    erase(const K &k);
    int     index(const K &k);
    const MapEntry<K,V> *entries() const {return m_Entries;}
    iterator begin() const;
    iterator end() const;
private:
    void expand_capacity();
private:
    MapEntry<K,V> *m_Entries;
    int            m_Size;
    int            m_Capacity;
};

// If k matches the key of an element in the container, the function 
// returns a reference to its mapped value.
// If k does not match the key of any element in the container, the 
// function inserts a new element with that key and returns a reference to 
// its mapped value. Notice that this always increases the container size by one, 
// even if no mapped value is assigned to the element (the element is constructed using 
// its default constructor).
// Note: default compare use operator== but when pyobject* we prefer to use the 'equal'.
template<typename K, typename V>
V& Map<K,V>::operator[](const K &_kType)
{
    auto index = this->index(_kType);
    if(index != -1)
    {
        return m_Entries[index].m_V;
    }
    // not found then put one.
    if(m_Size + 1 > m_Capacity)
    {
        this->expand_capacity();
    }
    entries[m_Size].m_K = _kType;
    return entries[m_Size++].m_V;
}


template<typename K, typename V>
Map<K,V>::Map()
{
    m_Entries   = new MapEntry<K,V>[DEFAULT_MAP_INIT_CAP];
    m_Capacity  = DEFAULT_MAP_INIT_CAP;
    m_Size      = 0;
}

template<typename K, typename V>
Map<K,V>::Map(int default_cap)
{
    m_Entries   = new MapEntry<K,V>[default_cap];
    m_Capacity  = default_cap;
    m_Size      = 0;
}

template<typename K, typename V>
Map<K,V>::Map(const Map<K,V>& rhs)
{
    m_Entries   = new MapEntry<K,V>[rhs.m_Capacity];
    m_Capacity  = rhs.m_Capacity;
    m_Size      = rhs.m_Size;
    for (decltype(rhs.size()) i = 0; i < m_Size; i++)
    {
        m_Entries[i] = rhs.m_Entries[i];
    }
}

template<typename K, typename V>
Map<K,V>::Map(Map<K,V>&& rhs)
{
    m_Entries       = rhs.m_Entries;
    m_Capacity      = rhs.m_Capacity;
    m_Size          = rhs.m_Size;
    rhs.m_Entries   = nullptr;
    rhs.m_Capacity  = 0;
    rhs.m_Size      = 0;
}

template<typename K, typename V>
Map<K,V>& Map<K,V>::operator=(const Map<K,V>& rhs)
{
    if(m_Entries != nullptr)
        delete m_Entries;
    m_Entries   = new MapEntry<K,V>[rhs.m_Capacity];
    m_Capacity  = rhs.m_Capacity;
    m_Size      = rhs.m_Size;
    for (decltype(rhs.size()) i = 0; i < m_Size; i++)
    {
        m_Entries[i] = rhs.m_Entries[i];
    }
    return *this;
}

template<typename K, typename V>
Map<K,V>& Map<K,V>::operator=(Map<K,V>&& rhs)
{
    if(m_Entries != nullptr)
        delete m_Entries;
    m_Entries       = rhs.m_Entries;
    m_Capacity      = rhs.m_Capacity;
    m_Size          = rhs.m_Size;
    rhs.m_Entries   = nullptr;
    rhs.m_Capacity  = 0;
    rhs.m_Size      = 0;
    return *this;
}

template<typename K, typename V>
Map<K,V>::~Map()
{
    if(m_Entries != nullptr)
        delete []m_Entries;
}

template<typename K, typename V>
void Map<K,V>::put(const K &k, const V &v)
{
    for(decltype(size()) i = 0; i < m_Size; i++)
    {
        if(m_Entries[i].m_K == k)
        {
            m_Entries[i].m_V = v;
            return;
        }
    }
    if(m_Size >= m_Capacity)
    {
        this->expand_capacity();
    }
    m_Entries[m_Size++] = MapEntry<K,V>(k,v);
}

template<typename K, typename V>
V Map<K,V>::get(const K &k)
{
    int idx = index(k);
    if(idx < 0)
        return VM::PyNone;
    else
        return m_Entries[idx].m_V;
}

template<typename K, typename V>
K Map<K,V>::get_key(int idx)
{
    if(idx >= m_Size)
        throw std::out_of_range("index out of bounds.");
    return m_Entries[idx].m_K;
}

template<typename K, typename V>
bool Map<K,V>::contains_key(const K &k)
{
    return has_key(k);
}

template<typename K, typename V>    // alias contains_key
bool Map<K,V>::has_key(const K &k){
    return index(k) >= 0;
}

template<typename K, typename V>
void Map<K,V>::erase(const K &k)
{
    int idx = index(k);
    if(idx < 0)
        return;
    m_Entries[idx] = m_Entries[--m_Size];
}

template<typename K, typename V>
int Map<K,V>::index(const K &k)
{
    for(decltype(size()) i = 0; i < m_Size; i++)
    {
        if(m_Entries[i].m_K == k)
        {
            return i;
        }
    }
    return -1;
}

template<typename K, typename V>
void Map<K,V>::expand_capacity()
{
    int newCap = m_Capacity << 1;
    MapEntry<K,V> *newSpace = new MapEntry<K,V>[newCap];
    for(decltype(size()) i = 0; i < m_Size; i++)
    {
        newSpace[i] = m_Entries[i];
    }
    delete m_Entries;
    m_Entries = newSpace;
    m_Capacity = newCap;
}

template<typename K, typename V>
typename Map<K,V>::iterator Map<K,V>::begin() const
{
    return iterator(m_Entries, 0);
}

template<typename K, typename V>
typename Map<K,V>::iterator Map<K,V>::end() const
{
    return iterator(m_Entries, m_Size);
}

#endif
