
#include "ArtifactManager.h"
#include <iostream>
#include <sstream>

ArtifactManager::ArtifactManager()
{
}

ArtifactManager::~ArtifactManager()
{
}

int ArtifactManager::tokenize(const std::string &line, std::string tokens[], int maxTokens) const
{
    std::istringstream iss(line);
    std::string tok;
    int count = 0;
    while (iss >> tok && count < maxTokens)
    {
        tokens[count++] = tok;
    }
    return count;
}

void ArtifactManager::parseAndExecute(const std::string &line)
{
    // TODO: read lines and execuıte each command
    // Print "Error: Unknown command" if command is not known

    std::string tokens[15];
    int count = tokenize(line, tokens, 15);
    if (count == 0){
        return;}
    std::string command = tokens[0];

    if(command == "ADD_ARTIFACT"){
        handleAddArtifact(tokens, count);
    }
    else if(command == "REMOVE_ARTIFACT"){
        handleRemoveArtifact(tokens, count);
    }
    else if(command == "HIRE_RESEARCHER"){
        handleHireResearcher(tokens, count);
    }
    else if(command == "FIRE_RESEARCHER"){
        handleFireResearcher(tokens, count);
    }
    else if(command == "REQUEST"){
        handleRequest(tokens, count);
    }
    else if(command == "RETURN"){
        handleReturn(tokens, count);
    }
    else if(command == "RETURN_ALL"){
        handleReturnAll(tokens, count);
    }
    else if(command == "RESEARCHER_LOAD"){
        handleResearcherLoad(tokens, count);
    }
    else if(command == "MATCH_RARITY"){
        handleMatchRarity(tokens, count);
    }
    else if(command == "PRINT_UNASSIGNED"){
        handlePrintUnassigned(tokens, count);
    }
    else if(command == "PRINT_STATS"){
        handlePrintStats(tokens, count);
    }  
    else if(command == "CLEAR"){
        handleClear(tokens, count);
    }
    else{
        std::cout<<"Error: Unknown command '"<<command<<"'."<<std::endl;
    }
}

// =================== COMMAND HANDLERS ===================

void ArtifactManager::handleAddArtifact(const std::string tokens[], int count)
{
    // Expected: ADD_ARTIFACT <id> <name> <rarity> <value>
    // TODO:
    // 1) Check parameter count.
    // 2) Convert <id>, <rarity>, <value> to integers.
    // 3) Create Artifact and attempt to insert into AVL tree.
    // 4) On success: print "Artifact <id> added."
    // 5) On duplicate: print appropriate error (as in the PDF).
    if(count != 5){
        std::cout << "Error: Invalid parameter for ADD_ARTIFACT." << std::endl;
        return;
    }
    int id, rarity, value;
    std::string name = tokens[2];
    try
    {
        id = std::stoi(tokens[1]);      
        rarity = std::stoi(tokens[3]);  
        value = std::stoi(tokens[4]);
    }
    catch(const std::exception& e)
    {
       std::cout << "Error: Invalid parameter for ADD_ARTIFACT." << std::endl;
       return;
    }
    if(rarity<1 || rarity > 10){
        std::cout << "Error: Invalid parameter for ADD_ARTIFACT." << std::endl;
        return;
    }
    Artifact newArtifact(id, name, rarity, value); 
    bool result = artifactTree.insertArtifact(newArtifact);
    if(!result){
        std::cout << "Error: Artifact "<<id<<" already exists." << std::endl;
        return;
    }
    std::cout<<"Artifact "<<id<<" added."<<std::endl;
}

