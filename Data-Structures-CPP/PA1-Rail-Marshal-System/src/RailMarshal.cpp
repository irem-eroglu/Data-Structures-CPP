#include "RailMarshal.h"
#include <iostream>
#include <sstream>
#include <algorithm>

RailMarshal::RailMarshal()
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i){
        Destination dest = static_cast<Destination>(i);
        departureYard[i] = TrainTrack(dest);
    }
    // TODO: Initialize each track in the departure yard.
    // Each TrainTrack corresponds to one Destination.
}

RailMarshal::~RailMarshal()
{
    classificationYard.clear();

    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i){
        while (!departureYard[i].isEmpty())
        {
            Train* t = departureYard[i].departTrain();
            delete t;
        }
    }
    // TODO: Cleanup remaining trains, prevent memory leaks
}

// Getter (ready)
ClassificationYard &RailMarshal::getClassificationYard()
{
    return classificationYard;
}

// Getter (ready)
TrainTrack &RailMarshal::getDepartureYard(Destination dest)
{
    int idx = static_cast<int>(dest);
    return departureYard[idx];
}

void RailMarshal::processCommand(const std::string &line)
{
    // TODO: Parse user commands from input lines.
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;
    cmd = toUpper(cmd);

    // if ADD_WAGON
    // Use: std::cout << "Error: Invalid ADD_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << *w << " added to yard." << std::endl;
    if(cmd == "ADD_WAGON"){
        int id, weight, maxCouplerLoad;
        std::string cargoStr, destStr;
        
        if (!(ss >> id >> cargoStr >> destStr >> weight >> maxCouplerLoad)) {
            std::cout << "Error: Invalid ADD_WAGON parameters.\n";
            return;
        }

        CargoType cargotype = parseCargo(cargoStr);
        Destination dest = parseDestination(destStr);
        
        Wagon *w = new Wagon(id, cargotype, dest, weight, maxCouplerLoad);
        classificationYard.insertWagon(w);
        std::cout << "Wagon " << *w << " added to yard." << std::endl;
    }
    // if REMOVE_WAGON
    // Use: std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << id << " removed." << std::endl;
    // Use: std::cout << "Error: Wagon " << id << " not found." << std::endl;
    else if(cmd == "REMOVE_WAGON"){
        int id;
        if(!(ss >> id)){
            std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
            return;
        }
        Wagon *wagonRemove = nullptr;
        bool foundWagon = false;

        for(int i = 0; i < NUM_DESTINATIONS_INT && !foundWagon ; i++){
            for( int j = 0; j < NUM_CARGOTYPES_INT; j++){
                WagonList &block = classificationYard.getBlockTrain(i, j);
                wagonRemove = block.detachById(id);
                if(wagonRemove != nullptr) {
                    foundWagon = true;
                    break;
                }
            }
        }
        if(foundWagon){
            std::cout << "Wagon " << id << " removed." << std::endl;
            delete wagonRemove;
        }else{
            std::cout << "Error: Wagon " << id << " not found." << std::endl;
        }
    }
    // if ASSEMBLE_TRAIN
    //  Use: std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
    //  Use: std::cout << "No wagons to assemble for " << destStr << std::endl;
    //  verify couplers and possibly split (deterministic)
    //  Keep splitting the *front* train until no more overloaded couplers found
    //  create new train with same destination and name suffix
    //  use std::cout << "Train " << newTrain->getName() << " assembled after split with "
    //  << newTrain->getWagons()<< " wagons." << std::endl;
    // use std::cout << "Train " << t->getName() << " assembled with " << t->getWagons() << " wagons." << std::endl;
    else if(cmd == "ASSEMBLE_TRAIN"){
        std::string destStr;
        if(!(ss>>destStr)){
            std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
            return;
        }
        Destination dest = parseDestination(destStr);
        TrainTrack &track = getDepartureYard(dest);
        std::string trainName = track.generateTrainName();
        
        Train *t = classificationYard.assembleTrain(dest, trainName);

        if(t == nullptr){
            std::cout << "No wagons to assemble for " << destStr << std::endl;
            return;
        }
        
        Train *currentTrain = t;
        int splitCounter = 1;
        
        while(true){
            Train *newTrain = currentTrain->verifyCouplersAndSplit(splitCounter);
            if (newTrain != nullptr){
                track.addTrain(newTrain);  
                std::cout << "Train " << newTrain->getName() << " assembled after split with "
                     << newTrain->getWagons() << " wagons." << std::endl;
                splitCounter++;
            }else{
                break;
            }
        }
        track.addTrain(t);
        std::cout << "Train " << t->getName() << " assembled with " << t->getWagons() << " wagons." << std::endl;
     
    }

    // if DISPATCH_TRAIN
    //  use: std::cout << "Error: Invalid DISPATCH parameters.\n";
    //  use: std::cout << "Error: No trains to dispatch from track " << destStr << ".\n";
    //  use:  std::cout << "Dispatching " << train->getName() << " (" << t->getTotalWeight() << " tons)." << std::endl;
    else if(cmd == "DISPATCH_TRAIN"){
        std::string destStr;
        if(!(ss >> destStr)){
            std::cout << "Error: Invalid DISPATCH parameters.\n";
            return;
        }

        Destination dest = parseDestination(destStr);
        TrainTrack &track = getDepartureYard(dest);

        Train *train = track.departTrain();
        if(train == nullptr){
            std::cout << "Error: No trains to dispatch from track " << destStr << ".\n";
            return;
        }
        std::cout << "Dispatching " << train->getName() << " (" << train->getTotalWeight() << " tons)." << std::endl;
        delete train;
    }    
    // if PRINT_YARD
    //  use std::cout << "--- classification Yard ---\n";
    else if(cmd == "PRINT_YARD"){
        std::cout << "--- classification Yard ---\n";
        classificationYard.print();
    }
    // if PRINT_TRACK
    //  use std::cout << "Error: Invalid PRINT_TRACK parameters.\n";
    else if(cmd == "PRINT_TRACK"){
        std::string destStr;
        if(!(ss>>destStr)){
            std::cout << "Error: Invalid PRINT_TRACK parameters.\n";
            return;
        }
        Destination dest = parseDestination(destStr);
        TrainTrack &track = getDepartureYard(dest);
        track.printTrack();
    }
    // if AUTO_DISPATCH <ON/OFF>
    // Enable or disable automatic dispatch when weight exceeds limits.
    // std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
    // print "Auto dispatch "enabled" / "disabled"
    else if(cmd == "AUTO_DISPATCH"){
        std::string on_off;
        if(!(ss>> on_off)){
            std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
            return;
        }
        
        if(on_off == "ON"){
            TrainTrack::autoDispatch = true;
            std::cout << "Auto dispatch enabled\n";
        }else if(on_off == "OFF"){
            TrainTrack::autoDispatch = false;
        std::cout << "Auto dispatch disabled\n";
        }else{
            std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
        }
    }
    
    // if CLEAR
    // Completely reset the system (yard + departure tracks).
    // std::cout << "System cleared." << std::endl;
    else if(cmd == "CLEAR"){
        RailMarshal::~RailMarshal();
        std::cout << "System cleared." << std::endl;
    }
    
    // else std::cout << "Error: Unknown command '" << cmd << "'" << std::endl;
    else{
        std::cout << "Error: Unknown command '" << cmd << "'" << std::endl;
    }
}

