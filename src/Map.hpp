#ifndef MAP_HPP__
#define MAP_HPP__

#include <map>

// please use std::map
template<typename K, typename V>
struct Map {
    typedef std::map<K,V> type;
};

// Usage:
// Map<int,std::string>::type m_Map;
//
