#include <iostream>
#include <cassert>
#include "map.h"

int main() {
    // TEST 1, default constructor
    HashMap<std::string, int> map;
    assert(map.size() == 0);
    std::cout << "PASSED TEST 1" << std::endl;

    // TEST 2, add method
    int previous_size = map.size();
    map << "One";
    assert(map.size() == previous_size + 1);
    std::cout << "PASSED TEST 2" << std::endl;

    // TEST 3, add existing key
    previous_size = map.size();
    int previous_value = map["One"];
    map << "One";
    assert(map.size() == previous_size);
    assert(map["One"] == previous_value + 1);
    std::cout << "PASSED TEST 3" << std::endl;

    // TEST 4, check key in collection
    assert(map.contains("One"));
    std::cout << "PASSED TEST 4" << std::endl;

    // TEST 5, check value is returned
    assert(map["One"] == 2);
    std::cout << "PASSED TEST 5" << std::endl;

    // TEST 6, remove existing, remove unexisting
    previous_size = map.size();
    map.remove("nonexistent key");
    assert(map.size() == previous_size);
    map.remove("One");
    assert(map.size() == previous_size - 1);
    std::cout << "PASSED TEST 6" << std::endl;

    // TEST 7, not contains removed key
    assert(!map.contains("One"));
    std::cout << "PASSED TEST 7" << std::endl;

    // TEST 8, non-empty after clear() equal to default
    HashMap<std::string, int> default_map;
    map.clear();
    assert(default_map == map);
    std::cout << "PASSED TEST 8" << std::endl;

    // TEST 9, non-empty cope is equal to original, after key change
    // it becomes non-equal
    map << ("Key");
    HashMap<std::string, int> copy(map);
    assert(map == copy);
    map << ("Key");
    assert(map != copy);
    std::cout << "PASSED TEST 9" << std::endl;

    // TEST 10, restored from dump is equal to dumped one
    copy.clear();
    map.dump("test.txt");
    copy.load("test.txt");
    assert(map == copy);
    std::cout << "PASSED TEST 10" << std::endl;

    // ADDITIONAL TESTS, used for checking _balance()
    map << "Two";
    std::cout << "PASSED ADDITIONAL TESTS" << std::endl;
    map.add("Three", 3);
    std::cout << "PASSED ADDITIONAL TESTS" << std::endl;
    map << "Three";
    std::cout << "PASSED ADDITIONAL TESTS" << std::endl;
    assert(map["Three"] == 4);
    map << "Four";
    map.add("5", 10);
    assert(map["5"] == 10);
    std::cout << "PASSED ADDITIONAL TESTS" << std::endl;

    std::cout << "Key with max errors : " << map.max_key() << std::endl;
    std::cout << "PASSED ALL TESTS" << std::endl;

    return 0;
}