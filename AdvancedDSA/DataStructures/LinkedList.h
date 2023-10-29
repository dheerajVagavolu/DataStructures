//
//  LinkedList.h
//  AdvancedDSA
//
//  Created by Dheeraj Vagavolu on 28/10/23.
//

#ifndef LinkedList_h
#define LinkedList_h

#include <memory>
#include "Testing.h"

namespace linkedlist {

template <typename K, typename T>
struct Node {
    K key;
    T data;
    std::unique_ptr<Node<K, T>> next;
    Node(K k, T d): key(k), data(d), next(nullptr) {}
};

template <typename K, typename T>
class LinkedList{
    std::unique_ptr<Node<K, T>> head;
    
public:
    
    Node<K, T>* getHead(){
        return head.get();
    }
    
    bool insert(K key, T data) {
        if(!head){
            head = std::make_unique<Node<K, T>>(key, data);
            return true;
        }
        
        auto it = find(key);
        if(it){
            // key already exists, replace the value
            it->data = data;
            return false;
        }
        
        auto temp = head.get();
        while(temp->next){
            temp = temp->next.get();
        }
        temp->next = std::make_unique<Node<K, T>>(key, data);
        return true;
    }
    
    // search by key
    Node<K, T>* find(K key) {
        if(!head){
            return head.get();
        }
        auto temp = head.get();
        while(temp){
            if(temp->key == key){
                return temp;
            }
            temp = temp->next.get();
        }
        return nullptr;
    };
    
    // delete by key, returns status if deleted or not
    bool deleteNodeKey(K key){
        if(!head){
            return false;
        }
        
        Node<K, T>* prev = nullptr;
        auto temp = head.get();
        
        while(temp){
            if(temp->key == key){
                
                // no prev
                if(!prev){
                    if(temp->next){
                        head = std::move(head->next);
                    }else{
                        head = nullptr;
                    }
                }else{
                    prev->next = std::move(temp->next);
                }
                return true;
                
            }
            prev = temp;
            temp = temp->next.get();
        }
        
        return false;
        
    }
    
    bool deleteNodeVal(T data){
        if(!head){
            return false;
        }
        Node<K, T>* prev = nullptr;
        auto temp = head.get();
        while(temp){
            if(temp->data == data){
                
                if(!prev){
                    if(!temp->next){
                        head = nullptr;
                    }else{
                        head = std::move(head->next);
                    }
                }else{
                    prev->next = std::move(temp->next);
                }
                
                return true;
            }
            prev = temp;
            temp = temp->next.get();
        }
        
        return false;
    }
};

// Tests for LinkedList

bool testInsertAndFind() {
    LinkedList<int, std::string> linkedList;
    
    linkedList.insert(1, "one");
    linkedList.insert(2, "two");
    linkedList.insert(3, "three");
    if(linkedList.find(1)->data != "one") return false;
    if(linkedList.find(2)->data != "two") return false;
    if(linkedList.find(3)->data != "three") return false;
    if(linkedList.find(4) != nullptr) return false;  // Key 4 doesn't exist, so should return nullptr
    return true;
}

bool testDeleteByKey() {
    LinkedList<int, std::string> linkedList;

    linkedList.insert(1, "one");
    linkedList.insert(2, "two");
    linkedList.insert(3, "three");

    if(linkedList.deleteNodeKey(2) != true) return false;
    if(linkedList.find(2) != nullptr) return false;
    if(linkedList.deleteNodeKey(4) != false) return false;
    return true;
}

bool testDeleteByValue() {
    LinkedList<int, std::string> linkedList;

    linkedList.insert(1, "one");
    linkedList.insert(2, "two");
    linkedList.insert(3, "three");

    if(linkedList.deleteNodeVal("two") != true) return false;
    if(linkedList.find(2) != nullptr) return false;
    if(linkedList.deleteNodeVal("four") != false) return false;
    return true;
}

bool testInsertDuplicates() {
    LinkedList<int, std::string> linkedList;

    linkedList.insert(1, "one");
    linkedList.insert(1, "two");

    if(linkedList.find(1)->data != "two") return false;
    return true;
}

bool testEmptyListOperations() {
    LinkedList<int, std::string> linkedList;

    if(linkedList.deleteNodeKey(1) != false) return false;
    if(linkedList.deleteNodeVal("test") != false) return false;
    return true;
}


void runTests() {
    
    int count = 0;
    int total = 5;
    
    
    
    tests::test(count, "Insert and Find Test", testInsertAndFind);
    tests::test(count, "Delete by Key Test", testDeleteByKey);
    tests::test(count, "Delete by Value Test", testDeleteByValue);
    tests::test(count, "Insert Duplicates Test", testInsertDuplicates);
    tests::test(count, "Empty List Test", testEmptyListOperations);
    
    std::cout << std::string(40, '-') << "\n";
    std::cout << " -- LinkedList: Passed [" << count << "/" << total << "] tests -- " << std::endl;
    std::cout << std::string(40, '-') << "\n\n";
}




}


#endif /* LinkedList_h */
