#include "ClassificationYard.h"
#include <iostream>

ClassificationYard::ClassificationYard() {}
ClassificationYard::~ClassificationYard() { clear(); }

WagonList &ClassificationYard::getBlockTrain(int destination, int cargoType)
{
    return blockTrains[destination][cargoType];
}

WagonList *ClassificationYard::getBlocksFor(Destination dest)
{
    return blockTrains[static_cast<int>(dest)];
}

// Inserts vagon to the corract place at the yard
void ClassificationYard::insertWagon(Wagon *w)
{
    if (!w)
        return;
    int dest = static_cast<int>(w->getDestination());
    int cargo = static_cast<int>(w->getCargoType());
    blockTrains[dest][cargo].insertSorted(w);
}

// Merges multiple blocks into a train while keeping blocks grouped
Train *ClassificationYard::assembleTrain(Destination dest, const std::string &trainName)
{
    WagonList *blocks = getBlocksFor(dest);

    bool checkWagons = false;
    for (int i = 0; i < NUM_CARGOTYPES_INT; ++i) {
        if (!blocks[i].isEmpty()) {
            checkWagons = true;
            break;
        }
    }
    if(!checkWagons){
        return nullptr;
    }
    Train *newTrain = new Train(trainName, dest);
    int weightsArray[30] = {0};
    bool statuArray[30] = {false}; 

    for(int i = 0; i < NUM_CARGOTYPES_INT; ++i){
      
        if (!blocks[i].isEmpty() && i != (int)(CargoType::HAZARDOUS)){
           Wagon *w = blocks[i].getFront();
           if(w){
            weightsArray[i] = w->getWeight();
           }else{
            weightsArray[i] = 0;
           }
           statuArray[i] = true; 
        }
    }

    for(int i = 0; i < NUM_CARGOTYPES_INT; ++i) { 
        int maxWeight = -1;
        int index = -1;
        for (int j = 0; j < NUM_CARGOTYPES_INT; ++j) {
            if (statuArray[j] && weightsArray[j] > maxWeight) { 
                maxWeight = weightsArray[j];
                index = j;
            }
        }
        if (index == -1) {
            break;
        }
            newTrain->appendWagonList(blocks[index]);
            statuArray[index] = false; 
        }

    int hazardousIndex = static_cast<int>(CargoType::HAZARDOUS);    
    if(!blocks[hazardousIndex].isEmpty()){
        Wagon *hazardousWagon = blocks[hazardousIndex].getFront();
        if(hazardousWagon != nullptr){
            int id = hazardousWagon->getID();
            Wagon *wagonToMove = blocks[hazardousIndex].detachById(id);
            if(wagonToMove != nullptr){
                newTrain->addWagonToRear(wagonToMove);    
            }
        }
    }
    return newTrain;
}
    // TODO: Collect wagons of the same destination and assemble them into a single Train.

    /**
     * - Blocks of the same cargo type must remain grouped together.
     * - These groups must be appended to the train in descending order
     *   based on their heaviest wagon.
     * - Hazardous cargo (e.g., OIL) must always be placed at the very end of the train,
     *   and only one hazardous block can be included per train.*/

bool ClassificationYard::isEmpty() const
{
    for(int i = 0; i <  NUM_DESTINATIONS_INT ; ++i){
        for(int j = 0; j< NUM_CARGOTYPES_INT; ++j){
            if(!blockTrains[i][j].isEmpty()){
                return false;
            }
        }
    }
    /** TODO: Check if the entire classification yard is empty.
     *
     * The yard is empty if every blockTrain list for all destination-cargo pairs is empty.
     */
    return true;
}

void ClassificationYard::clear()
{
     for(int i = 0; i <  NUM_DESTINATIONS_INT ; ++i){
        for(int j = 0; j< NUM_CARGOTYPES_INT; ++j){
            blockTrains[i][j].clear();
        }
    }
    /** TODO: Clear all wagons from the classification yard.
     *
     * Used when resetting or ending the simulation.
     */
}

// Print function is already implemented to keep output uniform
void ClassificationYard::print() const
{
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        auto dest = destinationToString(static_cast<Destination>(i));
        std::cout << "Destination " << dest << ":\n";
        for (int j = 0; j < static_cast<int>(CargoType::NUM_CARGOTYPES); ++j)
        {
            if (!blockTrains[i][j].isEmpty())
            {
                auto type = cargoTypeToString(static_cast<CargoType>(j));
                std::cout << "  CargoType " << type << ": ";
                blockTrains[i][j].print();
            }
        }
    }
}