void RailMarshal::dispatchFromTrack(Destination track)
{
    TrainTrack &track1 = getDepartureYard(track);
    Train *train = track1.departTrain();
    int destIndex = static_cast<int>(track);
    if(train == nullptr){
        std::cout << "Error: No trains to dispatch from Track " << destIndex << ".\n";
        return;
    }
    std::cout << "Train " << train->getName()
              << " departed from Track " << destIndex
              << " (" << destinationToString(static_cast<Destination>(track)) << ").\n";
    delete train;
    // TODO: Dispatch the next train (frontmost) from the specified track.
    // std::cout << "Error: No trains to dispatch from Track " << destIndex << ".\n";
    /*std::cout << "Train " << t->getName()
              << " departed from Track " << destIndex
              << " (" << destinationToString(static_cast<Destination>(destIndex)) << ").\n";
     */
}

void RailMarshal::printDepartureYard() const
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        std::cout << "Track " << i << " ("
                  << destinationToString(static_cast<Destination>(i)) << "):\n";
        departureYard[i].printTrack();
    }
}

// Debug helper functions
void RailMarshal::printStatus() const
{
    std::cout << "--- classification Yard ---\n";
    classificationYard.print();

    std::cout << "--- Departure Yard ---\n";
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        departureYard[i].printTrack();
    }
}
