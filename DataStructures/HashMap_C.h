//
//  HashMap_C.h
//  AdvancedDSA
//
//  Created by Dheeraj Vagavolu on 28/10/23.
//

#ifndef HashMap_C_h
#define HashMap_C_h

#include "LinkedList.h"
#include "Testing.h"
#include <functional>
#include <memory>

namespace chaining {

template <class K, class T>
class HashMap{
    static const size_t ARRAY_SIZE = 100;
    size_t currentSize;
    size_t currentMembers;
    float allowedLoadFactor;
    std::unique_ptr<linkedlist::LinkedList<K, T>[]> map;
    
    using HashFuncType = std::function<size_t(const K&)>;
    HashFuncType hashFunction;
    
    static size_t defaultHash(const K key){
        return std::hash<K>()(key);
    }
    
    size_t getIndex(const K key){
        return defaultHash(key) % currentSize;
    }
    
public:
    HashMap():currentSize(ARRAY_SIZE), allowedLoadFactor(0.75f), map(std::make_unique<linkedlist::LinkedList<K, T>[]>(ARRAY_SIZE)), hashFunction(defaultHash) {}
    
    size_t getCurrentSize(){
        return currentSize;
    }
    
    size_t getCurrentMembers(){
        return currentMembers;
    }
    
    float getLoadFactor(){
        return (float) currentMembers / (float) currentSize;
    }
    
    bool shouldReHash(){
        if (getLoadFactor() > allowedLoadFactor){
            return true;
        }
        return false;
    }
    
    bool insert(K key, T val){
        size_t index = getIndex(key);
        if(map[index].insert(key, val)){
            currentMembers += 1;
            if(shouldReHash()){
                reHash();
            }
            return true;
        }
        return false;
    }
    
    // overload index operator
    linkedlist::Node<K, T>* operator[](K key){
        return find(key);
    }
    
    bool deleteNode(K key){
        size_t index = getIndex(key);
        if(map[index].deleteNodeKey(key)){
            currentMembers -= 1;
            return true;
        }
        return false;
    }
    
    linkedlist::Node<K, T>* find(K key){
        size_t index = getIndex(key);
        return map[index].find(key);
    }
    
    bool reset(){
        try{
            map = std::make_unique<linkedlist::LinkedList<K, T>[]>(currentSize);
            currentMembers = 0;
            return true;
        }catch(...){
            return false;
        }
    };
    
    void reHash(){
        // load factor is greater than 0.75
        // create a new map with twice the capacity
        std::unique_ptr<linkedlist::LinkedList<K, T>[]> newMap;
        newMap = std::make_unique<linkedlist::LinkedList<K, T>[]>(2 * currentSize);
        
        // this effectively changes the hash function
        currentSize *= 2;
        
        for(int i = 0; i < currentSize/2; i++){
            // traverse each list, and hash the key to the new value
            // it is a linkedList it has a smart pointer
            auto ptr = map[i].getHead();
            while(ptr){
                newMap[getIndex(ptr->key)].insert(ptr->key, ptr->data);
                ptr = ptr->next.get();
            }
        }
        map = std::move(newMap);
        newMap = nullptr;
        
    }
    
};

bool testInsertAndFind() {
    HashMap<int, std::string> map;
    
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");
    if(map.find(1)->data != "one") return false;
    if(map.find(2)->data != "two") return false;
    if(map.find(3)->data != "three") return false;
    
    // testing overloading operator
    if(map[3]->data != "three") return false;
    
    if(map.find(4) != nullptr) return false;  // Key 4 doesn't exist, so should return nullptr
    return true;
}

bool testDelete() {
    HashMap<int, std::string> map;
    
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");
    if(map.deleteNode(2) != true) return false;
    if(map.find(2) != nullptr) return false;
    if(map.deleteNode(4) != false) return false;

    return true;
}

bool testDuplicateInserts() {
    HashMap<int, std::string> map;
    
    map.insert(1, "one");
    map.insert(1, "first");
    
    if(map[1]->data != "first") return false; // Assuming that the second insert updates the value
    return true;
}

bool testReset() {
    HashMap<int, std::string> map;

    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");
    map.reset();

    if(map.find(1) != nullptr) return false;
    if(map.find(2) != nullptr) return false;
    if(map.find(3) != nullptr) return false;
    return true;
}

bool testSize() {
    HashMap<int, std::string> map;

    // Initial insertions and size checks
    map.insert(1, "one");
    if(map.getCurrentSize() != 100 || map.getCurrentMembers() != 1) return false;
    
    
    
    map.insert(2, "two");
    if(map.getCurrentSize() != 100 || map.getCurrentMembers() != 2) return false;
    
    
    map.insert(3, "three");
    if(map.getCurrentSize() != 100 || map.getCurrentMembers() != 3) return false;
    

    // Insert enough elements to trigger a resize
    for(int i = 4; i <= 80; i++) {
        map.insert(i, std::to_string(i));
    }
    
    if(map.getCurrentSize() != 200 || map.getCurrentMembers() != 80) return false;  // Check that the size has doubled

    // Check if data is still accessible after rehashing
    if(map.find(79)->data != "79") return false;

    return true;
}

void runTests() {
    
    int count = 0;
    int total = 5;  // Updated total count
    
    
    
    tests::test(count, "Testing Insert and Find", testInsertAndFind);
    tests::test(count, "Testing Delete", testDelete);
    tests::test(count, "Testing Duplicate Inserts", testDuplicateInserts);
    tests::test(count, "Testing Reset", testReset);
    tests::test(count, "Testing Size", testSize);
    
    std::cout << std::string(40, '-') << "\n";
    std::cout << " -- Chaining::HashMap: Passed [" << count << "/" << total << "] tests -- " << std::endl;
    
    std::cout << std::string(40, '-') << "\n\n";
}

}

#endif /* HashMap_C_h */
