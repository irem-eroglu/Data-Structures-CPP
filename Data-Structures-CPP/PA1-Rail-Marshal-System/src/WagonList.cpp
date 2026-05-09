#include "WagonList.h"

#include <iostream>

WagonList::~WagonList() { clear(); }

void WagonList::clear()
{
    Wagon *p = front;
    if (p == nullptr){
        front= nullptr;
        rear = nullptr;
        totalWeight = 0;
        return;
    }
    while(p){
        Wagon *late = p->getNext();
        delete p;
        p = late;
    }
    front = nullptr;
    rear = nullptr;
    totalWeight = 0;
    // TODO: Delete all Wagon objects in this list and reset pointers.
}

WagonList::WagonList(WagonList &&other) noexcept
{
    front = other.front;
    rear = other.rear;
    totalWeight = other.getTotalWeight();

    other.front = nullptr;
    other.rear = nullptr;
    other.totalWeight = 0;
    // TODO: Implement move constructor.
    // Transfer ownership of 'other' list’s nodes into this list
    // and leave 'other' in an empty but valid state.
}

WagonList &WagonList::operator=(WagonList &&other) noexcept
{
    if (this != &other) {
        clear();

    front = other.front;
    rear = other.rear;
    totalWeight = other.getTotalWeight();

    other.front = nullptr;
    other.rear = nullptr;
    other.totalWeight = 0;
    }    
    
    // Operation version of the move constructor.
    // TODO: Implement it.
    return *this;
}

Wagon *WagonList::findById(int id)
{
    Wagon *p = front;
    if(front == nullptr){
        return nullptr;
    }
    while(p){
        if(p->getID() == id){
            return p;
        }
        p = p->getNext();
    }
    // TODO: Find and return the Wagon with given ID.
    // Return nullptr if not found.
    return nullptr;
}

void WagonList::addWagonToRear(Wagon *w)
{
    if(front == nullptr){
        w->setNext(nullptr);
        w->setPrev(nullptr);
        front = rear = w;
        totalWeight = w->getWeight();
        return;
    }
    rear->setNext(w);
    w->setPrev(rear);
    w->setNext(nullptr);
    rear = w;
    totalWeight += w->getWeight();
    return;
    // TODO: Add a Wagon to the rear (end) of the list.
    // This function does not respect the weight order
    // it inserts to end regardless of the weight
}

int WagonList::getTotalWeight() const { return totalWeight; }

bool WagonList::isEmpty() const
{
    if(front == nullptr && rear == nullptr){
        return true;
    }
    // TODO: Return true if the list has no wagons.
    return false;
}

void WagonList::insertSorted(Wagon *wagon)
{
    if(this->isEmpty()){
        front = rear = wagon;
        wagon->setNext(nullptr); 
        wagon->setPrev(nullptr);
        totalWeight = wagon->getWeight();
        return;
    }
    if(front->getWeight() <= wagon->getWeight()){
        wagon->setNext(front);
        wagon->setPrev(nullptr);
        front->setPrev(wagon);
        front = wagon;
        totalWeight += wagon->getWeight();
        return;
    }
    if(rear->getWeight() >= wagon->getWeight()){
        wagon->setPrev(rear);
        wagon->setNext(nullptr);
        rear->setNext(wagon);
        rear = wagon;
        totalWeight += wagon->getWeight();
        return;
    }
    Wagon *p= front;
    while(p != nullptr && p->getWeight() >= wagon->getWeight()){
        p = p->getNext();
    }
    wagon->setPrev(p->getPrev());
    wagon->setNext(p);
    p->getPrev()->setNext(wagon);
    p->setPrev(wagon);
    totalWeight += wagon->getWeight();
    return;
    // TODO: Insert wagon into this list in descending order of weight.
}

void WagonList::appendList(WagonList &&other)
{
    if(other.isEmpty()){
        return;
    }    
    if(this->isEmpty()){
        front = other.getFront();
        rear = other.getRear();
        totalWeight = other.getTotalWeight();
        other.front = other.rear = nullptr;
        other.totalWeight= 0;
        return;    
    }
    if(other.front->getWeight() > front->getWeight()){
        other.rear->setNext(front);
        front->setPrev(other.rear);
        front = other.front;
    }else{
        rear->setNext(other.front);
        other.front->setPrev(rear);
        rear = other.rear;
    }
    totalWeight += other.getTotalWeight();
    other.totalWeight= 0;
    other.front = other.rear = nullptr;    
   // TODO: Append another WagonList to this one (merge them).
   // Use move semantics to avoid deep copies. (Double && at the parameter already makes it so)
   // 'other' should end up empty after this operation
   // At merge lists (blocks) will be protected 
   // But the one with heavier wagon at the front will be before the other list
}

Wagon *WagonList::detachById(int id)
{
   if(isEmpty()){
        return nullptr;
    }
    Wagon *p = findById(id);
    if(p == nullptr){
        return nullptr;
    }

    if(p == front){
        if(p != rear){
            front = p->getNext();
            front->setPrev(nullptr);
        }else{
            front = rear = nullptr;
        }
    }else{
        if(p == rear){
            rear = p->getPrev();
            if (rear != nullptr) {
                rear->setNext(nullptr);
            }else{
                front = nullptr; }
        }else{
        p->getPrev()->setNext(p->getNext());
        p->getNext()->setPrev(p->getPrev());}
    }
    totalWeight -= p->getWeight();
    p->setNext(nullptr);
    p->setPrev(nullptr);  
    std::cout << "Wagon " << p->id << " detached from Wagon List. " << std::endl;
    // TODO: Remove a specific wagon (by ID) from this list and return it.
    // Use: std::cout << "Wagon " << toRemove->id << " detached from Wagon List. " << std::endl;
    // Return nullptr if wagon not found.
    return p;
}


WagonList WagonList::splitAtById(int id)
{
    WagonList newList; // return-by-value (will be moved automatically)
    Wagon *splitId = findById(id); 
    if(this->isEmpty()){
        return newList;
    }
    if(splitId == nullptr){
        return newList;
    }
    if(splitId == front){
        newList.front = front;
        newList.rear = rear;
        newList.totalWeight = totalWeight;
        front = rear = nullptr;
        totalWeight = 0;
        return newList; 
    }
    newList.totalWeight = 0;
    
    Wagon *p = splitId;
    
    while(p){
        newList.totalWeight += p->getWeight();
        if(p->getNext() == nullptr){
            newList.rear = p;
            break;
        }
        p = p->getNext();
    }

    rear = splitId->getPrev();
    if(rear != nullptr){
        rear->setNext(nullptr);
    }
    splitId->setPrev(nullptr);
    totalWeight -= newList.totalWeight; 
    
    newList.front = splitId;
    // TODO: Split this list into two lists at the wagon with given ID.
    // The wagon with 'id' becomes the start of the new list.
    // Return the new WagonList (move return).
    // If 'id' not found, return an empty list.

    return newList; // moved, not copied
}

// Print is already implemented
void WagonList::print() const
{

    std::cout << *this << std::endl;
    return;
}

// << operator is already implemented
std::ostream &operator<<(std::ostream &os, const WagonList &list)
{
    if (list.isEmpty())
        return os;

    Wagon *current = list.front;

    while (current)
    {
        os << "W" << current->getID() << "(" << current->getWeight() << "ton)";
        if (current->getNext())
            os << " - ";
        current = current->getNext();
    }
    return os;
}
