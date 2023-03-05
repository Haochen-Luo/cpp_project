#include <iostream>
#include <vector>

template<typename KEY, typename VALUE>
class MyHashMap{
public:
    MyHashMap() : buckets(1024) {}

    // Use chaining approach to handle hash collision
    void put(KEY key, VALUE value) {
        int index = hash(key);
        if (buckets[index].empty()) {
            buckets[index][0] = {key, value};
        } else {
            // if it is already there, update it
            for (auto& [k, v] : buckets[index]) {
                if (k == key) {
                    v = value;
                    return;
                }
            }
            // otherwise add it
            buckets[index].push_back({key, value});
        }
    }
    
    VALUE& operator[](KEY key) {
        int index = hash(key);
        for (auto& [k, v] : buckets[index]) {
            if (k == key) {
                return v;
            }
        }
        buckets[index].push_back({key, VALUE()});
        return buckets[index].back().second;
    }

private:
    std::vector<std::vector<std::pair<KEY, VALUE>>> buckets;
    
    int hash(const int key) {
        return key % buckets.size();
    }
    
    int hash(const std::string& key) {
        int hashVal = 0;
        for (char c : key) {
            hashVal = hashVal  + c;
        }
        // std::cout<<key<<hashVal % buckets.size()<<std::endl;
        return hashVal % buckets.size();
    }
};

int main()
{
    MyHashMap<int, int> intHash;
    MyHashMap<std::string, std::string> strHash;
    
    intHash[1] = 11;
    intHash[2] = 12;
    intHash[3] = 13;
    // Add one case to demonstrate hash collision
    // 1025%1024 = 1%1000
    intHash[1025] = 14;
    std::cout << intHash[1] << " " << intHash[2] << " " << intHash[3] << " " << intHash[1025] << std::endl;
    
    strHash["abc"] = "cde";
    strHash["bcd"] = "fgh";
    strHash["cde"] = "ijk";
    // Add one case to demonstrate hash collision
    // bbb has the same hashval 294 with abc
    strHash["bbb"] = "lmn";
    std::cout << strHash["abc"] << " " << strHash["bcd"] << " " << strHash["cde"] << " " << strHash["bbb"] << std::endl;
    
    return 0;
}
