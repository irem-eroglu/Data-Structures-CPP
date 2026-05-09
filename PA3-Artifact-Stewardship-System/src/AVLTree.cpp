#include "AVLTree.h"
#include <iostream> 
AVLTree::AVLTree()
    : root(nullptr)
{
}

AVLTree::~AVLTree()
{
    clear();
}

void AVLTree::clear()
{
    clear(root);
    root = nullptr;
}

void AVLTree::clear(ArtifactNode *node)
{
//TODO
    if(!node){
        return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
}

int AVLTree::height(ArtifactNode *node) const
{
    return node ? node->height : -1;
}

int AVLTree::getBalance(ArtifactNode *node) const
{
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

ArtifactNode *AVLTree::rotateLeft(ArtifactNode *x)
{
    // TODO: Standard AVL left rotation.
    //return new root
    ArtifactNode* newRoot = x->right;
    ArtifactNode* changeChild = newRoot->left;
    newRoot->left= x;
    x->right = changeChild;
    
    if(height(x->right) > height(x->left)){
        x->height = height(x->right) + 1;
    }else{
        x->height = height(x->left) + 1;
    }

    if(height(newRoot->right) > height(newRoot->left)){
        newRoot->height = height(newRoot->right) + 1;
    }else{
        newRoot->height = height(newRoot->left) + 1;
    }

    return newRoot;
}

ArtifactNode *AVLTree::rotateRight(ArtifactNode *y)
{
    // TODO: Standard AVL right rotation (mirror of rotateLeft).

    ArtifactNode* newRoot = y->left;
    ArtifactNode* changeChild = newRoot->right;
    newRoot->right= y;
    y->left = changeChild;

    y->height = std::max(height(y->left), height(y->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;
    return newRoot;
}

ArtifactNode *AVLTree::findMinNode(ArtifactNode *node) const
{
    // TODO: Return leftmost node in this subtree.
    if(node == nullptr){
        return nullptr;
    }
    if(node->left == nullptr){
        return node;
    }
    return findMinNode(node->left);
}

ArtifactNode *AVLTree::insert(ArtifactNode *node, const Artifact &artifact, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by artifactID.
    // 2) If duplicate ID, set inserted = false and return node unchanged.
    // 3) On actual insertion, update node->height.
    // 4) Compute balance and apply AVL rotations if needed.
    // 5) Return the (possibly new) root of this subtree.
    int key = artifact.artifactID;
    if(node== nullptr){
        ArtifactNode* newNode = new ArtifactNode(artifact);
        inserted = true;    
        return newNode;

    }else if(key == node->data.artifactID){
        inserted = false;
        return node;
       
    }else if(node->data.artifactID > key){
        node->left = insert(node->left, artifact, inserted);

    }else{
        node->right = insert(node->right, artifact, inserted);
    }
    
    
    node->height =std::max(height(node->left), height(node->right)) + 1;
    int balance = getBalance(node);
    
    if(balance <= 1 && balance >= -1){
        return node;
    }
    if(balance > 1 && key < node->left->data.artifactID){
        return rotateRight(node);
    }
    if(balance < -1 && key > node->right->data.artifactID){
        return rotateLeft(node);
    }
    if(balance < -1 && key < node->right->data.artifactID){
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    if(balance > 1 && key > node->left->data.artifactID){
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    return node;
}

ArtifactNode *AVLTree::remove(ArtifactNode *node, int artifactID, bool &removed)
{
    // TODO:
    // 1) Standard BST deletion by artifactID.
    // 2) If node not found, leave removed = false.
    // 3) Upon deletion, update heights and rebalance with rotations.
    // 4) Return (possibly new) root of this subtree.

    if(node == nullptr){
        removed = false;
        return nullptr;

    }else if(artifactID < node->data.artifactID){
        node->left = remove(node->left, artifactID, removed);
       
    }else if(artifactID > node->data.artifactID){
        node->right = remove(node->right, artifactID, removed);
       
    }else{
        removed = true;        
        if(node->left == nullptr && node->right == nullptr){
            delete node;
            node = nullptr;
        }else if(node->left == nullptr){
            ArtifactNode* temp = node->right;
            ArtifactNode* child = temp;
            delete node;
            node = child;
            return child;

        }else if(node->right == nullptr){
            ArtifactNode* temp = node->left;
            ArtifactNode* child = temp;
            delete node;
            node = child;
            return child;
        }else{
            ArtifactNode* newRoot = findMinNode(node->right);
            node->data = newRoot->data;
            node->right = remove(node->right, newRoot->data.artifactID, removed);
        }
    }    
    
    if(node == nullptr){
        return nullptr;
    }
    
    node->height =std::max(height(node->left), height(node->right)) + 1;
    int balance = getBalance(node);
    
    if(balance <= 1 && balance >= -1){
        return node;
    }
    if(balance > 1 && getBalance(node->left) >= 0){
        return rotateRight(node);
    }
    if(balance < -1 && getBalance(node->right) <= 0){
        return rotateLeft(node);
    }
    if(balance < -1 && getBalance(node->right) > 0){
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    if(balance > 1 && getBalance(node->left) < 0){
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    return node;
}

bool AVLTree::insertArtifact(const Artifact &artifact)
{
    bool inserted = false;//tracks whether operation is succesful
    root = insert(root, artifact, inserted);
    return inserted;
}

bool AVLTree::removeArtifact(int artifactID)
{
    bool removed = false;//tracks whether operation is succesful
    root = remove(root, artifactID, removed);
    return removed;
}

ArtifactNode *AVLTree::findArtifact(int artifactID) const
{
    return find(root, artifactID);
}

ArtifactNode *AVLTree::find(ArtifactNode *node, int artifactID) const
{
    //TODO
    if(node == nullptr){
        return nullptr;

    }else if(artifactID == node->data.artifactID){
        return node;
       
    }else if(node->data.artifactID > artifactID){
        return find(node->left, artifactID);

    }else{
        return find(node->right, artifactID);
    } 
    return nullptr;
}

void AVLTree::setAssignedTo(int artifactID, const std::string &researcherName)
{
    //assign artifact to researcher
    ArtifactNode* node = findArtifact(artifactID);
    if(node == nullptr){
        return;
    }
    node->data.assignedToName = researcherName;
    
}

void AVLTree::clearAssignedTo(int artifactID)
{
    //TODO
    ArtifactNode* node = findArtifact(artifactID);
    if(node == nullptr){
        return;
    }
    node->data.assignedToName = "";
}

void AVLTree::printUnassigned() const
{
    printUnassigned(root);
}

void AVLTree::printUnassigned(ArtifactNode *node) const
{
    // TODO:
    // Inorder traversal.
    // For each node with data.assignedToName == "", print in required format, e.g.:
    // <id> <name> <rarity> <value>
    if(!node){
        return;
    }
    printUnassigned(node->left);
    if(node->data.assignedToName == ""){
        std::cout<<node->data.artifactID<<" "<<node->data.name<<" "<<node->data.rarityLevel<<" "<<node->data.researchValue<<std::endl;
    }
    printUnassigned(node->right);
}

int AVLTree::getArtifactCount() const
{
    return getArtifactCount(root);
}

int AVLTree::getArtifactCount(ArtifactNode *node) const
{
    // TODO: return size of subtree.
    if(!node){
        return 0;
    }
    return 1 + getArtifactCount(node->left) + getArtifactCount(node->right);
}

int AVLTree::getTotalRarity() const
{
    return getTotalRarity(root);
}

int AVLTree::getTotalRarity(ArtifactNode *node) const
{
    // TODO: sum of rarityLevel over subtree.
    if(node == nullptr){
        return 0;
    }
    int leftRarity = getTotalRarity(node->left);
    int rightRarity = getTotalRarity(node->right);
    return node->data.rarityLevel + leftRarity + rightRarity;
}

void AVLTree::traversePostOrderForStats() const
{
    traversePostOrderForStats(root);
}

void AVLTree::traversePostOrderForStats(ArtifactNode *node) const
{
    // TODO:
    // Post-order traversal (left, right, node).
    // Students will decide what exactly to print in PRINT_STATS.
    if(!node){
        return;
    }
    traversePostOrderForStats(node->left);
    traversePostOrderForStats(node->right);
    if(node->data.assignedToName == ""){
        std::cout<<node->data.artifactID<<" "<<node->data.name<<" "<<node->data.rarityLevel<<" "<<node->data.researchValue<<" "<<"UNASSIGNED"<<std::endl;    
    }else{
    std::cout<<node->data.artifactID<<" "<<node->data.name<<" "<<node->data.rarityLevel<<" "<<node->data.researchValue<<" "<<node->data.assignedToName<<std::endl;
    }
}

//I added it for handle match rarity
void AVLTree::printMatchRarity(int minRarity) 
{
    printMatchRarity(root, minRarity);
}
void AVLTree::printMatchRarity(ArtifactNode* node, int minRarity){
    if(!node){
        return;
    }
    printMatchRarity(node->left, minRarity);

    if(node->data.rarityLevel >= minRarity){
    std::cout << node->data.artifactID << " "<< node->data.name << " "<< node->data.rarityLevel << " "<< node->data.researchValue << " ";
        if(node->data.assignedToName == ""){
            std::cout << "UNASSIGNED" << std::endl;
        }else{
            std::cout << "ASSIGNED:" << node->data.assignedToName << std::endl;
        }
    }

    printMatchRarity(node->right, minRarity);
}