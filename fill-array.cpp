#include <iostream>
#include <thread>
#include <mutex>
#include <vector>


std::mutex lock;

class Loader {
    public:
        int id, minIndex, maxIndex;

        Loader(int i, int min, int max): id(i), minIndex(min), maxIndex(max) {}
        ~Loader() {}

        void operator()(std::vector<int> &v) {
            for (int i = this->minIndex; i < this->maxIndex; i++) {
                lock.lock();
                v[i] = i;
                std::cout << "Loader-" << this->id << " loaded: " << i << "\n";
                lock.unlock();
            }
        } 
};


int main() {
    int arraySize, nThreads, threadRange;

    printf("Dile the array size: ");
    scanf("%i", &arraySize);
    printf("\n");
    printf("Dile the number of threads: ");
    scanf("%i", &nThreads);
    printf("\n");
    printf("Dile the range each thread will fill in: ");
    scanf("%i", &threadRange);

    std::vector<int> cells(arraySize, -1);
    std::vector<std::thread> threads;
    int start = 0, end = threadRange; 

    for (int i = 0; i < nThreads && start <= arraySize - 1; i++) {// i == 1, start == 3, end == 5, arraySize == 10
        if (end > arraySize - 1) {
            threads.push_back(std::thread(Loader(i, start, arraySize), std::ref(cells)));
            start = arraySize;
        } else {
            threads.push_back(std::thread(Loader(i, start, end), std::ref(cells)));
        }
        start += threadRange;
        end += threadRange;
    }
    for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }

    return 0;
}