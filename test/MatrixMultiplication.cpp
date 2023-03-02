#include "../lib/Matrix.h"
#include <string>
#include <chrono>

int main(){
  std::string str0 = "";
    for(long long int i = 1; i <= 1024*1024; i++){
        str0 += ' ';
        str0 += std::to_string(i);
    }
    matrix<int> mxt0(1024,1024);
    matrix<int> mxt1(1024,1024);
    mxt0.setMatrix(str0);
    mxt1.setMatrix(str0);
    mxt0.setOptimalParams();
    std::cout << "Using threads:"<< std::thread::hardware_concurrency() << '/' << mxt0.getNumberOfThreads() << std::endl;
    std::cout << "Chunk size:" << mxt0.getBlockSize() << std::endl;
    std::cout << "Start...\n";
    auto start_time = std::chrono::high_resolution_clock::now();

    auto newMtx = mxt0 * mxt1;

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
   // newMtx.display();
    std::cout << "\n";
    std::cout<<"Benchmark time: "<< static_cast<double>(duration.count())/1000 << " ms\n";

}