void ArtifactManager::handleRemoveArtifact(const std::string tokens[], int count)
{
    // Expected: REMOVE_ARTIFACT <id>
    // TODO:
    // 1) Parse id.
    // 2) Find artifact in AVL; if not found, print error.
    // 3) If artifact is assigned , find researcher and
    //    remove artifact from their list.
    // 4) Remove artifact from AVL; print success or error message.
    if(count != 2){
        std::cout << "Error: Invalid parameter for REMOVE_ARTIFACT." << std::endl;
        return; 
    }
    int id;
    try
    {
        id = std::stoi(tokens[1]);      
    }
    catch(const std::exception& e)
    {
       std::cout << "Error: Invalid parameter for REMOVE_ARTIFACT." << std::endl;
       return;
    }
    ArtifactNode* removedNode = artifactTree.findArtifact(id);
    if(removedNode == nullptr){
        std::cout << "Error: Artifact "<<id<<" not found." << std::endl;
        return;
    }
    std::string researcherName = removedNode->data.assignedToName;
    if(researcherName == ""){
        artifactTree.removeArtifact(id);
        std::cout << "Artifact " << id << " removed." << std::endl;
        return;
    }else{
        ResearcherNode* researcherNode = researcherTree.findResearcher(researcherName);
        researcherNode->data.removeArtifact(id);
        artifactTree.removeArtifact(id);
        std::cout << "Artifact " << id << " removed." << std::endl;
        return;
    }
}

void ArtifactManager::handleHireResearcher(const std::string tokens[], int count)
{
    // Expected: HIRE_RESEARCHER <name> <capacity>
    // TODO:
    // 1) Parse name and capacity.
    // 2) Create Researcher and insert into RedBlackTree.
    // 3) On success: "Researcher <name> hired."
    // 4) On duplicate: error message.
    if(count != 3){
        std::cout << "Error: Invalid parameter for HIRE_RESEARCHER." << std::endl;
        return;
    }
    int capacity;
    std::string name = tokens[1];
    try
    {
        capacity = std::stoi(tokens[2]);      
    }
    catch(const std::exception& e)
    {
       std::cout << "Error: Invalid parameter for HIRE_RESEARCHER." << std::endl;
       return;
    }
    Researcher newResearcher(name, capacity);
    bool result = researcherTree.insertResearcher(newResearcher);
    if(result){
        std::cout<<"Researcher "<<name<<" hired."<<std::endl;
    }else{
        std::cout << "Error: Researcher "<<name<<" already exists." << std::endl;
    }
}

void ArtifactManager::handleFireResearcher(const std::string tokens[], int count)
{
    // Expected: FIRE_RESEARCHER <name>
    // TODO:
    // 1) Find researcher by name. If not found, print error.
    // 2) For each artifact ID in their assignment list:
    //      - clear assignedToName in AVL.
    // 3) Remove researcher from RBT.
    // 4) Print success message.
    if(count != 2){
        std::cout << "Error: Invalid parameter for FIRE_RESEARCHER." << std::endl;
        return;
    }
    std::string name = tokens[1];
    ResearcherNode* researcherNode = researcherTree.findResearcher(name);
    if(researcherNode == nullptr){
        std::cout << "Error: Reseracher "<<name<<" not found." << std::endl;
        return;
    }
    int numAssigned = researcherNode->data.numAssigned;
    int* assignedArtifactsArray = researcherNode->data.assignedArtifacts;
    
    for(int i = 0; i<numAssigned; i++){
        artifactTree.clearAssignedTo(assignedArtifactsArray[i]);
    }
    researcherTree.removeResearcher(name);

    std::cout<<"Researcher "<<name<<" fired."<<std::endl;
}

void ArtifactManager::handleRequest(const std::string tokens[], int count)
{
    // Expected: REQUEST <researcherName> <artifactID>
    // TODO:
    // 1) Find researcher by name; error if missing.
    // 2) Find artifact by ID; error if missing.
    // 3) Check artifact is unassigned; error if already assigned.
    // 4) Check researcher capacity; error if at full capacity.
    // 5) On success: add artifact to researcher list AND set assignedToName in AVL.
    //    Print "Artifact <id> assigned to <name>."
    if(count != 3){
        std::cout << "Error: Invalid parameter for REQUEST." << std::endl;
        return;
    }
    int id;
    std::string name = tokens[1];
    try
    {
        id = std::stoi(tokens[2]);      
    }
    catch(const std::exception& e)
    {
       std::cout << "Error: Invalid parameter for REQUEST." << std::endl;
       return;
    }
    ArtifactNode* artifact =artifactTree.findArtifact(id);
    if(artifact == nullptr){
        std::cout << "Error: Artifact "<<id<<" not found." << std::endl;
        return;
    }
    if(artifact->data.assignedToName !=""){
        std::cout << "Error: Artifact "<<id<<" already assigned." << std::endl;
        return;
    }
    ResearcherNode* researcher = researcherTree.findResearcher(name);
    if(researcher == nullptr){
        std::cout << "Error: Researcher "<<name<<" not found." << std::endl;
        return;
    }
    if(researcher->data.numAssigned >= researcher->data.capacity){
        std::cout << "Error: Researcher "<<name<<" is at full capacity." << std::endl;
        return;
    }
    researcher->data.addArtifact(id);
    artifactTree.setAssignedTo(id, name);
    artifact->data.updateValueBasedOnUsage();
    std::cout<<"Artifact "<<id<<" assigned to "<<name<<"."<<std::endl;
}

