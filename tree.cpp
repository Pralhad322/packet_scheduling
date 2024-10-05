#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

#define MAX_QUEUE_SIZE 2

enum NodeType { LEAF, INTERNAL };

// Forward declarations
struct TreeNode1;
struct TreeNode2;
struct TreeNode3;
struct TreeNode4;

// for leaf
struct TreeNode1 {
    bool isFull;
    TreeNode2* parent;
    std::queue<int> (*correspondingRank)[8];

    TreeNode1(NodeType nodeType) : parent(nullptr), correspondingRank(nullptr) {
        if (nodeType == LEAF) {
            isFull = false;
        }
    }
    void setCorrespondingRank(std::queue<int> (*rankNode)[8]) {
        correspondingRank = rankNode;
    }
    void setValueInQueue(int x, int y, int val) {
        if (correspondingRank != nullptr && x < 8) {
            (*correspondingRank)[x].push(val);
            if ((*correspondingRank)[x].size() >= MAX_QUEUE_SIZE) {
                isFull = true;
                propagateFullStatus();
            }
        }
    }
    void propagateFullStatus() {
        if (parent != nullptr) {
            parent->updateFullStatus();
        }
    }
};

// for one level up from leaf
struct TreeNode2 {
    bool rightFull;
    bool leftFull;
    TreeNode3* parent;
    TreeNode1* leftChild;
    TreeNode1* rightChild;
    TreeNode2(NodeType nodeType) : parent(nullptr), leftChild(nullptr), rightChild(nullptr) {
        if (nodeType == LEAF) {
            rightFull = false;
            leftFull = false;
        }
    }
    void setLeftChild(TreeNode1* child) {
        leftChild = child;
        leftFull = (child != nullptr) ? child->isFull : false;
    }
    void setRightChild(TreeNode1* child) {
        rightChild = child;
        rightFull = (child != nullptr) ? child->isFull : false;
    }
    TreeNode1* getLeftChild() const { return leftChild; }
    TreeNode1* getRightChild() const { return rightChild; }
    void updateFullStatus() {
        leftFull = (leftChild != nullptr) ? leftChild->isFull : false;
        rightFull = (rightChild != nullptr) ? rightChild->isFull : false;
        if (parent != nullptr) {
            parent->updateFullStatus();
        }
    }
};

// for two levels up from leaf
struct TreeNode3 {
    bool leftrightFull;
    bool leftleftFull;
    bool rightleftFull;
    bool rightrightFull;
    TreeNode4* parent;
    TreeNode2* leftChild;
    TreeNode2* rightChild;

    TreeNode3(NodeType nodeType) : parent(nullptr), leftChild(nullptr), rightChild(nullptr) {
        if (nodeType == LEAF) {
            leftrightFull = false;
            leftleftFull = false;
            rightleftFull = false;
            rightrightFull = false;
        }
    }

    void setLeftChild(TreeNode2* child) {
        leftChild = child;
        leftleftFull = (child != nullptr) ? child->leftFull : false;
        leftrightFull = (child != nullptr) ? child->rightFull : false;
    }

    void setRightChild(TreeNode2* child) {
        rightChild = child;
        rightrightFull = (child != nullptr) ? child->rightFull : false;
        rightleftFull = (child != nullptr) ? child->leftFull : false;
    }
    TreeNode2* getLeftChild() const { return leftChild; }
    TreeNode2* getRightChild() const { return rightChild; }
    void updateFullStatus() {
        leftleftFull = (leftChild != nullptr) ? leftChild->leftFull : false;
        leftrightFull = (leftChild != nullptr) ? leftChild->rightFull : false;
        rightleftFull = (rightChild != nullptr) ? rightChild->leftFull : false;
        rightrightFull = (rightChild != nullptr) ? rightChild->rightFull : false;
        if (parent != nullptr) {
            parent->updateFullStatus();
        }
    }
};

// for three levels up from leaf
struct TreeNode4 {
    bool leftleftleftFull;
    bool leftleftrightFull;
    bool leftrightleftFull;
    bool leftrightrightFull;
    bool rightrightrightFull;
    bool rightrightleftFull;
    bool rightleftleftFull;
    bool rightleftrightFull;
    TreeNode4* parent;
    TreeNode3* leftChild;
    TreeNode3* rightChild;
    
    TreeNode4(NodeType nodeType) : parent(nullptr), leftChild(nullptr), rightChild(nullptr) {
        if (nodeType == LEAF) {
            leftleftleftFull = false;
            leftleftrightFull = false;
            leftrightleftFull = false;
            leftrightrightFull = false;
            rightrightrightFull = false;
            rightrightleftFull = false;
            rightleftleftFull = false;
            rightleftrightFull = false;
        }
    }

    void setLeftChild(TreeNode3* child) {
        leftChild = child;
        leftleftleftFull = (child != nullptr) ? child->leftleftFull : false;
        leftleftrightFull = (child != nullptr) ? child->leftrightFull : false;
        leftrightleftFull = (child != nullptr) ? child->rightleftFull : false;
        leftrightrightFull = (child != nullptr) ? child->rightrightFull : false;
    }

