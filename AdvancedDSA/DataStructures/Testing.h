//
//  Testing.h
//  AdvancedDSA
//
//  Created by Dheeraj Vagavolu on 28/10/23.
//

#ifndef Testing_h
#define Testing_h
#include <functional>

namespace tests {
using TestFunc = std::function<bool()>;

bool test(int& count, const char* testName, TestFunc func) {
    std::cout << "Running " << testName << " ... ";
    if(func()) {
        std::cout << "PASSED\n";
        count += 1;
        return true;
    } else {
        std::cout << "FAILED\n";
        return false;
    }
}

}

#endif /* Testing_h */
