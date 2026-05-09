#include "QuakeAssistController.h"
#include <iostream>
#include <sstream>

QuakeAssistController::QuakeAssistController()
    : teams(nullptr),
      teamCount(0),
      supplyQueue(4),
      rescueQueue(4) {
}

QuakeAssistController::~QuakeAssistController() {
    delete[] teams;
}

bool QuakeAssistController::parseAndExecute(const std::string& line) {
    //Read the input file line by line and execute realtime.
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if(cmd == "INIT_TEAMS"){
        int numTeams;
        if(!(ss>>numTeams)){
            std::cout << "Error: Invalid INIT_TEAMS parameters.\n";
            return true;
        }
        initializeTeams(numTeams);
        return true;

    }else if(cmd == "SET_TEAM_CAPACITY"){
        int teamID, maxLoadCapacity;
        if(!(ss>>teamID>>maxLoadCapacity)){
            std::cout << "Error: Invalid SET_TEAM_CAPACITY parameters.\n";
            return true;
        }
        bool result = handleSetTeamCapacity(teamID, maxLoadCapacity);
        if(result){            
            std::cout<<"Team "<<teamID<<" capacity set to "<<maxLoadCapacity<<"."<<std::endl;
        }else{
            std::cout << "Error: Team " << teamID << " not found.\n";
        }
        return true;

    }else if(cmd == "ADD_SUPPLY"){
        std::string id, city, supplyType; 
        int amount, emergencyLevel;
        if(!(ss>>id>>city>>supplyType>>amount>>emergencyLevel)){
            std::cout << "Error: Invalid ADD_SUPPLY parameters.\n";
            return true;
        }
        handleAddSupply(id, city, supplyType, amount, emergencyLevel);
        return true;

    }else if(cmd == "ADD_RESCUE"){
        std::string id, city, buildingRisk; 
        int numPeople, emergencyLevel;
        if(!(ss>>id>>city>>numPeople>>buildingRisk>>emergencyLevel)){
            std::cout << "Error: Invalid ADD_RESCUE parameters.\n";
            return true;
        }
        handleAddRescue(id, city, numPeople, buildingRisk, emergencyLevel);
        return true;

    }else if(cmd == "REMOVE_REQUEST"){
        std::string id;
        if(!(ss>>id)){
            std::cout << "Error: Invalid REMOVE_REQUEST parameters.\n";
            return true;
        }
        bool result = handleRemoveRequest(id);
        if(result){
            std::cout<<"Request "<<id<<" removed from queues."<<std::endl;
        }else{
            std::cout<<"Error: Request "<<id<<" not found."<<std::endl;
        }
        return true;

    }else if(cmd == "HANDLE_EMERGENCY"){
        int teamID, k;
        if(!(ss>>teamID>>k)){
            std::cout << "Error: Invalid HANDLE_EMERGENCY parameters.\n";
            return true;
        }
        handleHandleEmergency(teamID, k);
        return true;

    }else if(cmd == "DISPATCH_TEAM"){
        int teamID;
        if(!(ss>>teamID)){
            std::cout << "Error: Invalid DISPATCH_TEAM parameters.\n";
            return true;
        }
        handleDispatchTeam(teamID);
        return true;

    }else if(cmd == "PRINT_QUEUES"){
        printQueues();
        return true;

    }else if(cmd == "PRINT_TEAM"){
        int teamID;
        if(!(ss>>teamID)){
            std::cout << "Error: Invalid PRINT_TEAM parameters.\n";
            return true;
        }
        printTeam(teamID);
        return true;

    }else if(cmd == "CLEAR"){
        clear();
        std::cout<<"System cleared."<<std::endl;
        return true;

    }else{
        std::cout << "Error: Unknown command '" << cmd << "'." << std::endl;
        return true;
    }
}

