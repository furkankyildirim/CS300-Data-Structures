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
    
    // Initializes main string and hash table
    string text;
    int pos = 0, notFound = -1;
    HashTable<int,string> table(notFound, 4096);

    // Inserts all ASCII characters to hash table
    for (int i = 0; i < 256; i++) {
        table.insert(pos++,string(1,char(i)));
    }
    
    // Defines char and file names
    char ch;
    ifstream input("compin");
    ofstream output("compout");
    
    // Inserts all text to main string
    while (!input.eof()) {
        input.get(ch);
        text += ch;
    }
    
    // Initializes controllers
    int index = 0, step = 1;

    // Makes sure characters of main string is compressed
    while (index + step <= text.size()) {

        // If the text to be compressed is at the end of the main string
        if (index + step == text.size()) {

            // Finds and prints the value from the hash table
            output << table.find(text.substr(index, step -1)) << " ";
            step++;
        }
    
        // If the text to be compressed is inside the hash table
        else if(table.find(text.substr(index, step)) > 0){
            step++;
        }
        
        // If the text to be compressed is not inside the hash table
        else{
            
            // Finds and prints the value from the hash table
            output << table.find(text.substr(index, step -1)) << " ";

            // Compresses the new string
            table.insert(pos++,text.substr(index, step));
            index += (step - 1);
            step = 1;
        }
    }
    
    return 0;
}