void ArtifactManager::handleReturn(const std::string tokens[], int count)
{
    // Expected: RETURN <researcherName> <artifactID>
    // TODO:
    // 1) Validate existence of researcher and artifact.
    // 2) Check that artifact.assignedToName == researcherName.
    // 3) If not, print error.
    // 4) Otherwise, remove artifact from researcher list, clear assignedToName in AVL.
    //    Print "Artifact <id> returned by <name>."
    if(count != 3){
        std::cout << "Error: Invalid parameter for RETURN." << std::endl;
        return;
    }
    int id;
    std::string name = tokens[1];
    try
    {
        id = std::stoi(tokens[2]);      
    }
    catch(const std::exception& e)
    {
       std::cout << "Error: Invalid parameter for RETURN." << std::endl;
       return;
    }
    ArtifactNode* artifact =artifactTree.findArtifact(id);
    if(artifact == nullptr){
        std::cout << "Error: Artifact "<<id<<" not found." << std::endl;
        return;
    }
    ResearcherNode* researcher = researcherTree.findResearcher(name);
    if(researcher == nullptr){
        std::cout << "Error: Researcher "<<name<<" not found." << std::endl;
        return;
    }
    if(artifact->data.assignedToName != name){
        std::cout << "Error: Artifact "<<id<<" not assigned to researcher " <<name<< std::endl;
        return;
    }
    researcher->data.removeArtifact(id);
    artifactTree.clearAssignedTo(id);
    std::cout<<"Artifact "<<id<<" returned by "<<name<<"."<<std::endl;
}

void ArtifactManager::handleReturnAll(const std::string tokens[], int count)
{
    // Expected: RETURN_ALL <researcherName>
    // TODO:
    // 1) Find researcher; error if missing.
    // 2) For each artifact they supervise, clear assignedToName in AVL.
    // 3) Clear researcher's assignment list (removeAllArtifacts()).
    // 4) Print appropriate confirmation message.
    if(count != 2){
        std::cout << "Error: Invalid parameter for RETURN_ALL." << std::endl;
        return;
    }
    std::string name = tokens[1];
    ResearcherNode* researcher = researcherTree.findResearcher(name);
    if(researcher == nullptr){
        std::cout << "Error: Researcher "<<name<<" not found." << std::endl;
        return;
    }
    int numAssigned = researcher->data.numAssigned;
    int* assignedArtifactsArray = researcher->data.assignedArtifacts;
   
    for(int i = 0; i<numAssigned; i++){
        artifactTree.clearAssignedTo(assignedArtifactsArray[i]);
    }
    researcher->data.removeAllArtifacts();
    
    std::cout<<"All artifacts returned by "<<name<<"."<<std::endl;
}

void ArtifactManager::handleResearcherLoad(const std::string tokens[], int count)
{
    // Expected: RESEARCHER_LOAD <name>
    // TODO:
    // 1) Find researcher by name.
    // 2) If not found, print error.
    // 3) Otherwise, print number of supervised artifacts in required format.
    if(count != 2){
        std::cout << "Error: Invalid parameter for RESEARCHER_LOAD." << std::endl;
        return;
    }
    std::string name = tokens[1];
    ResearcherNode* researcher = researcherTree.findResearcher(name);
    if(researcher == nullptr){
        std::cout << "Error: Researcher "<<name<<" not found." << std::endl;
        return;
    }
    int load = researcher->data.numAssigned;
    std::cout << "Researcher " << name << " is supervising " << load << " artifacts." << std::endl;
}

