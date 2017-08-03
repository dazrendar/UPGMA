#include "dynmatrix.h"
#include <iostream>

//--___--!

void addCluster(ClusterNode *&head,ClusterNode *&tail,const std::string& name)
// adds a cluster (at the tail) and the corresponding row and column to data structure
// distance of all added DistanceNodes should be initialized to 0.0
// at the end, tail should point to the newly added ClusterNode
{
    ClusterNode * newCluster = new ClusterNode;
    newCluster->name = name;
    
    int counter = 0;
    
    
    if (head == NULL && tail == NULL) {
        
        newCluster->prev = NULL;
        newCluster->next = NULL;
        
        DistanceNode * newDistance = new DistanceNode;
        
        newDistance->distance = counter;
        newDistance->nextInRow = NULL;
        newDistance->nextInColumn = NULL;
        
        newCluster->row = newDistance;
        newCluster->column = newDistance;
        
        head = newCluster;
        tail = newCluster;
        
    }
    
    else {
        
        newCluster->prev = tail;
        newCluster->next = NULL;
        tail->next = newCluster;
        
        DistanceNode * accumulatorRow = head->row;  //just need to initialize the accumulators here;
        DistanceNode * accumulatorCol = head->column;
        bool flagA = false;
        bool flagB = false;
        
        
        
        for (DistanceNode * curr = tail->row; curr != NULL; curr = curr->nextInRow){
            
            DistanceNode * newDistance = new DistanceNode;
            
            newDistance->distance = counter;
            newDistance->nextInRow = NULL;
            newDistance->nextInColumn = NULL;
            
            // link CLUSTERnode to left of newDistance
            if (!flagA) {
                newCluster->row = newDistance;
                flagA = true;
            }
            
            
            ///ELSE, link accumulator to left of newDistance
            else {
                accumulatorRow->nextInRow = newDistance;
            }
            
            //set accumulator
            accumulatorRow = newDistance;
            
            // now link curr to top of newDistance
            curr->nextInColumn = newDistance;
        }
        
        for (DistanceNode * curr = tail->column; curr != NULL; curr = curr->nextInColumn){
            //construct new Distance node
            DistanceNode * newDistance = new DistanceNode;
            
            newDistance->distance = counter;
            newDistance->nextInRow = NULL;
            newDistance->nextInColumn = NULL;
            
            // link CLUSTERnode to left of newDistance
            if (!flagB) {
                newCluster->column = newDistance;
                flagB = true;
            }
            
            
            ///ELSE, link accumulator to top of newDistance
            else {
                accumulatorCol->nextInColumn = newDistance;
            }
            
            
            //set accumulator
            accumulatorCol = newDistance;
            
            // now link curr to left of newDistance
            curr->nextInRow = newDistance;
            
            
        }
        accumulatorCol = NULL;
        accumulatorRow = NULL;
        
        delete accumulatorRow;
        delete accumulatorCol;
        
        tail = newCluster;
        
        
    }
    
    
    
    
}

