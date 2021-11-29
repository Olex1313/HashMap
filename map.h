#ifndef HASHMAP_MAP_H
#define HASHMAP_MAP_H

#define HASH_MAP_SIZE 1
#define BALANCE 0.75

#include <functional>

template<typename K, typename V>
struct Node {
    K key;
    V value;
};

template<typename K, typename V>
class HashMap {
public:
    HashMap();
    HashMap(const HashMap<K, V> &parent);
    ~HashMap();

    int size() const;

    void clear();
    void add(const K &key, const V &value);
    void remove(const K &key);
    void load(const std::string &filename);
    void dump(const std::string &filename) const;

    bool contains(const K &key) const;

    K max_key() const;
    K min_key() const;

    HashMap<K, V>& operator<< (const K &key);
    V& operator[] (const K &key);

private:
    void _balance();

    int _size;
    int _max_size;

    Node<K, V> *_hash_map[HASH_MAP_SIZE];
    std::hash<K> _hash_function;
};

#include "map_source.h"
#endif //HASHMAP_MAP_H
