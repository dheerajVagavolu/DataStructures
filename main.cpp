//
//  main.cpp
//  AdvancedDSA
//
//  Created by Dheeraj Vagavolu on 28/10/23.
//

#include <iostream>
#include <string>
#include <cassert>
#include "DataStructures/LinkedList.h"
#include "DataStructures/HashMap_C.h"
#include "DataStructures/HashMap_P.h"

int main(int argc, const char * argv[]) {
    
    linkedlist::runTests();
    chaining::runTests();
    probing::runTests();
    
    return 0;
}