bool QuakeAssistController::initializeTeams(int numTeams) {
    //Create a team array and initialize it with teams.
    if(numTeams <= 0){
        return false;
    }
    clear();
    if(teams != NULL){
        delete[] teams;
        teams = nullptr;
    }
    teams = new Team[numTeams];
    teamCount = numTeams;
    for(int i=0; i<teamCount; i++){
        teams[i].setId(i);
    }
    std::cout<<"Initialized "<<numTeams<< " teams."<<std::endl;
    return true;
}

int QuakeAssistController::findTeamIndexById(int teamId) const {
    //Find the index of the team using teamId.
    for(int i = 0; i<teamCount; i++){
        if(teams[i].getId() == teamId){
            return i;
        }
    }
    return -1;
}

bool QuakeAssistController::handleSetTeamCapacity(int teamId, int capacity) {
    //Find the index of team in the array, update the capacity value of the team.
    int teamIndex = findTeamIndexById(teamId);
    if(teamIndex == -1){
        return false;
    }
    teams[teamIndex].setMaxLoadCapacity(capacity);
    return true;
}

bool QuakeAssistController::handleAddSupply(const std::string& id,
                                            const std::string& cityStr,
                                            const std::string& supplyTypeStr,
                                            int amount,
                                            int emergencyLevel) {
    //Create new supply request, and add it to the SUPPLY queue.
    Request newSupply(id, cityStr, supplyTypeStr, amount, emergencyLevel);
    supplyQueue.enqueue(newSupply);
    std::cout<<"Request "<<id<<" added to SUPPLY queue."<<std::endl;
    return true;
}

bool QuakeAssistController::handleAddRescue(const std::string& id,
                                            const std::string& cityStr,
                                            int numPeople,
                                            const std::string& riskStr,
                                            int emergencyLevel) {
    //Create new rescue request, and add it to the RESCUE queue.
    Request newRescue(id, cityStr, numPeople, riskStr, emergencyLevel);
    rescueQueue.enqueue(newRescue);
    std::cout<<"Request "<<id<<" added to RESCUE queue."<<std::endl;
    return true;
}

bool QuakeAssistController::handleRemoveRequest(const std::string& id) {
    //Remove request using request ID from request(SUPPLY, RESCUE) queue. 
    if(supplyQueue.removeById(id)){
        return true;
    }else if(rescueQueue.removeById(id)){
        return true;
    }
    return false;
}

bool QuakeAssistController::handleHandleEmergency(int teamId, int k) {
    int teamIndex = findTeamIndexById(teamId);
    if (teamIndex == -1) {
        std::cout << "Error: Team " << teamId << " not found.\n";
        return false;
    }
    int supplycounter = 0;
    int rescuecounter = 0;
    int totalLoad = 0;
    
    for(int i = 0; i<k; i++){
        Request supplyRequset, rescueRequest;
        bool supplyFront = supplyQueue.peek(supplyRequset);
        bool rescueFront = rescueQueue.peek(rescueRequest);
        if(!supplyFront && !rescueFront){break;}
        
        Request chosenRequest;
        if(!supplyFront){
            chosenRequest = rescueRequest;

        }else if(!rescueFront){
            chosenRequest = supplyRequset;

        }else{
            int scoreRescue = rescueRequest.computeEmergencyScore();
            int scoreSupply = supplyRequset.computeEmergencyScore();
            if(scoreSupply > scoreRescue){
                chosenRequest = supplyRequset;
            }else{
                chosenRequest = rescueRequest;
            }
        }
        bool result = teams[teamIndex].tryAssignRequest(chosenRequest);
        if(result){
            if(chosenRequest.getType() == "SUPPLY"){
                supplyQueue.dequeue(chosenRequest);
                supplycounter++;
            }else{
                rescueQueue.dequeue(chosenRequest);
                rescuecounter++;
            }
            totalLoad += chosenRequest.computeWorkloadContribution();
            continue;
        }else{
            std::cout<<"Overload on Team "<< teamId << ": rolling back mission."<<std::endl;
            teams[teamIndex].rollbackMission(supplyQueue, rescueQueue);
            return true;
        }
    }
    int finalLoad = teams[teamIndex].getCurrentWorkload();
    std::cout<<"Team "<<teamId<<" assigned "<< (supplycounter+rescuecounter) <<" requests (" <<supplycounter<<" SUPPLY, "<<rescuecounter<<" RESCUE), total workload "<<finalLoad<<"."<<std::endl;
    return true;
    // TODO: Implement:
    // 1) Find team by id.
    // 2) For up to k steps:
    //    - Look at front of supplyQueue and rescueQueue using peek().
    //    - Use Request::computeEmergencyScore() to decide which to take.
    //    - If both empty -> break.
    //    - Try teams[teamIdx].tryAssignRequest(chosenRequest).
    //       * If this returns false, print overload message and
    //         call teams[teamIdx].rollbackMission(supplyQueue, rescueQueue),
    //         then break.
    //       * Else, dequeue chosen request from its queue and continue.    
}

