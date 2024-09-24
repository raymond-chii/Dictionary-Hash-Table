#include "hash.h"

hashTable::hashTable(int size){
    capacity = getPrime(size);
    filled = 0;
    data.resize(capacity);
}

unsigned int hashTable::getPrime(int size){
    static const std::array<unsigned int, 12> primes = {1949, 7877, 11551, 52639, 220579, 
        500069, 700319, 800647, 1000099, 3000073, 6000641, 9001481};
    for (unsigned int prime : primes){
        if (prime >= size){
            return prime;
        }
    }
    return primes.back();
}

int hashTable::hash(const std::string &key){
    uint32_t hashVal = 0;
    for (char c : key) {
        hashVal = 37 * hashVal + static_cast<unsigned char>(c);
    }
    return hashVal % capacity;
}

int hashTable::findPos(const std::string &key){
    unsigned int pos = hash(key);
    unsigned int start = pos;

    while (data[pos].isOccupied)
    {
        if (data[pos].key == key){
            return pos;
        }
        
        pos = (pos + 1) % capacity;

        if (pos == start){
            break;
        }
    }
    
    return -1;

}

int hashTable::insert(const std::string &key, void *pv) {
    if (filled > capacity * 0.5) {  // Check rehash at 50% capacity
        if (!rehash()) {
            return 2;  // Rehash failed
        }
    }

    int pos = findPos(key);
    if (pos != -1 && data[pos].isOccupied && data[pos].key == key) {
        return 1;  // Key already exists
    }

    // Insert at the found position or the next available slot
    if (pos == -1) {
        pos = hash(key);
        while (data[pos].isOccupied) {
            pos = (pos + 1) % capacity;
        }
    }

    data[pos].key = key;
    data[pos].isOccupied = true;
    data[pos].pv = pv;
    filled++;

    return 0;  // Success
}

bool hashTable::contains(const std::string &key){
    int pos = findPos(key);
    return (pos != -1 && data[pos].key == key);
}

bool hashTable::rehash() {
    int newCapacity = getPrime(capacity * 2);
    std::vector<hashItem> newData(newCapacity);

    for (const auto &item : data) {
        if (item.isOccupied) {
            int newPos = hash(item.key) % newCapacity;
            while (newData[newPos].isOccupied) {
                newPos = (newPos + 1) % newCapacity;
            }
            newData[newPos] = std::move(item);
        }
    }

    data = std::move(newData);
    capacity = newCapacity;

    return true;
}
