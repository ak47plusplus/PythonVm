#ifndef TRAIT_MAP_HPP__
#define TRAIT_MAP_HPP__

#include <map>

// Trait
template<typename K, typename V>
struct Map {
    typedef std::map<K,V> type;
};

// Usage:
// Map<int,std::string>::type m_Map;
//

#endif
