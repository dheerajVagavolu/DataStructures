//
//  HashMap_P.h
//  AdvancedDSA
//
//  Created by Dheeraj Vagavolu on 28/10/23.
//

#ifndef HashMap_P_h
#define HashMap_P_h
#include "LinkedList.h"
#include "Testing.h"
#include <memory>
#include <functional>

namespace probing {

enum class STATUS {
    EMPTY,
    OCCUPIED,
    TOMBSTONE
};

template <class K, class T>
struct Node {
    K key;
    T data;
    STATUS status;
    
    Node(): status(STATUS::EMPTY) {}
};

template <class K, class T>
class HashMap{
    
    size_t currentSize, currentMembers;
    float allowedLoadFactor;
    
    std::unique_ptr<Node<K, T>[]> map;
    
    using hashFuncType = std::function<size_t(const K)>;
    
    static size_t defaultHash(const K key) {
        return std::hash<K>()(key);
    }
    
    size_t getIndex(const K key){
        return defaultHash(key) % currentSize;
    }
    
    hashFuncType hashFunction;
    
public:
    HashMap(hashFuncType customHash = defaultHash): currentSize(100), allowedLoadFactor(0.5f),
    map(std::make_unique<Node<K, T>[]>(100)),
    hashFunction(customHash) {}

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
        // insert should only avoid occupied
        size_t index = hashFunction(key);
        while(map[index].status == STATUS::OCCUPIED){
            if(map[index].key == key){
                map[index].data = val;
                return true;
            }
            index += 1;
        }
        // found the right index
        map[index].key = key;
        map[index].data = val;
        map[index].status = STATUS::OCCUPIED;
        currentMembers += 1;
        if(getLoadFactor() > allowedLoadFactor){
            rehash();
        }
        return true;
    }
    
    Node<K, T>* find(K key){
        // search should be over tombstones
        size_t index = hashFunction(key);
        while(map[index].status != STATUS::EMPTY){
            if(map[index].key == key){
                return &map[index];
            }
            index += 1;
        }
        return nullptr;
    }
    
    bool deleteNode(K key){
        auto it = find(key);
        if(it){
            *it = Node<K, T>();
            currentMembers -= 1;
            return true;
        }
        return false;
    }
    
    bool reset(){
        try{
            map = std::make_unique<Node<K, T>[]>(currentSize);
            currentMembers = 0;
            return true;
        }catch(...){
            return false;
        }
    }
    
    void rehash(){
        std::unique_ptr<Node<K, T>[]> newMap = std::make_unique<Node<K, T>[]>(2* currentSize);
        
        currentSize *= 2;
        
        for(int i = 0; i < currentSize/2; i++){
            if(map[i].status == STATUS::OCCUPIED){
                newMap[getIndex(map[i].key)].key = map[i].key;
                newMap[getIndex(map[i].key)].data = map[i].data;
                newMap[getIndex(map[i].key)].status = STATUS::OCCUPIED;
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
    if(map.find(3)->data != "three") return false;
    
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
    
    if(map.find(1)->data != "first") return false; // Assuming that the second insert updates the value
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
    
//    std::cout << "\nSize: " << map.getCurrentSize() << ", Members: " << map.getCurrentMembers() << ", Load:" << map.getLoadFactor();
    
    // Insert enough elements to trigger a resize
    for(int i = 4; i <= 51; i++) {
        map.insert(i, std::to_string(i));
    }
    
//    std::cout << "\nSize: " << map.getCurrentSize() << ", Members: " << map.getCurrentMembers()  << ", Load:" << map.getLoadFactor() << std::endl;
    
    
    
    if(map.getCurrentSize() != 200 || map.getCurrentMembers() != 51) return false;  // Check that the size has doubled

    // Check if data is still accessible after rehashing
    if(map.find(49)->data != "49") return false;
    if(map.find(34)->data != "34") return false;

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
    std::cout << " -- Probing::HashMap: Passed [" << count << "/" << total << "] tests -- " << std::endl;
    std::cout << std::string(40, '-') << "\n\n";
}


}


#endif /* HashMap_P_h */
