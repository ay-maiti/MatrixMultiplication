#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

void getSimpleProduct(vector<vector<int>>& A, vector<vector<int>>& B, vector<vector<unsigned long long int>>& C, int p, int q, int r, int start, int end){
    for(int i=0; i<p; i++){
        for(int j=0; j<r; j++){
            unsigned long long int val = 0;
            for(int k=start; k<end; k++){
                val += (A[i][k]*B[k][j]);
            }
            C[i][j] += val;
        }
    }
}

void getThreadedProduct(vector<vector<int>>& A, vector<vector<int>>& B, vector<vector<unsigned long long int>>& C, int p, int q, int r, int numThreads){
    vector<vector<vector<unsigned long long int>>> results(4, vector<vector<unsigned long long int>>());
    vector<thread> threads(numThreads);
    int chunkSize = max(1, (int)q/numThreads);
    for(int i=0; i<numThreads; i++){
        int start = i*chunkSize;
        int end = (i == numThreads-1) ? q : start + chunkSize;
        threads[i] = thread(getSimpleProduct, ref(A), ref(B), ref(C), p, q, r, start, end);
    }
    for(auto& th: threads){
        th.join();
    }
}

int main(){
    int numThreads = std::thread::hardware_concurrency();
    int p = 50;
    int q = 30;
    int r = 40;
    vector<vector<int>> A(p, vector<int>(q, 5));
    vector<vector<int>> B(q, vector<int>(r, 2));
    vector<vector<unsigned long long int>> C_nonthread(p, vector<unsigned long long int>(r, 0));
    vector<vector<unsigned long long int>> C_thread(p, vector<unsigned long long int>(r, 0));
    
    auto start_time = chrono::high_resolution_clock::now();
    getSimpleProduct(A, B, C_nonthread, p, q, r, 0, q);
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> time_taken = end_time - start_time;
    cout<<"result size (non-threaded): "<<C_nonthread.size()<<" "<<C_nonthread[0].size()<<endl;
    cout<<"non threaded time (seconds): "<<time_taken.count()<<endl;
    
    start_time = chrono::high_resolution_clock::now();
    getThreadedProduct(A, B, C_thread, p, q, r, numThreads);
    end_time = chrono::high_resolution_clock::now();
    time_taken = end_time - start_time;
    cout<<"result size (threaded): "<<C_thread.size()<<" "<<C_thread[0].size()<<endl;
    cout<<"threaded time (seconds): "<<time_taken.count()<<endl;
    
    // for(int i=0; i<C.size(); i++){
    //     for(int j=0; j<C[0].size(); j++){
    //         std::cout<<C[i][j]<<" ";
    //     }
    //     std::cout<<"\n";
    // }
    // cout<<"result size: "<<C.size()<<" "<<C[0].size()<<endl;
    return 0;
}