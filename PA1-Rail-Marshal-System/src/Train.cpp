#include "Train.h"
#include <iostream>

Train::Train() : name(""), destination(Destination::OTHERS), totalWeight(0), nextLocomotive(nullptr) {}
Train::Train(const std::string &_name, Destination _dest) : name(_name), destination(_dest), totalWeight(0), nextLocomotive(nullptr) {}
Train::~Train() { clear(); }

// This function is given to you ready
void Train::appendWagonList(WagonList &wl)
{
    // Makes appendList use move semantics
    wagons.appendList(std::move(wl));
    totalWeight = wagons.getTotalWeight();
}

// This function is given to you ready
void Train::addWagonToRear(Wagon *w)
{
    wagons.addWagonToRear(w);
    totalWeight = wagons.getTotalWeight();
}

void Train::clear()
{ 
    wagons.clear();
    totalWeight = 0;
    nextLocomotive = nullptr;
    //TODO: Do the cleaning as necesssary
}

// This function is given to you ready
void Train::print() const
{
    std::cout << "Train " << name << " (" << totalWeight << " tons): ";
    std::cout << wagons << std::endl;
}

Train *Train::verifyCouplersAndSplit(int splitCounter)
{
    WagonList newList;
    Wagon *p = wagons.getRear();
    if (!p){
        return nullptr;
    } 
    
    int cumulative_weight = p->getWeight(); 
    p = p->getPrev(); 
    if (!p){
        return nullptr; 
    } 

    while (p)
    {
        if (cumulative_weight > p->getMaxCouplerLoad()) 
        {
            Wagon *split_start_wagon = p->getNext(); 
            if (!split_start_wagon){
                return nullptr;} 
            
            newList = wagons.splitAtById(split_start_wagon->getID()); 
            Train* newTrain = new Train(name + "_split_" + std::to_string(splitCounter), destination);
            
            newTrain->appendWagonList(newList); 
            newTrain->totalWeight = newTrain->wagons.getTotalWeight();
            
            totalWeight = wagons.getTotalWeight(); 
            
            std::cout << "Train " << name << " split due to coupler overload before Wagon " 
                      << split_start_wagon->getID() << std::endl; 
            std::cout << newTrain->getWagons() << std::endl;
            
            return newTrain;
        }
        cumulative_weight += p->getWeight();
        p = p->getPrev();
    } 
    return nullptr;
}
    // TODO: Verify whether any wagon’s coupler is overloaded.
    // You must traverse from the rear (backmost wagon) toward the front.
    //
    // Splitting rule:
    // Split the train AFTER the overloaded wagon (the overloaded one stays).
    // Use WagonList::splitAtById to detach the overloaded segment into a new WagonList.
    //
    // If no overload occurs, return nullptr (no split needed).
    //
    // If a valid split occurs:
    // new Train should be named "<oldName>_split_<splitCounter>".

    // print message
    // std::cout << "Train " << name << " split due to coupler overload before Wagon " << splitId << std::endl;
    // std::cout << newTrain->wagons << std::endl;