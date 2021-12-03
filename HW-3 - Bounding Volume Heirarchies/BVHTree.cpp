#include "BVHTree.h"

BVHTree::BVHTree(){
    root = NULL;
}

BVHTree::~BVHTree(){
    std::vector<std::string> keys;
    for (auto key : map) {
        keys.push_back(key.first);
    }
    
    for (int i = 0; i < keys.size(); i++) {
        removeBVHMember(keys[i]);
    }
    
    root = NULL;
}

void BVHTree::printNode(std::ostream &out, BVHTreeNode *node, int level) {
	if (root == nullptr) return;
	for (int i = 0; i < level; i++) {
		out << "  ";
	}
	if (!node->isLeaf) {
		out << "+ branch || ";
		node->aabb.printAABB(out);
		out << std::endl;
		printNode(out, node->rightChild, level + 1);
		printNode(out, node->leftChild, level + 1);
	}
	else {
		out << "- ";
		if (node->parent) {
			if (node->parent->rightChild == node)
				out << "R ";
			else
				out << "L ";
		}
		out << "- leaf: " << node->name << " || ";
		node->aabb.printAABB(out);
		out << std::endl;
	}
}
std::ostream &operator<<(std::ostream &out, BVHTree &tree) {
	tree.printNode(out, tree.root, 0);
	return out;
}

void BVHTree::resize(BVHTreeNode * node) {
    while (node->parent != NULL) {
        BVHTreeNode * sibling = node->parent->leftChild == node ? node->parent->rightChild : node->parent->leftChild;
        node->parent->aabb = node->aabb + sibling->aabb;
        node = node->parent;
    }
}

void BVHTree::addBVHMember(AABB objectArea, std::string name){
    
    if (map.find(name) != map.end()) return;
    
    if (map.size() == 0) {
        BVHTreeNode * node = new BVHTreeNode(objectArea, name, true);
        root = node;
        map[name] = node;
    }
    
    else if (map.size() == 1)  {
        BVHTreeNode * oldRoot = root;
        BVHTreeNode * newNode = new BVHTreeNode(objectArea, name, true);
        
        AABB branchArea = oldRoot->aabb + newNode->aabb;
        BVHTreeNode * branch = new BVHTreeNode(branchArea, "branch", false);
        
        root = branch;
        branch->leftChild = newNode;
        branch->rightChild = oldRoot;
        oldRoot->parent = newNode->parent = branch;
        
        map[name] = newNode;
    }
    
    else {
        BVHTreeNode * temp = root;
        BVHTreeNode * newNode = new BVHTreeNode(objectArea, name, true);
        
        while (!temp->isLeaf) {
            BVHTreeNode * left = temp->leftChild;
            BVHTreeNode * right = temp->rightChild;
            
            AABB leftArea = left->aabb + objectArea;
            AABB rightArea = right->aabb + objectArea;
            
            if (leftArea.getArea()-left->aabb.getArea() <= rightArea.getArea()-right->aabb.getArea())
                temp = temp->leftChild;
            
            else
                temp = temp->rightChild;
        }
        
        AABB branchArea = temp->aabb + newNode->aabb;
        BVHTreeNode * branch = new BVHTreeNode(branchArea, "branch", false);
        
        branch->parent = temp->parent;
        branch->leftChild = newNode;
        branch->rightChild = temp;
        
        temp->parent->leftChild == temp
            ? temp->parent->leftChild = branch
            : temp->parent->rightChild = branch;
        
        temp->parent = newNode->parent = branch;
        
        resize(newNode);
        map[name] = newNode;
    }
}

void BVHTree::removeBVHMember(std::string name){
    
    if (map.find(name) == map.end()) return;
    
    BVHTreeNode * toRemove = map[name];
    
    if (toRemove == root)
        root = NULL;
    
    
    else if (toRemove->parent == root){
        BVHTreeNode * temp = root;
        root = root->leftChild == toRemove ? root->rightChild : root->leftChild;
        root->parent = NULL;
        delete temp;
    }
    
    else {
        BVHTreeNode * temp = toRemove->parent;
        
        if (temp->parent->leftChild == temp)
            temp->parent->leftChild = temp->leftChild == toRemove ? temp->rightChild : temp->leftChild;
        
        else
            temp->parent->rightChild = temp->leftChild == toRemove ? temp->rightChild : temp->leftChild;
        
        toRemove->parent->leftChild == toRemove
            ? toRemove->parent->rightChild->parent = temp->parent
            : toRemove->parent->leftChild->parent = temp->parent;
        
        resize(toRemove->parent->leftChild == toRemove ? toRemove->parent->rightChild : toRemove->parent->leftChild);
        delete temp;
    }
    
    delete toRemove;
    map.erase(name);
}

void BVHTree::moveBVHMember(std::string name, AABB newLocation){
    BVHTreeNode * node = map[name];
    AABB temp = newLocation + node->parent->aabb;
    
    if (temp.getArea() - node->parent->aabb.getArea() == 0) {
        node->aabb = newLocation;
        //resize(node->parent);
    }
    
    else {
        removeBVHMember(name);
        addBVHMember(newLocation, name);
    }
}

std::vector<std::string> BVHTree::getCollidingObjects(AABB object){
    std::vector<std::string> collides;
    std::vector<BVHTreeNode *> temp;
    
    if (!(root->aabb.collide(object)))
        return collides;
    
    temp.push_back(root);
        
    while (!(temp.empty())) {
        BVHTreeNode * node = temp.back();
        temp.pop_back();

        if (node->isLeaf){
            collides.push_back(node->name);
        }
        
        else {
            if (node->leftChild->aabb.collide(object))
                temp.push_back(node->leftChild);
            
            if (node->rightChild->aabb.collide(object))
                temp.push_back(node->rightChild);
        }
    }
    
    return collides;
}
