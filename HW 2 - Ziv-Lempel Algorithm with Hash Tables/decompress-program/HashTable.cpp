//
//  Created by Furkan K. Yıldırım on 11.11.2021.
//  furkanyildirim 28138

#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>
using namespace std;


template <class Key, class Value>
class HashTable{
public:
  
    HashTable(const Value &, int size = 256);                   //  Default constructor
    HashTable(const HashTable & rhs) : array(rhs.array) {}      //  Copy constructor

    
    void makeEmpty();                                           //  Delete all items from hash table
    void remove(const Key &);                                   //  Remove item x from the hash table
    void insert(const Key &, const Value &);                    //  Insert item x into the hash table
    
    Value find(const Key &);                                    //  Find item x in the hash table

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
    
    const Value ITEM_NOT_FOUND;

    vector<HashEntry> array;                                    //  Hash Table
    bool isActive(int) const;                                   //  Return true if currentPos exists and is active
    int findPos(const Key &) const;                              //  Return the position where the search for x terminates
};


template <class Key, class Value>
HashTable<Key, Value>::HashTable(const Value & notFound, int size) : ITEM_NOT_FOUND(notFound) , array(size){
// postcondition: Construct the hash table
    
        makeEmpty();
    }


template <class Key, class Value>
void HashTable<Key, Value>::makeEmpty(){
    
    for (int i = 0; i < array.size(); i++) {
        array[i].info = EMPTY;
    }
}

template <class Key, class Value>
int HashTable<Key, Value>::findPos(const Key & pos) const {
// postcondition: Return the position where the search for x terminates.
    
    int p = pos % array.size();
    
    while (array[p].info != EMPTY && array[p].code != pos) {
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
Value HashTable<Key, Value>::find(const Key & code) {
// postcondition: Find item x in the hash table.

    int pos = findPos(code);
    if (isActive(pos)) {
        return array[pos].value;
    }
    
    return ITEM_NOT_FOUND;
}


template <class Key, class Value>
void HashTable<Key, Value>::remove(const Key & code){
// postcondition: Remove item x from the hash table.

    int pos = findPos(code);
    
    if (isActive(pos)) {
        array[pos].info = DELETED;
    }
}


template <class Key, class Value>
void HashTable<Key, Value>:: insert(const Key & code, const Value & value) {
// postcondiiton: Insert item x into the hash table.
// If the item is already present, then do nothing.
    
    // Insert x as active
    int pos = findPos(code);

    if (isActive(pos)) {
        return;
    }

    array[pos] = HashEntry(code, value, ACTIVE);
}