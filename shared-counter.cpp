#include <iostream>
#include <thread>
#include <mutex>

std::mutex lock;


class Counter { // Shared resource
    public:
        int c = 0;
        Counter() {}
        ~Counter() {}

        void increment() {
            this->c++;
        }

        int get() {
            return this->c;
        }
};

class MyThread { // Threads
    public:
        int id, amount;
        MyThread(int i, int m): id(i), amount(m) {}
        ~MyThread() {}

        void operator()(Counter &c) {
            for (int j = 0; j < this->amount; j++) {
                lock.lock();
                c.increment();
                printf("Thread-%i: %i\n", this->id,  c.get());
                lock.unlock();
            }
        }
};


int main()
{
    Counter myCounter;
    int nThreads, mByThread;
    printf("Dile the number of threads: ");
    scanf("%i", &nThreads);
    printf("\n");
    printf("Dile the increment amount per thread: ");
    scanf("%i", &mByThread);
    printf("\n");
    
    std::thread threads[nThreads];
    
    for (int i = 0; i < nThreads; i++) {
        threads[i] = std::thread(MyThread(i, mByThread), std::ref(myCounter));
    }
    for (int i = 0; i < nThreads; i++) {
        threads[i].join();
    }
    return 0;
}