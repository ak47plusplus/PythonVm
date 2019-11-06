#ifndef MAP_HPP____
#define MAP_HPP____

template<typename K, typename V>
class MapEntry {
public:
    MapEntry() = default;
    MapEntry(K k, V v): m_K(k), m_V(v){}
    MapEntry(const MapEntry<K,V>& rhs);
    MapEntry& operator=(const MapEntry<K,V>& rhs);
private:
    K m_K;
    V m_V;
};

template<typename K, typename V>
MapEntry::MapEntry(const MapEntry<K,V> &rhs)
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

template<typename K, typename V>
class Map {
public:
    Map();
    int     size()     const {return m_Size;}
    int     capacity() const { return m_Capacity;}
    void    put(K k, V v);
    V       get(K k);
    K       get_key(int index);
    bool    contains_key(K k);
    bool    has_key(K k);       // alias contains_key
    V       remove(K k);
    int     index(K k);
    const MapEntry<K,V> *entries() const {return m_Entries;}
private:
    void expand_capacity();
private:
    MapEntry<K,V> *m_Entries;
    int            m_Size;
    int            m_Capacity;
};

#endif
