//
//  Created by Furkan K. Yıldırım on 11.11.2021.
//  furkanyildirim 28138

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "HashTable.cpp"

using namespace std;

int main() {
    
    // Initializes code list and hash table
    int pos = 0; vector<int> list;
    string notFound = string(1,'\0');
    HashTable<int, string> table(notFound,4096);
    
    // Inserts all ASCII characters to hash table
    for (int i = 0; i < 256; i++) {
        table.insert(pos++,string(1, char(i)));
    }
    
    // Defines char and file names
    int code;
    ifstream input("compout");
    ofstream output("decompout");
    
    // Inserts all codes to list
    while (!input.eof()) {
        input >> code;
        list.push_back(code);
    }
    
    
    list.pop_back(); // Removes the latest code from the list as it adds it twice
    output << table.find(list[0]); // Prints the value of the first code
    
    // Makes sure all codes are decompressed
    for (int i = 1; i < list.size(); i++) {

        // If the code is not in the hash table
        if (list[i] < pos) {
            
            // Finds current and previous values from hash table
            string prevStr = table.find(list[i-1]);
            string currentStr = table.find(list[i]);
            
            //  Adds code that is not in the hash table to the hash table
            table.insert(pos++, prevStr + currentStr[0]);

            // Prints current value
            output << currentStr;

        }
        
        // If the code is not decompressed
        else {

            // Finds previous value
            string prevStr = table.find(list[i-1]);
            
            //  Adds code that is not in the hash table to the hash table
            table.insert(pos++, prevStr + prevStr[0]);

            // Prints prevStr and its first character
            output << prevStr + prevStr[0];
        }
    }

    return 0;
}
