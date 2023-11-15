#include <iostream>
#include "abstract.h"
using namespace std;
class MinHeapOperations: public MinHeap{
    vector<int> heapTree;
    void swap(int *a, int *b){
        int temp = *a;
        *a = *b;
        *b = temp;
    }
    void heapify(int n){
        int size = (int)heapTree.size(), smallest = n;
        if(n * 2 + 1 < size && heapTree[smallest] > heapTree[n * 2 + 1])
            smallest = n * 2 + 1;
        if(n * 2 + 2 < size && heapTree[smallest] > heapTree[n * 2 + 2])
            smallest = n * 2 + 2;
        if(n != smallest){
            swap(&heapTree[n], &heapTree[smallest]);
            heapify(smallest);
        }
    }
public:
    int removeMin(){
        if(heapTree.empty()){
            return -1;
        }
        int z = heapTree[0];
        swap(&heapTree[0], &heapTree[heapTree.size() - 1]);
        heapTree.pop_back();
        heapify( 0);
        return z;
    }
    int peek(){
        if(heapTree.empty()){
            return -1;
        }else{
            return heapTree[0];
        }
    }
    void deleteNode(int num){
        for(int i = (int)heapTree.size() - 1; i >= 0; i--){
            if(heapTree[i] == num){
                swap(&heapTree[i], &heapTree[heapTree.size() - 1]);
                heapTree.pop_back();
                for(;i >= 0; i--){
                    heapify(i);
                }
            }
        }
    }

    void insert(int new_num) {
        int size = heapTree.size();
        if (size == 0) {
            heapTree.push_back(new_num);
        } else {
            heapTree.push_back(new_num);
            for (int ctr = size / 2 - 1; ctr >= 0; ctr--) {
                heapify(ctr);
            }
        }
    }

    void printHeapTree() {
        for (int ctr : heapTree) {
            cout << ctr << " ";
        }
        cout << endl;
    }
};