//
//  Created by Furkan Kerim YILDIRIM on 24.10.2021.
//  furkanyildirim 28138

#ifndef MLFQs_H
#define MLFQs_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Process{
    string name                                         ;   //  indicates filename of process
    int pid, qid                                        ;   //  they indicate process id and queue level
    
    Process * next                                      ;   //  referances next process
    
    
    Process(){}                                         ;   //  default constructor
    Process(string n, int p, int q, Process *ptr = NULL)    //  valid constructor
    :name(n), pid(p), qid(q){
        next = ptr;
    }
};


class Queue{
public:
    Queue(void)                                     ;   //  constructs empty process queue
    void enqueue(Process & process)                 ;   //  add process to end of queue
    void dequeue(Process & process)                 ;   //  gets and pops process from head of queue
    
    bool isEmpty(void) const                        ;   //  make sure that the process queue is empty
    void clear(void)                                ;   //  deletes all processes of queue

private:
    Process * front                                 ;   //  the top process of process queue
    Process * rear                                  ;   //  the end process of process queue
};


class MLFQs{
public:
    MLFQs(void)                                     ;   //  constructs empty MLFQ class
    ~MLFQs(void)                                    ;   //  destructs MLFQ class
    MLFQs(int p, int q, int t)                      ;   //  constructs MLFQ class and sets its sizes
    
    void setConfiguration(string path)              ;   //  opens folder and sets all file configurations
    void timePeriot()                               ;   //  all the processes are moved to the topmost queue
    void operate()                                  ;   //  executes all processes on MLFQ class
    
private:
    Queue * data                                    ;   //  stores all queue levels (Q4, Q3, ...)
    ifstream * files                                ;   //  stores all process files (pid1.txt, pid2.txt, ...)
    ofstream output                                 ;   //  contains what has happened in each time slice
    
    int timeSlice, processCount                     ;   //  they indicate the number of time slices and total process count
    int queueSize;                                  ;   //  indicates total queue level count
    int count, current;                             ;   //  they indicate the current queue level and the number of transactions after T
};

#endif /* MLFQs_H */
