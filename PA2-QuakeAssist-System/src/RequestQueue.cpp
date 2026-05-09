#include "RequestQueue.h"
#include <new> // for std::nothrow

RequestQueue::RequestQueue()
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    // start with a small default capacity
    resize(4);
}

RequestQueue::RequestQueue(int initialCapacity)
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

RequestQueue::~RequestQueue() {
    delete[] data;
}

bool RequestQueue::isEmpty() const {
    return count == 0;
}

bool RequestQueue::isFull() const {
    return count == capacity;
}

int RequestQueue::size() const {
    return count;
}

int RequestQueue::nextIndex(int idx) const {
    if (capacity == 0) return 0;
    return (idx + 1) % capacity;
}

bool RequestQueue::enqueue(const Request& req) {
    //Implement enqueue function as explained in the PDF.
    if(isEmpty()){
        data[rear] = req;
        front = rear;
        count++;
        return true;
    }
    if(isFull()){
        resize(2*capacity);
        enqueue(req);
        return true; 
    }
    rear = (rear + 1) % capacity;
    data[rear] = req;
    count++;
    return true;
    // (void)req; suppress unused warning until implemented
    //return false;
}

bool RequestQueue::dequeue(Request& outReq) {
    if(isEmpty()){
        return false;
    }else if(front == rear){
        outReq = data[front];
        front = rear = 0;
        count = 0;
        return true;
    }else{
        outReq = data[front];
        front = (front + 1) % capacity;
        count--;
        return true;
    }
    //Implement dequeue function as explained in the PDF.
    // (void)outReq; suppress unused warning until implemented
    return false;
}

bool RequestQueue::peek(Request& outReq) const {
    //Implement peek function as explained in the PDF.
    if(isEmpty()){
        return false;
    }
    outReq = data[front];
    return true;
}

void RequestQueue::clear() {
    front = 0;
    rear = 0;
    count = 0;
}

bool RequestQueue::removeById(const std::string& id) {
    //Implement removeById function as explained in the PDF.
    // (void)id; suppress unused warning until implemented
    if(isEmpty()){
        return false;
    }
    int currIdx = front;
    for(int i = 0; i< count; i++){
        if(data[currIdx].getId() == id){
            if(currIdx == front && currIdx == rear){
                front = rear = 0;
                count = 0;
                return true;
            }
            else if(currIdx == front){
                front = (front + 1) % capacity;
            }
            else if(currIdx == rear){
                rear = (rear - 1 + capacity) % capacity;
            }
            else{
                int index = currIdx;
                while(index != rear){
                    data[index] = data[(index + 1) % capacity];                         
                    index = (index + 1) % capacity;
                }
                rear = (rear - 1 + capacity) % capacity;
            }
            count--;
            return true;
        }
        currIdx = (currIdx + 1) % capacity;
    }
    return false;
}

bool RequestQueue::resize(int newCapacity) {
    //Implement resize function as explained in the PDF.
    Request* queueNew = new Request[newCapacity];
    for(int i = 0; i<count; i++){
        queueNew[i] = data[(front + i) % capacity];
    }
    delete[] data;
    data = queueNew;
    capacity = newCapacity;
    front = 0;
    if(count != 0){
        rear = count-1;
    }else{
        rear = 0;
    }
    queueNew = NULL;    
    return true;
}
