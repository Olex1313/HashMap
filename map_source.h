#ifndef HASHMAP_MAP_SOURCE_H
#define HASHMAP_MAP_SOURCE_H

#include <fstream>
#include <exception>
#include <functional>
#include <sstream>
#include <algorithm>
#include "map.h"

template <typename K, typename V>
HashMap<K, V>::HashMap() {
    _max_size = 5;
    _size = 0;
    _real_size = 0;
    _hash_function = std::hash<K>();
    Node<K, V> **tmp = new Node<K, V>*[10];
    _hash_map = tmp;
    for (int i = 0; i < _max_size; i++) {
        _hash_map[i] = nullptr;
    }
}

template <typename K, typename V>
HashMap<K, V>::HashMap(const HashMap<K, V> &parent) : HashMap() {
    std::vector<K> keys = parent.get_keys();
    for (auto key : keys) {
        add(key, parent[key]);
    }
}

template <typename K, typename V>
HashMap<K, V>::~HashMap() {
    clear();
}

template <typename K, typename V>
V& HashMap<K, V>::operator[](const K &key) {
    std::size_t index = _hash_function(key) % _max_size;
    for (int i = index; i < _max_size; i++) {
        if (_hash_map[index] == nullptr) {
            throw std::range_error("No such key");
        } else if (_hash_map[index]->key == key) {
            return _hash_map[index]->value;
        }
    }
    throw std::runtime_error("Map is broken");
}

template<typename K, typename V>
void HashMap<K, V>::clear() {
    for (int i = 0; i < _max_size; i++) {
        if (_hash_map[i] != nullptr) {
            delete _hash_map[i];
            _hash_map[i] = nullptr;
        }
    }
    _size = 0;
    _real_size = 0;
}

template <typename K, typename V>
int HashMap<K, V>::size() const {
    return _size;
}

template <typename K, typename V>
void HashMap<K, V>::add(K key, V value) {
    std::size_t index = _hash_function(key) % _max_size;
    for (int i = index; i < _max_size; i++) {
        if (_hash_map[index] == nullptr) {
            _hash_map[index] = new Node<K, V>;
            _hash_map[index]->key = key;
            _hash_map[index]->value = value;
            _size++;
            _real_size++;
            return;
        }
    }
}

template <typename K, typename V>
void HashMap<K, V>::remove(const K &key) {
    if (!contains(key)) {
        return;
    }
    std::size_t index;
    index = _hash_function(key) % _max_size;
    for (int i = index; i < _max_size; i++) {
        if (_hash_map[index]->key == key) {
            delete _hash_map[index];
            _hash_map[index] = nullptr;
            _real_size--;
            _size--;
            return;
        }
    }
    throw std::runtime_error("Not found key, that exists");
}

template <typename K, typename V>
HashMap<K, V>& HashMap<K, V>::operator<<(const K &key) {
    if (_real_size / _max_size > BALANCE) {
        _balance();
    }
    std::size_t index = _hash_function(key) % _max_size;
    if (contains(key)) {
        operator[](key) += 1;
        return *this;
    }
    for (int i = index; i < _max_size; i++) {
        if (_hash_map[index] == nullptr) {
            _hash_map[index] = new Node<K, V>;
            _hash_map[index]->key = key;
            _hash_map[index]->value = 1;
            _size++;
            _real_size++;
            return *this;
        }
    }
    return *this;
}

template <typename K, typename V>
bool HashMap<K, V>::contains(const K &key) const {
    std::size_t index = _hash_function(key) % _max_size;
    for (int i = index; i < _max_size; i++) {
        if (_hash_map[index] != nullptr) {
            if (_hash_map[index]->key == key) {
                return true;
            }
        }
    }
    return false;
}

template <typename K, typename V>
std::vector<K> HashMap<K, V>::get_keys() const {
    std::vector<K> keys;
    for (int i = 0; i < _max_size; i++) {
        if (_hash_map[i] != nullptr) {
            keys.push_back(_hash_map[i]->key);
        }
    }
    return keys;
}

template <typename K, typename V>
std::vector<V> HashMap<K, V>::get_values() const {
    std::vector<V> keys;
    for (int i = 0; i < _max_size; i++) {
        if (_hash_map[i] != nullptr) {
            keys.push_back(_hash_map[i]->value);
        }
    }
    return keys;
}

template<typename K, typename V>
V HashMap<K, V>::operator[](const K &key) const {
    std::size_t index;
    index = _hash_function(key) % _max_size;
    for (int i = index; i < _max_size; i++) {
        if (_hash_map[index]->key == key) {
            return _hash_map[index]->value;
        }
    }
    throw std::runtime_error("Safe [] operator is broken");
}

template<typename K, typename V>
void HashMap<K, V>::dump(const std::string &filename) const {
    std::vector<K> keys = get_keys();
    std::vector<V> values = get_values();
    std::ofstream f(filename);
    if (!f) {
        throw std::runtime_error("Couldn't open file");
    }
    for (int i = 0; i < size(); ++i) {
        f << keys[i] << ":" << values[i] << std::endl;
    }
}

template<typename K, typename V>
void HashMap<K, V>::load(const std::string &filename) {
    clear();
    std::ifstream f(filename);
    if (!f.is_open()) {
        throw std::runtime_error("Couldn't open file");
    }
    for (std::string line; std::getline(f, line); ) {
        K key;
        V value;
        std::string token;
        std::stringstream ss (line);
        std::getline(ss, token, ':');
        std::stringstream converter (token);
        converter >> key;
        converter.clear();
        std::getline(ss, token);
        converter.str(token);
        converter >> value;
        add(key, value);
    }
}

template<typename K, typename V>
void HashMap<K, V>::_balance() {
    int new_size = _max_size * 2;
    int old_size = _max_size;
    Node<K, V> *new_hash_map[new_size];
    std::vector<K> keys = get_keys();
    std::vector<V> values = get_values();
    clear();
    _hash_map = new_hash_map;
    for (int i = 0; i < old_size; i++) {
        add(keys[i], values[i]);
    }
}

template<typename K, typename V>
K HashMap<K, V>::max_key() const {
    std::vector<K> keys = get_keys();
    std::vector<V> values = get_values();
    auto max_value = 0;
    for (auto value : values) {
        if (value >= max_value) {
            max_value = value;
        }
    }
    for (auto key : keys) {
        if (this->operator[](key) == max_value) {
            return key;
        }
    }
    return nullptr;
}

template<typename K, typename V>
K HashMap<K, V>::min_key() const {
    std::vector<K> keys = get_keys();
    std::vector<V> values = get_values();
    auto min_value = 0;
    for (auto value : values) {
        if (value <= min_value) {
            min_value = value;
        }
    }
    for (auto key : keys) {
        if (this->operator[](key) == min_value) {
            return key;
        }
    }
    return nullptr;
}

template <typename K, typename V>
bool operator==(const HashMap<K, V> &lhs, const HashMap<K, V> &rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    std::vector<K> keys = lhs.get_keys();
    for (K key : keys) {
        if(!rhs.contains(key)) {
            return false;
        }
        if (lhs[key] != rhs[key]) {
            return false;
        }
    }
    return true;
}

template <typename K, typename V>
bool operator!=(const HashMap<K, V> &lhs, const HashMap<K, V> &rhs) {
    return !(lhs==rhs);
}

#endif //HASHMAP_MAP_SOURCE_H
