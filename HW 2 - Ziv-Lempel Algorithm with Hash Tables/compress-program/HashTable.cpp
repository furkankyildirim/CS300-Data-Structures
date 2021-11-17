//
//  Created by Furkan K. Yıldırım on 11.11.2021.
//  furkanyildirim 28138

#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>
using namespace std;

template <class Value>
int hashFunc(Value & obj){
    
    // Initialize constants
    const int p = 31;
    const int m = 1e9 + 9;

    long long hash_value = 0;
    long long p_pow = 1;
    
    // Until all characters are hashed
    for (unsigned char c : obj) {
        hash_value = (hash_value + c * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }

    return hash_value;
}


template <class Key, class Value>
class HashTable{
public:
  
    HashTable(const Key &, int size = 256);                     //  Default constructor
    HashTable(const HashTable & rhs) : array(rhs.array) {}      //  Copy constructor

    
    void makeEmpty();                                           //  Delete all items from hash table
    void remove(const Value &);                                 //  Remove item x from the hash table
    void insert(const Key &, const Value &);                    //  Insert item x into the hash table
    
    Key find(const Value &);                                    //  Find item x in the hash table

    enum EntryType {ACTIVE, EMPTY, DELETED};
    const HashTable & operator=(const HashTable & rhs);         //  Copy constructor operator

private:
    struct HashEntry{
        Key code;                                               //  Store Key data
        Value value;                                            //  Store Value data
        EntryType info;                                         //  Store struct info

        HashEntry(Key c = Key(), const Value & x = Value(), EntryType i = EMPTY)
            : code(c), value(x), info(i){}
    };
    
    const Key ITEM_NOT_FOUND;

    vector<HashEntry> array;                                    //  Hash Table
    bool isActive(int) const;                                   //  Return true if currentPos exists and is active
    int findPos(const Value &) const;                           //  Return the position where the search for x terminates
};


template <class Key, class Value>
HashTable<Key, Value>::HashTable(const Key & notFound, int size ) : array(size), ITEM_NOT_FOUND(notFound){
// postcondition: Construct the hash table
    
        makeEmpty();
    }


template <class Key, class Value>
void HashTable<Key, Value>::makeEmpty(){
// postcondition: Delete all items from hash table
    
    for (int i = 0; i < array.size(); i++) {
        array[i].info = EMPTY;
    }
}


template <class Key, class Value>
int HashTable<Key, Value>::findPos(const Value & x) const {
// postcondition: Return the position where the search for x terminates.
    

    int p = hashFunc(x) % array.size();
    
    while (array[p].info != EMPTY && array[p].value != x) {
        p++;
    }
    
    return p;
}


template <class Key, class Value>
bool HashTable<Key, Value>::isActive(int pos) const {
// postcondition: Return true if currentPos exists and is active.
    
    return array[pos].info == ACTIVE;
}


template <class Key, class Value>
void HashTable<Key, Value>::remove(const Value & x){
// postcondition: Remove item x from the hash table.

    int pos = findPos(x);
    
    if (isActive(pos)) {
        array[pos].info = DELETED;
    }
}


template <class Key, class Value>
Key HashTable<Key, Value>::find(const Value & x) {
// postcondition: Find item x in the hash table.

    int pos = findPos(x);
    if (isActive(pos)) {
        return array[pos].code;
    }
    
    return ITEM_NOT_FOUND;
}


template <class Key, class Value>
void HashTable<Key, Value>::insert(const Key & code, const Value & x) {
// postcondiiton: Insert item x into the hash table.
// If the item is already present, then do nothing.
    
    // Insert x as active
    int pos = findPos(x);

    if (isActive(pos)) {
        return;
    }

    array[pos] = HashEntry(code, x, ACTIVE);
} 