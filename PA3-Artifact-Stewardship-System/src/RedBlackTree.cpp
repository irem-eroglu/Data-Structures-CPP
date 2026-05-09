#include "RedBlackTree.h"

RedBlackTree::RedBlackTree()
    : root(nullptr)
{
}

RedBlackTree::~RedBlackTree()
{
    clear();
}

void RedBlackTree::clear()
{
    clear(root);
    root = nullptr;
}

void RedBlackTree::clear(ResearcherNode *node)
{
    //TODO:
    if(!node){
        return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
}

ResearcherNode *RedBlackTree::findResearcher(const std::string &fullName) const
{
    return find(root, fullName);
}

ResearcherNode *RedBlackTree::find(ResearcherNode *node, const std::string &fullName) const
{

//TODO: search and find researcher by name
    if(node == nullptr){
        return nullptr;
    }
    if(node->data.fullName == fullName){
        return node;
    }else if(node->data.fullName > fullName){
        return find(node->left, fullName);
    }else{
        return find(node->right, fullName);
    }
}

bool RedBlackTree::insertResearcher(const Researcher &researcher)
{
    //attempt to insert, return true if success, false otherwise
    ResearcherNode* newReseacher = new ResearcherNode(researcher);
    bool inserted = false;
    bstInsert(root, newReseacher, inserted);
    if(inserted){
        insertFixup(newReseacher);
        return true;
    }
    delete newReseacher;
    return false;
}

ResearcherNode *RedBlackTree::bstInsert(ResearcherNode *current, ResearcherNode *node, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by node->data.fullName.
    // 2) If tree is empty, node becomes root (set inserted = true).
    // 3) If name already exists, inserted = false and return current without inserting.
    // 4) Otherwise insert and set inserted = true.
    // 5) Do not modify colors here (node->color already RED).
    if(current == nullptr){
        current = node;
        if(root == nullptr){
            root = current;
        }
        inserted = true;
        return current;
    }
    if(current->data.fullName == node->data.fullName){
        inserted = false;
        return current;
    }
    if(current->data.fullName > node->data.fullName){
        current->left = bstInsert(current->left, node, inserted); 
    }else{
        current->right= bstInsert(current->right, node, inserted);
    }

    if(current->left == node){
            node->parent = current;
        }
    if(current->right == node){
            node->parent = current;
        }    
    return current;
}

void RedBlackTree::insertFixup(ResearcherNode *node)
{
    // TODO: Standard Red-Black insertion fixup.
    // While parent is RED, handle cases based on uncle's color and position.
    // Ensure root becomes BLACK at the end.
    ResearcherNode* uncle;
    if(!node){
        return;
    }    
    while(node->parent != nullptr && node->parent->color == RED){
        if(node->parent == node->parent->parent->left){
            uncle = node->parent->parent->right;
            if(uncle != nullptr && uncle->color == RED){
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
                continue;
            }
            if(uncle == nullptr || uncle->color == BLACK){
                if(node == node->parent->right){
                    node = node->parent;
                    rotateLeft(node);
                }    
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotateRight(node->parent->parent);
                continue;
            } 
        }else{
            uncle = node->parent->parent->left;
            if(uncle != nullptr && uncle->color == RED){
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
                continue;
            }
            if(uncle == nullptr || uncle->color == BLACK){
                if(node == node->parent->left){
                    node = node->parent;
                    rotateRight(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotateLeft(node->parent->parent);
                continue;
            } 
        }
    }
    root->color = BLACK;
}

bool RedBlackTree::removeResearcher(const std::string &fullName)
{
    // TODO:
    // 1) Find node z with data.fullName == fullName.
    // 2) If not found, return false.
    // 3) Perform standard RBT delete:
    //    - Track original color of removed node.
    //    - If a black node is removed, call deleteFixup on the appropriate child.
    // 4) Free node memory.
    // 5) Return true.
    ResearcherNode* z = findResearcher(fullName);
    if(z ==  nullptr){
        return false;
    }

    ResearcherNode *child = nullptr; 
    ResearcherNode *fixUpParent = nullptr; 
    Color color_info = z->color;
    
    if(z->left == nullptr && z->right == nullptr){
        if(z == root){
            root = nullptr;
        }else{
            if(z->parent->right == z){
            z->parent->right = nullptr;
            }else{
            z->parent->left = nullptr;
            }
            if(color_info == BLACK){
            deleteFixup(z, z->parent);
            }
        }
        delete z;
        return true;

    }else if(z->left == nullptr || z->right == nullptr){
        if(z->left == nullptr){
            child = z->right;
        }else{
            child = z->left;
        }        

        if (child != nullptr) { 
            child->parent = z->parent; 
        }

        if(z->parent == nullptr){
            root = child;
        }else if(z->parent->right == z){
            z->parent->right = child; 
        }else{
            z->parent->left = child;           
        }
        if(color_info == BLACK){
            deleteFixup(child, z->parent);
        }
        delete z;
        return true;

    }else{
        ResearcherNode *leftmost = minimum(z->right);
        color_info = leftmost->color;
        if(leftmost->right != nullptr){
            child = leftmost->right;
        }
        
        if(leftmost->parent == z){
            fixUpParent = leftmost;    

        }else{
            fixUpParent = leftmost->parent;
            if(child){
                child->parent = leftmost->parent;
            }

            if(leftmost == leftmost->parent->right){
                leftmost->parent->right = child;
            }else{
                leftmost->parent->left = child;
            }
            leftmost->right = z->right;
            leftmost->right->parent = leftmost;
        }

        if(z->parent == nullptr){
            root = leftmost;
        }else if(z->parent->right == z){
            z->parent->right = leftmost;
        }else{
            z->parent->left = leftmost;
        }
        leftmost->parent = z->parent;
        leftmost->left = z->left;
        if (leftmost->left != nullptr) { 
            leftmost->left->parent = leftmost;
        }
        leftmost->color = z->color;
        
        if(color_info == BLACK){
            deleteFixup(child, fixUpParent);
        }
        delete z;
        return true; 
    }
}

void RedBlackTree::deleteFixup(ResearcherNode *node, ResearcherNode *parent)
{
    // TODO: Standard Red-Black deletion fixup.
    if(node == root){
        if(node){
            node->color = BLACK;}       
        return;
    }
    ResearcherNode* sibling;
    while(node == nullptr || node->color == BLACK){
        if(node == root){
            break;
        }
        
        if(node == parent->left){
            sibling = parent->right;
            if(sibling != nullptr && sibling->color == RED){
                sibling->color = BLACK;
                parent->color = RED;
                rotateLeft(parent);
                sibling = parent->right;    
            }
            if(!sibling){
                node = parent;
                parent = node->parent;
                continue;
            }else{
                if(sibling->right == nullptr || sibling->right->color == BLACK){ 
                    if(sibling->left == nullptr || sibling->left->color == BLACK){
                        sibling->color = RED;
                        node = parent;
                        parent = node->parent;
                        continue;
                    }else{
                        sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = parent->right;
                    }
                }
                if(sibling->right != nullptr && sibling->right->color == RED){
                    sibling->color = parent->color;
                    parent->color = BLACK;
                    sibling->right->color = BLACK;
                    rotateLeft(parent);
                    node = root;
                    break;
                }
            }
        }else{
            sibling = parent->left;
            if(sibling != nullptr && sibling->color == RED){
                sibling->color = BLACK;
                parent->color = RED;
                rotateRight(parent);
                sibling = parent->left;    
            }
            if(!sibling){
                node = parent;
                parent = node->parent;
                continue;
            }else{
                if(sibling->left == nullptr || sibling->left->color == BLACK){ 
                    if(sibling->right == nullptr || sibling->right->color == BLACK){
                        sibling->color = RED;
                        node = parent;
                        parent = node->parent;
                        continue;
                    }else{
                        sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = parent->left;
                    }
                }
                if(sibling->left != nullptr && sibling->left->color == RED){
                    sibling->color = parent->color;
                    parent->color = BLACK;
                    sibling->left->color = BLACK;
                    rotateRight(parent);
                    node = root;
                    break;
                }
            }
        }
    }
    if(node != nullptr){
        node->color = BLACK;
    }    
}

ResearcherNode *RedBlackTree::minimum(ResearcherNode *node) const
{
    // TODO: Return leftmost node in subtree.
    if(node == nullptr){
        return nullptr;
    }
    if(node->left == nullptr){
        return node;
    }
    return minimum(node->left);
}

void RedBlackTree::rotateLeft(ResearcherNode *x)
{
    // TODO: Standard left rotation.
    ResearcherNode* newParent = x->right;
    x->right = newParent->left;
    if(newParent->left != nullptr){
        newParent->left->parent = x;
    }    
    newParent->parent = x->parent;
    if(x->parent != nullptr){
        if(x == x->parent->right){
            x->parent->right = newParent;
        }else{
            x->parent->left = newParent;
        }
    }else{
        root = newParent;
    }

    x->parent = newParent;
    newParent->left = x;
}

void RedBlackTree::rotateRight(ResearcherNode *y)
{
    // TODO: Standard right rotation.
    ResearcherNode* newParent = y->left;
    y->left = newParent->right;
    if(newParent->right != nullptr){
        newParent->right->parent = y;
    }    
    newParent->parent = y->parent;
    if(y->parent != nullptr){
        if(y == y->parent->right){
            y->parent->right = newParent;
        }else{
            y->parent->left = newParent;
        }
    }else{
        root = newParent;
    }
    y->parent = newParent;
    newParent->right = y;
}

int RedBlackTree::getResearcherCount() const
{
    return getResearcherCount(root);
}

int RedBlackTree::getResearcherCount(ResearcherNode *node) const
{
    // TODO: return size of subtree.
    if(node == nullptr){
        return 0;
    }
    int leftCount = getResearcherCount(node->left);
    int rightCount = getResearcherCount(node->right);
    return 1 + leftCount + rightCount;
}

int RedBlackTree::getTotalLoad() const
{
    return getTotalLoad(root);
}

int RedBlackTree::getTotalLoad(ResearcherNode *node) const
{
    // TODO: sum of data.numAssigned in subtree.
    if(!node){
        return 0;
    }
    int leftAssigned = getTotalLoad(node->left);
    int rightAssigned = getTotalLoad(node->right);
    return node->data.numAssigned + leftAssigned + rightAssigned;
}

void RedBlackTree::traversePreOrderForStats() const
{
    traversePreOrderForStats(root);
}

void RedBlackTree::traversePreOrderForStats(ResearcherNode *node) const
{
    // TODO:
    // Pre-order traversal (node, left, right).
    // Students will decide what exactly to print in PRINT_STATS.
    if(!node){
        return;
    }
    std::cout<<node->data.fullName<<" "<<node->data.capacity<<" "<<node->data.numAssigned<<std::endl;
    traversePreOrderForStats(node->left);
    traversePreOrderForStats(node->right);
}
