#include "RedBlackTree.h"
Node *RedBlackTree::newNode(Node *parent, int value) {
    return new Node{value, false, null, null, parent};
}

Node* RedBlackTree::minimumNode(Node *node) {
    if(node->left != null)
        return minimumNode(node->left);
    return node;
}

void RedBlackTree::rbTransplant(Node *u, Node *v) {
    if(u->parent == null)
        root = v;
    else if(u->parent->left == u)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

void RedBlackTree::rotateLeft(Node *node) {
    Node* rightNode = node->right;
    Node* rightLeftNode = rightNode->left;
    if(node == root)
        root = rightNode;
    else if(node->parent && node == node->parent->right)
        node->parent->right = rightNode;
    else if(node->parent && node == node->parent->left)
        node->parent->left = rightNode;
    rightNode->parent = node->parent;
    rightNode->left = node;
    node->right = rightLeftNode;
    node->parent = rightNode;
    rightLeftNode->parent = node;
}

void RedBlackTree::rotateRight(Node *node) {
    Node* leftNode = node->left;
    Node* leftRightNode = leftNode->right;
    if(node == root)
        root = leftNode;
    else if(node->parent && node == node->parent->right)
        node->parent->right = leftNode;
    else if(node->parent && node == node->parent->left)
        node->parent->left = leftNode;
    leftNode->parent = node->parent;
    leftNode->right = node;
    node->left = leftRightNode;
    node->parent = leftNode;
    leftRightNode->parent = node;
}

void RedBlackTree::insertFix(Node *node) {
    Node* currentNode = node;
    while(!currentNode->parent->isBlack){
        Node* gp = currentNode->parent->parent;
        if(currentNode->parent == gp->left){
            if(!gp->right->isBlack){
                gp->right->isBlack = true;
                gp->isBlack = false;
                currentNode->parent->isBlack = true;
                currentNode = gp;
            }else{
                if(currentNode == currentNode->parent->right){
                    currentNode = currentNode->parent;
                    rotateLeft(currentNode);
                }
                currentNode->parent->isBlack = true;
                gp->isBlack = false;
                rotateRight(gp);
            }
        }else{
            if(!gp->left->isBlack){
                gp->left->isBlack = true;
                gp->isBlack = false;
                currentNode->parent->isBlack = true;
                currentNode = gp;
            }else{
                if(currentNode == currentNode->parent->left){
                    currentNode = currentNode->parent;
                    rotateRight(currentNode);
                }
                currentNode->parent->isBlack = true;
                gp->isBlack = false;
                rotateLeft(gp);
            }
        }
        if(currentNode == root)
            break;
    }
    root->isBlack = true;
}

void RedBlackTree::insertHelper(Node *node, int value) {
    if(node->value > value) {
        if(node->left != null)
            insertHelper(node->left, value);
        else {
            node->left = newNode(node, value);
            insertFix(node->left);
        }
    }else{
        if(node->right != null)
            insertHelper(node->right, value);
        else {
            node->right = newNode(node, value);
            insertFix(node->right);
        }
    }
}

void RedBlackTree::insert(int value) {
    if(root == null)
        root = new Node{value, true, null, null, null};
    else
        insertHelper(root, value);
    size++;
}

void RedBlackTree::deleteFix(Node *node) {
    Node* currentNode = node;
    while(currentNode != root && currentNode->isBlack){
        Node* sibling;
        if(currentNode->parent->left == currentNode){
            sibling = currentNode->parent->right;
            if(!sibling->isBlack){
                currentNode->parent->isBlack = false;
                sibling->isBlack = true;
                rotateLeft(currentNode->parent);
                sibling = currentNode->parent->right;
            }
            if(sibling->left->isBlack && sibling->right->isBlack){
                sibling->isBlack = false;
                currentNode = currentNode->parent;
            }else{
                if(sibling->right->isBlack){
                    sibling->left->isBlack = true;
                    sibling->isBlack = false;
                    rotateRight(sibling);
                    sibling = currentNode->parent->right;
                }
                sibling->isBlack = currentNode->parent->isBlack;
                sibling->right->isBlack = true;
                rotateLeft(currentNode->parent);
                currentNode = root;
            }
        }else{
            sibling = currentNode->parent->left;
            if(!sibling->isBlack){
                currentNode->parent->isBlack = false;
                sibling->isBlack = true;
                rotateRight(currentNode->parent);
                sibling = currentNode->parent->left;
            }
            if(sibling->left->isBlack && sibling->right->isBlack){
                sibling->isBlack = false;
                currentNode = currentNode->parent;
            }else{
                if(sibling->left->isBlack){
                    sibling->right->isBlack = true;
                    sibling->isBlack = false;
                    rotateLeft(sibling);
                    sibling = currentNode->parent->left;
                }
                sibling->isBlack = currentNode->parent->isBlack;
                sibling->left->isBlack = true;
                rotateRight(currentNode->parent);
                currentNode = root;
            }
        }
    }
    currentNode->isBlack = true;
}

void RedBlackTree::deleteNode(Node* node) {
    bool isDeletedBlack = node->isBlack;
    Node *replacementNode;
    if(node->left == null) {
        replacementNode = node->right;
        rbTransplant(node, replacementNode);
    }else if(node->right == null) {
        replacementNode = node->left;
        rbTransplant(node, replacementNode);
    }else{
        replacementNode = minimumNode(node->right);
        node->value = replacementNode->value;
        isDeletedBlack = node->isBlack = replacementNode->isBlack;
        rbTransplant(replacementNode, replacementNode->right);
    }
    if (isDeletedBlack)
        deleteFix(replacementNode);
    size--;
    delete node;
}

Node* RedBlackTree::searchHelper(Node *node, int value) {
    if(node == null || node->value == value)
        return node;
    else if(node->value > value)
        return searchHelper(node->left, value);
    else
        return searchHelper(node->right, value);
}

Node* RedBlackTree::search(int value) {
    return searchHelper(root, value);
}

void RedBlackTree::printTreeHelper(Node *node, string indent, bool isRight) {
    if(node != null){
        cout << indent;
        if(isRight){
            cout << "R----";
            indent += "   ";
        }else{
            cout << "L----";
            indent += "|  ";
        }
        cout << node->value << " (" << (node->isBlack ? "Black" : "Red") << ")\n";
        printTreeHelper(node->left, indent, false);
        printTreeHelper(node->right, indent, true);
    }
}

void RedBlackTree::printTree() {
    cout << "Size: " << size << '\n';
    if(root != null)
        printTreeHelper(root, "", true);
    else
        cout << "Tree is empty\n";
}

int RedBlackTree::treeHeight() {
    return nodeHeight(root) + 1;
}

int RedBlackTree::nodeHeight(Node *node) {
    if(node == null) return -1;
    return max(nodeHeight(node->right), nodeHeight(node->left)) + 1;
}

int RedBlackTree::nodeDepth(Node *node) {
    if(node == null) return -1;
    return nodeDepth(node->parent) + 1;
}

Node* RedBlackTree::nullNode() {
    return null;
}