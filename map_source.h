#ifndef HASHMAP_MAP_SOURCE_H
#define HASHMAP_MAP_SOURCE_H

#include <fstream>
#include <exception>
#include <functional>
#include "map.h"

template<typename K, typename V>
HashMap<K, V>::HashMap() {
    _max_size = HASH_MAP_SIZE;
    _size = 0;
    _hash_function = std::hash<K>();
    for (int i = 0; i < _max_size; i++) {
        _hash_map[i] = nullptr;
    }
}

template<typename K, typename V>
HashMap<K, V>::~HashMap() {
    for (int i = 0; i < _max_size; i++) {
        if (_hash_map[i] != nullptr) {
            delete _hash_map[i];
        }
    }
}

#endif //HASHMAP_MAP_SOURCE_H
