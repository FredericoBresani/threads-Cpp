#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <stack>
#include <memory>
#include "buffer.h"

std::condition_variable cLock;
std::mutex mutex;
Buffer *buffer = new Buffer();


class Producer {
    public:
        int id, amount;

        Producer(int i, int m): id(i), amount(m) {}
        ~Producer() {}

        void operator()() {
            for (int i = 0; i < this->amount; i++) {
                std::unique_lock<std::mutex> uLock(mutex);
                cLock.wait(uLock, [] { return buffer->buffer.size() < buffer->capacity; });
                buffer->produce(this->id, i);
                uLock.unlock();
                cLock.notify_all();
            }
        }
};

class Consumer {
    public:
        int id;

        Consumer(int i): id(i) {}
        ~Consumer() {}

        void operator()(int totalConsumption) {
            while(true) {
                if (buffer->consumedControl >= totalConsumption) delete this;
                std::unique_lock<std::mutex> uLock(mutex);
                cLock.wait(uLock, [] { return !buffer->buffer.empty(); });
                buffer->consume(this->id);
                uLock.unlock();
                cLock.notify_all();
            }
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

    buffer->setCapacity(bufferSize);
    std::thread producers[nProducers];
    std::thread consumers[nConsumers];

    for (int i = 0; i < nProducers; i++) {
        producers[i] = std::thread(Producer(i, aProducers));
    }
    for (int i = 0; i < nConsumers; i++) {
        consumers[i] = std::thread(Consumer(i), nProducers*aProducers);
    }

    for (int i = 0; i < nProducers; i++) {
        producers[i].join();
    }
    for (int i = 0; i < nConsumers; i++) {
        consumers[i].join();
    }
    return 0;
}