#include "MissionStack.h"
#include <new>     // for std::nothrow

MissionStack::MissionStack()
    : data(nullptr),
      capacity(0),
      top(-1) {
    resize(4);
}

MissionStack::MissionStack(int initialCapacity)
    : data(nullptr),
      capacity(0),
      top(-1) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

MissionStack::~MissionStack() {
    delete[] data;
}

bool MissionStack::isEmpty() const {
    return top == -1;
}

int MissionStack::size() const {
    return top + 1;
}

bool MissionStack::push(const Request& req) {
    //Implement push function as explained in the PDF.
    if(top < capacity -1){
        top++;
        data[top] = req;
        return true;
    }else{
        resize(capacity*2);
        push(req);
        return true;
    }    
    return false;
}

bool MissionStack::pop(Request& outReq) {
    //Implement pop function as explained in the PDF.
    if(!isEmpty()){
        outReq = data[top];
        top--;
        return true;
    }
    return false;
}

bool MissionStack::peek(Request& outReq) const {
    //Implement peek function as explained in the PDF.
    if(isEmpty()){
        return false;
    }
    outReq = data[top];
    return true;
}

void MissionStack::clear() {
    top = -1;
}

bool MissionStack::resize(int newCapacity) {
    //Implement resize function as explained in the PDF. 
    Request* stackNew = new Request[newCapacity];
    for(int i = 0; i<capacity; i++){
        stackNew[i] = data[i];
    }
    delete[] data;
    data = stackNew;
    capacity = newCapacity;
    stackNew = NULL;
    return true;
}
