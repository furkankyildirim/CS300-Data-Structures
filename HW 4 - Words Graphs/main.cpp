//
//  Created by Furkan K. YILDIRIM on 23.12.2021.
//  furkanyildirim 28138

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
unordered_map<int, string> reverseHash;

bool isExist(unordered_map<string, int> hash, string word) {
    return hash.find(word) != hash.end();
}

void difference(string node, string edge) {
    if (node.length() == edge.length()) {
        for (int i = 0; i < node.length(); i++){
            if (node[i] != edge[i]) {
                cout << edge << " (change " << node[i] << " at position " << i + 1 << " to " << edge[i] << ")" << endl; break;
            }
        }
    } else if (node.length() > edge.length()) {
        for (int i = 0; i < node.length(); i++) {
            if (node[i] != edge[i]) {
                cout << edge << " (delete " << node[i] << " at position " << i + 1 << ")" << endl; break;
            }
        }
    } else {
        for (int i = 0; i < edge.length(); i++) {
            if (node[i] != edge[i]) {
                cout << edge << " (insert " << edge[i] << " after position " << i << ")" << endl; break;
            }
        }
    }
}


bool BFS(vector<vector<int>> adjList, int source, int target, int size, int sequence[], int distanceList[]){
    
    bool visitedNodes[size];
    vector<int> queue;
    
    for (int i = 0; i < size; i++) {
        visitedNodes[i] = false; distanceList[i] = INT_MAX;
        sequence[i] = -1;
    }
    
    distanceList[source] = 0; visitedNodes[source] = true;
    queue.push_back(source);
    
    while (queue.size() != 0) {
        int node = queue[0]; queue.erase(queue.begin());
        
        for (int edge = 0; edge < adjList[node].size(); edge++) {
            if (visitedNodes[adjList[node][edge]] == false) {
                visitedNodes[adjList[node][edge]] = true;
                distanceList[adjList[node][edge]] = distanceList[node] + 1;
                sequence[adjList[node][edge]] = node; queue.push_back(adjList[node][edge]);
 
                if (adjList[node][edge] == target) return true;
            }
        }
    }
    
    return false;
}

void findPath(vector<vector<int>> adjList, int source, int target, int size) {
    
    int sequence[size], dist[size];
    
    if (BFS(adjList, source, target, size, sequence, dist) == false) {
        cout << "There is no way to convert " << reverseHash[source];
        cout << " to " << reverseHash[target] << endl;
        return;
    }
    
    vector<int> path;
    int edge = target;

    path.push_back(edge);
    while (sequence[edge] != -1) {
        path.push_back(sequence[edge]);
        edge = sequence[edge];
    }
    
    long seq = path.size() - 1;
    cout << reverseHash[path[seq]] << endl;
    for (; seq > 0; seq--)
        difference(reverseHash[path[seq]], reverseHash[path[seq-1]]);
}

int main() {
    ifstream file("words.txt");
    string start, end, key, word;
    unordered_map<string, int> hash;
    
    int total = 0;
    
    if (file.fail()){
        cout << "The words.txt file was not found. Exiting." << endl;
        return 1;
    }
    while (file >> word){
        if (hash.find(word) == hash.end()){
            hash[word] = total;
            reverseHash[total++] = word;
        }
    }
    
    vector<vector<int>> adjList(total);
    for (auto i : hash) {
        for (int j = 0; j < i.first.length(); j++) {
            key = i.first; key.erase(j, 1); if (isExist(hash,key) && key != i.first) adjList[i.second].push_back(hash[key]);
            
            key = i.first; key[j] = char(39); if (isExist(hash,key) && key != i.first) adjList[i.second].push_back(hash[key]);
            key = i.first; key[j] = char(45); if (isExist(hash,key) && key != i.first) adjList[i.second].push_back(hash[key]);
            
            key = i.first; key.insert(j, 1, char(39)); if (isExist(hash,key) && key != i.first) adjList[i.second].push_back(hash[key]);
            key = i.first; key.insert(j, 1, char(45)); if (isExist(hash,key) && key != i.first) adjList[i.second].push_back(hash[key]);
            
            for (char k = 97; k < 123; k++) {
                key = i.first; key[j] = k; if (isExist(hash,key) && key != i.first) adjList[i.second].push_back(hash[key]);
                key = i.first; key.insert(j, 1, k); if (isExist(hash,key) && key != i.first) adjList[i.second].push_back(hash[key]);
            }
        }
        
        key = i.first; key.insert(key.length(), 1, char(39)); if (isExist(hash,key) && key != i.first) adjList[i.second].push_back(hash[key]);
        key = i.first; key.insert(key.length(), 1, char(45)); if (isExist(hash,key) && key != i.first) adjList[i.second].push_back(hash[key]);
        for (char k = 97; k < 123; k++) {
                key = i.first; key.insert(key.length(), 1, k); if (isExist(hash,key) && key != i.first) adjList[i.second].push_back(hash[key]);
        }
        
    }
    
    //    prints graph
    //    for (auto i : hash) {
    //        cout << "Node: " << i.first << "\nEdges: ";
    //        for (int j : adjList[i.second]) {
    //            cout << reverseHash[j] << ",";
    //        }
    //
    //        cout << endl << endl;
    //    }
    
    
    while (true) {
        cout << "Enter words: ";
        cin >> start >> end;
        if (start[0] == '*') break;
        
        if (hash.find(start) == hash.end()) cout << "First word is not in the words.txt" << endl;
        else if (hash.find(end) == hash.end()) cout << "Second word is not in the words.txt" << endl;
        else findPath(adjList, hash[start], hash[end], total);
    }
    return 0;
}