void ArtifactManager::handleMatchRarity(const std::string tokens[], int count)
{
    // Expected: MATCH_RARITY <minRarity>
    // TODO:
    // Traverse AVL tree and print all artifacts with rarity >= minRarity.
    // You may choose any reasonable order (probably inorder) unless specified otherwise
    // in your PDF. Artifacts may be assigned or unassigned; print as required.
    if(count != 2){
        std::cout << "Error: Invalid parameter for MATCH_RARITY." << std::endl;
        return;
    }
    int minRarity;
    try
    {
        minRarity = std::stoi(tokens[1]);      
    }
    catch(const std::exception& e)
    {
       std::cout << "Error: Invalid parameter for MATCH_RARITY." << std::endl;
       return;
    }

    std::cout<<"=== MATCH_RARITY "<<minRarity<<" ==="<<std::endl; 
    artifactTree.printMatchRarity(minRarity);

}

void ArtifactManager::handlePrintUnassigned(const std::string tokens[], int count)
{
    // Expected: PRINT_UNASSIGNED
    // TODO:
    // Print a header if needed, then call artifactTree.printUnassigned().
    if(count != 1){
        std::cout << "Error: Invalid parameter for PRINT_UNASSIGNED." << std::endl;
        return;
    }
    std::cout<<"Unassigned artifacts:"<<std::endl;
    artifactTree.printUnassigned();
}

void ArtifactManager::handlePrintStats(const std::string tokens[], int count)
{
    // Expected: PRINT_STATS
    // TODO:
    // 1) Compute:
    //    - totalArtifacts (artifactTree.getArtifactCount())
    //    - totalResearchers (researcherTree.getResearcherCount())
    //    - average artifact rarity (floor of totalRarity / totalArtifacts)
    //    - average researcher load (floor of totalLoad / totalResearchers)
    // 2) Print summary lines exactly as in the spec.
    // 3) Then:
    //    - Print researchers using preorder traversal:
    //      researcherTree.traversePreOrderForStats()
    //    - Print artifacts using postorder traversal:
    //      artifactTree.traversePostOrderForStats()
    //    (Exact formatting defined in your PDF.)
    if(count != 1){
        std::cout << "Error: Invalid parameter for PRINT_STATS." << std::endl;
        return;
    }
    std::cout<<"=== SYSTEM STATISTICS ==="<<std::endl;
    int totalArtifacts = artifactTree.getArtifactCount();
    int totalResearchers = researcherTree.getResearcherCount();
    int totalRarity = artifactTree.getTotalRarity();
    int totalLoad = researcherTree.getTotalLoad();
    int  average_artifact_rarity;
    int average_researcher_load;
    if(totalArtifacts == 0){
        average_artifact_rarity = 0;
    }else{
        average_artifact_rarity = (totalRarity / totalArtifacts);
    }
    if(totalResearchers == 0){
        average_researcher_load = 0;
    }else{
        average_researcher_load = (totalLoad / totalResearchers);
    }
    std::cout<<"Artifacts: "<<totalArtifacts<<std::endl;
    std::cout<<"Researchers: "<<totalResearchers<<std::endl;
    std::cout<<"Average rarity: "<<average_artifact_rarity<<std::endl;
    std::cout<<"Average load: "<<average_researcher_load<<std::endl;

    std::cout<<"Researchers:"<<std::endl;
    researcherTree.traversePreOrderForStats();
    std::cout<<"Artifacts:"<<std::endl;
    artifactTree.traversePostOrderForStats();
}

void ArtifactManager::handleClear(const std::string tokens[], int count)
{
    // Expected: CLEAR
    // TODO:
    // Clear both trees and print confirmation message.
    if(count != 1){
        std::cout << "Error: Invalid parameter for CLEAR." << std::endl;
        return;
    }
    artifactTree.clear();
    researcherTree.clear();
    std::cout<<"All data cleared."<<std::endl;
    // e.g. "All data cleared."
}
