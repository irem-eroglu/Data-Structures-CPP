#include "Team.h"

Team::Team()
    : teamID(-1),
      maxLoadCapacity(0),
      currentWorkload(0),
      missionStack(4) {
}

Team::Team(int id, int maxLoad)
    : teamID(id),
      maxLoadCapacity(maxLoad),
      currentWorkload(0),
      missionStack(4) {
}

int Team::getId() const {
    return teamID;
}

int Team::getMaxLoadCapacity() const {
    return maxLoadCapacity;
}

int Team::getCurrentWorkload() const {
    return currentWorkload;
}

void Team::setId(int id) {
    teamID = id;
}

void Team::setMaxLoadCapacity(int maxLoad) {
    maxLoadCapacity = maxLoad;
}

bool Team::hasActiveMission() const {
    return !missionStack.isEmpty();
}

bool Team::tryAssignRequest(const Request& req) {
    //Implement tryAssignRequest function as explained in the PDF.
    int addedcapacity = req.computeWorkloadContribution();
    if(currentWorkload + addedcapacity <= getMaxLoadCapacity()){
        currentWorkload += addedcapacity;
        missionStack.push(req);
        return true;
    }
    return false;
}

void Team::rollbackMission(RequestQueue& supplyQueue, RequestQueue& rescueQueue) {
    //Implement rollbackMission function as explained in the PDF.
    Request* tempArray = new Request[missionStack.size()];
    int idx = 0;
    while(!missionStack.isEmpty()){
        Request outreq;
        missionStack.pop(outreq);
        tempArray[idx] = outreq;
        idx++;
    }
    int i = 0;
    int tempSupplySize = supplyQueue.getCount();
    Request* tempSupplyQueue = new Request[tempSupplySize];
    while(!supplyQueue.isEmpty()){
        Request outreq;
        supplyQueue.dequeue(outreq);
        tempSupplyQueue[i] = outreq;
        i++;
    }
    i = 0;
    int tempRescueSize = rescueQueue.getCount();
    Request* tempRescueQueue = new Request[tempRescueSize];
    while(!rescueQueue.isEmpty()){
        Request outreq;
        rescueQueue.dequeue(outreq);
        tempRescueQueue[i] = outreq;
        i++;
    }

    for(int i = idx-1; i >= 0; i--){
        std::string type = tempArray[i].getType();
        if(type == "SUPPLY"){
            supplyQueue.enqueue(tempArray[i]);
        }else{
            rescueQueue.enqueue(tempArray[i]);
        }
    }
    for(int i = 0; i < tempSupplySize; i++){
        supplyQueue.enqueue(tempSupplyQueue[i]);
    }
    for(int i = 0; i < tempRescueSize; i++){
        rescueQueue.enqueue(tempRescueQueue[i]);
    }
    
    delete[] tempSupplyQueue;
    delete[] tempRescueQueue;
    delete[] tempArray;
    currentWorkload = 0;
}

void Team::clearMission() {
    missionStack.clear();
    currentWorkload = 0;
}

const MissionStack& Team::getMissionStack() const {
    return missionStack;
}
