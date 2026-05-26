#pragma once
#include <vector>
#include <iostream>
#include "Pig.h"

class PigMemoryPool {
private:
    std::vector<Pig*> v_pool;
    std::vector<bool> v_available;
    int i_poolSize;

public:
    PigMemoryPool(int size) : i_poolSize(size) {
        v_pool.resize(size, nullptr);
        v_available.resize(size, true);
        std::cout << "PigMemoryPool created with " << size << " slots" << std::endl;
    }

    int acquire() {
        for (int i = 0; i < i_poolSize; i++) {
            if (v_available[i]) {
                v_available[i] = false;
                std::cout << "PigMemoryPool: acquired slot " << i << std::endl;
                return i;
            }
        }
        std::cout << "PigMemoryPool: no slots available" << std::endl;
        return -1;
    }

    void store(int index, Pig* pig) {
        if (index >= 0 && index < i_poolSize)
            v_pool[index] = pig;
    }

    Pig* get(int index) {
        if (index >= 0 && index < i_poolSize)
            return v_pool[index];
        return nullptr;
    }

    void release(int index) {
        if (index >= 0 && index < i_poolSize) {
            v_available[index] = true;
            std::cout << "PigMemoryPool: released slot " << index << std::endl;
        }
    }

    int activeCount() const {
        int count = 0;
        for (bool available : v_available)
            if (!available) count++;
        return count;
    }

    int poolSize() const { return i_poolSize; }

    ~PigMemoryPool() {
        for (int i = 0; i < i_poolSize; i++) {
            if (!v_available[i] && v_pool[i])
                delete v_pool[i];
        }
        std::cout << "PigMemoryPool destroyed" << std::endl;
    }
};