#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <stack>
#include <memory>

std::condition_variable cLock;
std::mutex lock;

class Buffer {
    public:
        int capacity;
        std::stack<int> buffer;

        Buffer() {}
        Buffer(int c): capacity(c) {}
        ~Buffer() {} 

        void produce(int n) {
            this->buffer.push(n);
        }

        int consume() {
            int top = this->buffer.top();
            this->buffer.pop();
            return top;
        }

        void setCapacity(int c) {
            this->capacity = c; 
        }
};

class Producer {
    public:
        int id, amount;

        Producer(int i, int m): id(i), amount(m) {}
        Producer(Producer&& other) noexcept : id(std::move(other.id)), amount(other.id) {
            other.id = 0;
        }

        void operator()(Buffer& b) {
            for (int i = 0; i < this->amount; i++) {
                b.produce(1);
            }
        }
};

class Consumer {
    public:
        int id;

        Consumer(int i): id(i) {}
        ~Consumer() {}

        void operator()(Buffer &b) {
            int v = b.consume();
        }
};

int main() {
    int nConsumers, nProducers, aProducers, bufferSize;

    printf("Dile the number of consumers: ");
    scanf("%i", &nConsumers);
    printf("\n");
    printf("Dile the number of producers: ");
    scanf("%i", &nProducers);
    printf("\n");
    printf("Dile how much each producer will produce: ");
    scanf("%i", &aProducers);
    printf("\n");
    printf("Dile the buffer size: ");
    scanf("%i", &bufferSize);

    Buffer buffer;
    buffer.setCapacity(bufferSize);
    std::thread producers[nProducers];
    std::thread consumers[nConsumers];

    for (int i = 0; i < nProducers; i++) {
        producers[i] = std::thread(Producer(i, aProducers), std::ref(buffer));
    }
    //for (int i = 0; i < nConsumers; i++) {
    //    consumers[i] = std::thread(Consumer(i), buffer);
    //}

    for (int i = 0; i < nProducers; i++) {
        producers[i].join();
    }
    for (int i = 0; i < nConsumers; i++) {
        consumers[i].join();
    }
    return 0;
}