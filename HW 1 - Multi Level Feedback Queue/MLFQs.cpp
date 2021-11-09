//
//  Created by Furkan Kerim YILDIRIM on 24.10.2021.
//  furkanyildirim 28138

#include <iostream>
#include "MLFQs.h"

MLFQs::MLFQs(int p, int q, int t): current(q-1), processCount(p), timeSlice(t), queueSize(q) {
// postcondition: constructs MLFQ class and sets its sizes
    
    files = new ifstream [processCount];
    data = new Queue [queueSize];
    count = 0;
}

MLFQs::~MLFQs(void){
// postcondition: destructs MLFQ class
    
    output.close();
    delete [] data;
    delete [] files;
}

void MLFQs::setConfiguration(string path){
// postcondition: opens folder and sets all file configurations

    // creates file for program output
    string fileName, line;
    output.open(path + "/output.txt");
    
    // opens all process files in the given path and creates processes node
    for (int i = 0; i < processCount; i++) {
        
        // open process file in the given process id and saves it to list
        fileName = path + "/p" + to_string(i+1) + ".txt";
        files[i].open(fileName.c_str());

        // creates a new process and adds it to the highest queue
        Process process = Process(fileName, i+1, current+1);
        data[current].enqueue(process);
    }
}

void MLFQs::timePeriot(){
// postcondition: all the processes are moved to the topmost queue
    
    // defines temp current queue level and process
    Process process;
    int temp = current;
    
    //all the processes are moved to the topmost queue
    do{
        
        // make sure that current queue level is not empty
        while (!(data[current].isEmpty())){
            
            // make sure that current queue level is not equal to topmost queue
            if (current != queueSize - 1) {
                
                // moves process to the topmost queue
                data[current].dequeue(process);
                process.qid = queueSize;
                data[queueSize-1].enqueue(process);
                
                // prints result
                output << "B, PC" << process.pid << ", Q" << process.qid << endl;
                
            } else{
                break;
            }
        };
        
        // decreases current queue level
        current = (current + queueSize - 1) % queueSize;
        
    } while(temp != current);
    
    // sets the current queue level and the number of transactions after T
    count = 0;
    current = queueSize - 1;
}

void MLFQs::operate(){
// postcondition: executes all processes on MLFQ class
    
    // defines temp file lines and process
    Process process;
    string line, next;
    
    // make sure that MLFQ includes any process
    while (processCount > 0) {
        
        // make sure that current queue level is not empty
        while (!(data[current].isEmpty())) {
            
            // make sure that time slicing
            if (count == timeSlice) {
                timePeriot();
            }
            
            // get process infos from MLFQ
            data[current].dequeue(process);
            
            // reads current line of process file
            getline(files[process.pid-1], line);
            
            // make sure the process is not finished
            if (line != "-") {
                
                // reads next line of process file
                streampos pos = files[process.pid-1].tellg();
                getline(files[process.pid-1], next);
                files[process.pid-1].seekg(pos);
            } else {
                next = "";
            }
            
            // make sure that current process is last process
            if (next == "-") {
                
                // executes the process and closes the process file
                processCount--;
                files[process.pid-1].close();
                
                // prints result
                output << "E, PC" << process.pid << ", QX" << endl;

            }
            
            // make sure that current process is not last process
            else if (line != "-") {
                
                // sets the process query level
                process.qid = process.qid > 1 ? process.qid - stoi(line) : process.qid;
                data[process.qid - 1].enqueue(process);
                
                // prints result
                output << line <<", PC" << process.pid << ", Q" << process.qid << endl;
            }
            
            // increases number of transactions after T
            count ++;
        }
        
        // decreases current queue level
        current = (current + queueSize - 1) % queueSize;
    }
}

Queue::Queue(void){
// postcondition: constructs empty process queue
    
    front = NULL;
    rear = NULL;
}

void Queue::enqueue(Process & process){
// postcondition: add process to end of queue
    
    if (isEmpty()) {
        front = new Process(
            process.name, process.pid, process.qid);
        
        rear = front;
    }
    
    else{
        rear->next = new Process(
            process.name, process.pid, process.qid);
        
        rear = rear->next;
    }
}

void Queue::dequeue(Process & process){
// postcondition: gets and pops process from head of queue
    
    if (isEmpty()) {
        return;
    }
    
    else{
        process = Process(front->name,front->pid,front->qid);
        Process * temp = front;
        front = front->next;
        delete temp;
    }
}

bool Queue::isEmpty(void) const{
// postcondition: make sure that the process queue is empty
    
    return (front == NULL);
}
 
void Queue::clear(void){
// postcondition: deletes all processes of queue
    
    Process temp;
    while(!isEmpty()){
        dequeue(temp);
    }
}