bool QuakeAssistController::handleDispatchTeam(int teamId) {
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }
    Team& t = teams[idx];
    if (!t.hasActiveMission()) {
        std::cout << "Error: Team " << teamId << " has no active mission." << std::endl;
        return true;
    }
    int workload = t.getCurrentWorkload();
    std::cout << "Team " << teamId << " dispatched with workload " << workload << "." << std::endl;
    t.clearMission();
    return true;
}

void QuakeAssistController::printQueues() const {
    //Print queues.
    std::cout<<"SUPPLY QUEUE:"<<std::endl;
    Request* supplyData = supplyQueue.getData();
    int supplyCount = supplyQueue.getCount();
    int supplyCapacity = supplyQueue.getCapacity();
    int frontIndex = supplyQueue.getFrontIndex();
    for(int i = 0; i < supplyCount; i++){
        int circularIndex = (frontIndex + i) % supplyCapacity;
        std::cout<<supplyData[circularIndex ].getId()<<" "<<supplyData[circularIndex ].getCity()<<" "<<supplyData[circularIndex ].getSupplyType()
        <<" "<<supplyData[circularIndex ].getAmount()<<" "<<supplyData[circularIndex ].getEmergencyLevel()<<std::endl;
    }

    std::cout<<"RESCUE QUEUE:"<<std::endl;
    Request* rescueData = rescueQueue.getData();
    int rescueCount = rescueQueue.getCount();
    int rescueCapacity = rescueQueue.getCapacity();
    int frontIndexRescue = rescueQueue.getFrontIndex();
    for(int i = 0; i < rescueCount; i++){
        int cirIdxR = (frontIndexRescue + i)% rescueCapacity;
        std::cout<<rescueData[cirIdxR].getId()<<" "<<rescueData[cirIdxR].getCity()<<" "<<rescueData[cirIdxR].getNumPeople()
        <<" "<<rescueData[cirIdxR].getRisk()<<" "<<rescueData[cirIdxR].getEmergencyLevel()<<std::endl;
    }
}

void QuakeAssistController::printTeam(int teamId) const {
    //Print team data using teamId.
    int teamIndex = findTeamIndexById(teamId);
    if(teamIndex == -1){
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return;
    }
    std::cout<<"TEAM "<< teamId << " STACK:"<<std::endl;
    int stackSize = teams[teamIndex].getMissionStack().size();
    Request* data = teams[teamIndex].getMissionStack().getData();
    
    for(int i = stackSize-1; i >= 0; i--){
        if(data[i].getType() == "SUPPLY"){
            std::cout<<data[i].getId()<<" "<<data[i].getCity()<<" "<<data[i].getSupplyType()<<" "<<data[i].getAmount()<<" "<<data[i].getEmergencyLevel()<<std::endl;
        }else{
            std::cout<<data[i].getId()<<" "<<data[i].getCity()<<" "<<data[i].getNumPeople()<<" "<<data[i].getRisk()<<" "<<data[i].getEmergencyLevel()<<std::endl;
        }        
    }
}

void QuakeAssistController::clear() {
    //Clear data.
    if(teams != NULL){
        for(int i = 0 ; i<teamCount ;i++){
        teams[i].clearMission();
        }
    }
    rescueQueue.clear();
    supplyQueue.clear();
}