void printMatrix(ClusterNode *&head,ClusterNode *&tail) {
    // Prints the Node matrix
    
    
    std::cout << "======= MATRIX RELOADED =======" << std::endl;
    for (ClusterNode * curr = head; curr != NULL; curr = curr->next) {
        std::cout << curr->name << " ";
    }
    std::cout << std::endl;
    ClusterNode * curr = head;
    
    for (DistanceNode * curr1 = curr->column; curr1 != NULL; curr1 = curr1->nextInColumn) {
        for (DistanceNode * curr2 = curr1; curr2 != NULL; curr2 = curr2->nextInRow) {
            std::cout << curr2->distance << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "INVERSE" << std::endl;
    for (DistanceNode * curr1 = curr->row; curr1 != NULL; curr1 = curr1->nextInRow) {
        for (DistanceNode * curr2 = curr1; curr2 != NULL; curr2 = curr2->nextInColumn) {
            std::cout << curr2->distance << " ";
        }
        std::cout << std::endl;
    }
    
    
    
    std::cout << std::endl;
    
    std::cout << "TAIL IS: " << tail->name << std::endl;
    std::cout << "HEAD IS: " << head->name << std::endl;
    
    std::cout << "======= =============== =======" << std::endl;
}


void removeCluster(ClusterNode *&head,ClusterNode *&tail,ClusterNode *toBeRemoved)
// removes a cluster pointed to by toBeRemoved and the corresponding row and column
// if toBeRemoved is the first or last cluster then head or tail needs to be updated
{
    if (toBeRemoved==head) {
        bool flag = false;
        
        for (ClusterNode * curr = head; curr != NULL; curr = curr->next) {
            DistanceNode * tempDistRow = curr->row;
            DistanceNode * tempDistCol = curr->column;
            if (!flag) {
                delete tempDistRow;
                
                flag = true;
            }
            else {
                curr->row = tempDistRow->nextInRow;
                delete tempDistRow;
                
                curr->column = tempDistCol->nextInColumn;
                delete tempDistCol;
                
            }
        }
        
        ClusterNode * tempCluster = head;
        head = tempCluster->next;
        if (head) head->prev = NULL;
        delete tempCluster;    
    }
    
    
    else if (head) {
        
        for (DistanceNode * curr = toBeRemoved->prev->column; curr != NULL; curr = curr->nextInColumn) {
            DistanceNode * tempMiddleNode = curr->nextInRow;
            
            if (curr->nextInRow->nextInRow) { // if there will be a gap in COLUMN
                curr->nextInRow = curr->nextInRow->nextInRow;
                tempMiddleNode->nextInRow = NULL;
                tempMiddleNode->nextInColumn = NULL;
                delete tempMiddleNode;
            }
            else {
                curr->nextInRow = NULL;
                tempMiddleNode->nextInRow = NULL;
                tempMiddleNode->nextInColumn = NULL;
                delete tempMiddleNode;
            }
        }
        
        
        
        for (DistanceNode * curr = toBeRemoved->prev->row; curr != NULL; curr = curr->nextInRow) {
            DistanceNode * tempMiddleNode = curr->nextInColumn;
            if (curr->nextInColumn->nextInColumn) { // if there will be a gap in ROW
                curr->nextInColumn = curr->nextInColumn->nextInColumn;
                tempMiddleNode->nextInRow = NULL;
                tempMiddleNode->nextInColumn = NULL;
                delete tempMiddleNode;
                
            }
            else {
                curr->nextInColumn = NULL;
                tempMiddleNode->nextInRow = NULL;
                tempMiddleNode->nextInColumn = NULL;
                delete tempMiddleNode;
            }
            
        }
        
        if (toBeRemoved == tail) {  //finally delete cluster node and update tail
            tail = tail->prev;
            tail->next = NULL;
            toBeRemoved->column = NULL;
            toBeRemoved->row = NULL;
            delete toBeRemoved;
        }
        else {
            ClusterNode * left = toBeRemoved->prev;
            ClusterNode * right = toBeRemoved->next;
            left->next = right;
            right->prev = left;
            toBeRemoved->column = NULL;
            toBeRemoved->row = NULL;
            delete toBeRemoved;
            
        }
        
        
    }
}












void findMinimum(ClusterNode *head,ClusterNode *&C,ClusterNode *&D)
// finds the minimum distance (between two different clusters) in the data structure
// and returns the two clusters via C and D
{
    double min = 0.0;
    bool flagCurrentMinNotSet = true; // true if current min not yet set
    for (ClusterNode * currentCluster = head; currentCluster != NULL; currentCluster = currentCluster->next) {
        
        for (DistanceNode * currentDNode = currentCluster->row; currentDNode != NULL; currentDNode = currentDNode->nextInRow) { // for all elements in the row...
            
            if (flagCurrentMinNotSet && currentDNode->distance > 0) {
                min = currentDNode->distance;
                flagCurrentMinNotSet = false;
            }
            else if (currentDNode->distance > 0 && currentDNode->distance < min) {
                min = currentDNode->distance;
            }
        }
    }
    
    
    
    bool flag = true;  //true if C is empty
    int globalCountX = 0;
    int globalCountY = 0;
    
    for (ClusterNode * currentCluster = head; currentCluster != NULL && flag; currentCluster = currentCluster->next) {
        globalCountX++;
        int tempCount = 0;
        for (DistanceNode * currentDNode = currentCluster->column; currentDNode != NULL; currentDNode = currentDNode->nextInColumn) { // for all elements in the column...
            tempCount++;
            if (currentDNode->distance == min) {
                if (flag) {
                    C = currentCluster;
                    globalCountY = tempCount;
                    flag = false;  //to exit this loop
                }
            }
        }
    }
    
    int globalCountBX = 0;
    
    int globalCountBY = 0;
    for (ClusterNode * currentCluster = head; currentCluster != NULL; currentCluster = currentCluster->next) {
        globalCountBX++;
        if (globalCountBX == globalCountY) {
            for (DistanceNode * currentDNode = currentCluster->column; currentDNode != NULL; currentDNode = currentDNode->nextInColumn) { // for all elements in the row...
                globalCountBY++;
                if (currentDNode->distance == min && globalCountBY == globalCountX) {
                    D = currentCluster;
                    flag = false;
                    return;
                    
                }
            }
            
        }
        
        
    }
    
}




