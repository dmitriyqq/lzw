#pragma once

#include <string>
#include <climits>

class BaseDictionary {
protected:
    void initializeDictionary() {
        for (int i = 0; i <= UCHAR_MAX; i++) {
            insert(i, i);
        }
    }

    virtual void insert(char c, uint32_t code) = 0;
};
