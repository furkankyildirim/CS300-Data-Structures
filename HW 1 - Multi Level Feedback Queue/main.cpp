//
//  Created by Furkan Kerim YILDIRIM on 24.10.2021.
//  furkanyildirim 28138

#include <iostream>
#include <fstream>
#include <sstream>
#include "MLFQs.h"

using namespace std;

int main() {

    // Define file variables
    string path, configName;
    ifstream file;
    
    
    // Get filename until open successfully
    do{
        cout << "Please enter the process folder name: ";
        cin >> path;

        configName = path + "/configuration.txt";
        file.clear();
        file.open(configName.c_str());
        
    } while(file.fail());
    

    // Defines number of queues, number of processes and size of S variables
    int queueSize, processCount, timeSlice;
    string line, a;
    

    // Gets values of number of queues, number of processes and size of S
    getline(file, line);
    queueSize = stoi(line);

    getline(file,line);
    processCount = stoi(line);

    getline(file,line);
    timeSlice = stoi(line);
    file.close();
    
    
    // Creates MLFQ and operate the processes
    MLFQs mlfqs(processCount, queueSize, timeSlice);
    mlfqs.setConfiguration(path);
    mlfqs.operate();
    
    return 0;
}