    void setRightChild(TreeNode3* child) {
        rightChild = child;
        rightleftleftFull = (child != nullptr) ? child->leftleftFull : false;
        rightleftrightFull = (child != nullptr) ? child->leftrightFull : false;
        rightrightleftFull = (child != nullptr) ? child->rightleftFull : false;
        rightrightrightFull = (child != nullptr) ? child->rightrightFull : false;
    }
    TreeNode3* getLeftChild() const { return leftChild; }
    TreeNode3* getRightChild() const { return rightChild; }
    void updateFullStatus() {
        leftleftleftFull = (leftChild != nullptr) ? leftChild->leftleftFull : false;
        leftleftrightFull = (leftChild != nullptr) ? leftChild->leftrightFull : false;
        leftrightleftFull = (leftChild != nullptr) ? leftChild->rightleftFull : false;
        leftrightrightFull = (leftChild != nullptr) ? leftChild->rightrightFull : false;
        rightleftleftFull = (rightChild != nullptr) ? rightChild->leftleftFull : false;
        rightleftrightFull = (rightChild != nullptr) ? rightChild->leftrightFull : false;
        rightrightleftFull = (rightChild != nullptr) ? rightChild->rightleftFull : false;
        rightrightrightFull = (rightChild != nullptr) ? rightChild->rightrightFull : false;
    }
};

void traverseAndInsert(int val, TreeNode4* root, std::queue<int>& q) {
    TreeNode3* node3 = nullptr;
    TreeNode2* node2 = nullptr;
    TreeNode1* leaf = nullptr;

    // Traverse to the appropriate TreeNode3
    if (val < 4) {
        node3 = root->getLeftChild();
    } else {
        node3 = root->getRightChild();
        val -= 4;
    }

    // Traverse to the appropriate TreeNode2
    if (val < 2) {
        node2 = node3->getLeftChild();
    } else {
        node2 = node3->getRightChild();
        val -= 2;
    }

    // Traverse to the appropriate TreeNode1 (leaf)
    if (val == 0) {
        leaf = node2->getLeftChild();
    } else {
        leaf = node2->getRightChild();
    }

    // Check if the leaf node is not full and insert into the queue
    if (!leaf->isFull) {
        q.push(val);
        leaf->setValueInQueue(0, 0, val);
    } else {
        // If the leaf node is full, traverse up to the parent node
        TreeNode2* parent2 = leaf->parent;
        TreeNode3* parent3 = parent2->parent;
        TreeNode4* parent4 = parent3->parent;
        // Handle the case when the leaf node is full
        std::cerr << "Leaf node is full, cannot insert value: " << val << std::endl;
        return;
    }
}

void insert(int val, std::queue<int> (&q1)[8][4], std::queue<int> (&q2)[8][2], TreeNode4* root) {
    // Traverse the tree and insert into the appropriate queue
    traverseAndInsert(val, root, q1[0][0]); // Example insertion, adjust as needed
    traverseAndInsert(val, root, q2[0][0]); // Example insertion, adjust as needed
}

int main() {
    // Create a hash table to store the address of the corresponding rank of the second queue
    std::unordered_map<int, std::queue<int>*> rankToAddressMap;
    std::queue<int> queue2[8][2]; // Initialize with empty queues
    std::queue<int> queue1[8][4]; // Initialize with empty queues

    // Populate the hash table with the addresses of the corresponding ranks
    for (int rank = 0; rank < 8; ++rank) {
        rankToAddressMap[rank] = &queue2[rank][0];
    }

    // Create leaf nodes
    std::vector<TreeNode1*> leafNodes;
    for (int i = 0; i < 8; ++i) {
        leafNodes.push_back(new TreeNode1(LEAF));
    }

    // Create TreeNode2 nodes and set their children to the leaf nodes
    std::vector<TreeNode2*> node2Nodes;
    for (int i = 0; i < 4; ++i) {
        TreeNode2* node = new TreeNode2(LEAF);
        node->setLeftChild(leafNodes[2 * i]);
        node->setRightChild(leafNodes[2 * i + 1]);
        node2Nodes.push_back(node);
    }

    // Create TreeNode3 nodes and set their children to the TreeNode2 nodes
    std::vector<TreeNode3*> node3Nodes;
    for (int i = 0; i < 2; ++i) {
        TreeNode3* node = new TreeNode3(LEAF);
        node->setLeftChild(node2Nodes[2 * i]);
        node->setRightChild(node2Nodes[2 * i + 1]);
        node3Nodes.push_back(node);
    }

    // Create TreeNode4 nodes and set their children to the TreeNode3 nodes
    TreeNode4* root = new TreeNode4(LEAF);
    root->setLeftChild(node3Nodes[0]);
    root->setRightChild(node3Nodes[1]);

    // Insert into the actual queue it could be of first level or it could be of second level
    insert(1, queue1, queue2, root);

    return 0;
}

// abhi tak tree a architecture and queue and all this are done
// task left to from insertion 
<<<<<<< HEAD
=======
// this code is giving the error
>>>>>>> e74cb53 (reinitialzing repo)
