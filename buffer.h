#ifndef __BUFFER__
#define __BUFFER__

#include <iostream>
#include <stack>


class Buffer {
    public:
        int capacity, consumedControl;
        std::stack<int> buffer;

        Buffer() {
            this->capacity = 0;
            this->consumedControl = 0;
        }
        Buffer(int c): capacity(c) {}
        ~Buffer() {} 

        void produce(int id, int n) {
            this->buffer.push(n);
            printf("Producer-%i produced: %i\n", id, n);
        }

        int consume(int id) {
            int top = this->buffer.top();
            this->buffer.pop();
            this->consumedControl++;
            printf("Consumer-%i consumed: %i\n", id, top);
            return top;
        }

        void setCapacity(int c) {
            this->capacity = c; 
        }
};


#endif