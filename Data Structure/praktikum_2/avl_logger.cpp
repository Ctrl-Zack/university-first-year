#include <bits/stdc++.h>
using namespace std;

typedef struct AVLNode_t {
    int data;
    struct AVLNode_t *left, *right;
    int height;
} AVLNode;

typedef struct AVL_t {
    AVLNode *_root;
    unsigned int _size;
} AVL;

AVLNode *_avl_createNode(int value) {
    AVLNode *newNode = (AVLNode *)malloc(sizeof(AVLNode));
    newNode->data = value;
    newNode->height = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

AVLNode *_search(AVLNode *root, int value, bool mode) {
    while (root != NULL) {
        if(mode) cout << root->data << " ";
        if (value < root->data) {
            root = root->left;
        } else if (value > root->data) {
            root = root->right;
        } else {
            return root;
        }
    }
    return root;
}

int _getHeight(AVLNode *node) {
    if(node == NULL) return 0; 
    return node->height;
}

int _max(int a, int b) {
    return (a > b) ? a : b;
}

AVLNode *_rightRotate(AVLNode *pivotNode) {
    AVLNode *newParrent = pivotNode->left;
    pivotNode->left = newParrent->right;
    newParrent->right = pivotNode;

    pivotNode->height  = _max(_getHeight(pivotNode->left),  _getHeight(pivotNode->right))  + 1;
    newParrent->height = _max(_getHeight(newParrent->left), _getHeight(newParrent->right)) + 1;
    
    return newParrent;
}

AVLNode *_leftRotate(AVLNode *pivotNode) {

    AVLNode *newParrent = pivotNode->right;
    pivotNode->right = newParrent->left;
    newParrent->left = pivotNode;

    pivotNode->height  = _max(_getHeight(pivotNode->left),  _getHeight(pivotNode->right))  + 1;
    newParrent->height = _max(_getHeight(newParrent->left), _getHeight(newParrent->right)) + 1;
    
    return newParrent;
}

AVLNode *_leftCaseRotate(AVLNode *node){
    return _rightRotate(node);
}

AVLNode *_rightCaseRotate(AVLNode *node){
    return _leftRotate(node);
}

AVLNode *_leftRightCaseRotate(AVLNode *node){
    node->left = _leftRotate(node->left);
    return _rightRotate(node);
}

AVLNode *_rightLeftCaseRotate(AVLNode *node){
    node->right = _rightRotate(node->right);
    return _leftRotate(node);
}

int _getBalanceFactor(AVLNode *node){
    if(node == NULL) return 0;
    return _getHeight(node->left) - _getHeight(node->right);
}

AVLNode *_insert_AVL(AVL *avl, AVLNode *node, int value) {
    
    if(node == NULL) return _avl_createNode(value);
    if(value < node->data) node->left = _insert_AVL(avl, node->left, value);
    else if(value > node->data) node->right = _insert_AVL(avl, node->right, value);
    
    node->height = 1 + _max(_getHeight(node->left), _getHeight(node->right)); 

    int balanceFactor = _getBalanceFactor(node);
    
    if(balanceFactor > 1 && value < node->left->data) return _leftCaseRotate(node);
    if(balanceFactor > 1 && value > node->left->data) return _leftRightCaseRotate(node);
    if(balanceFactor < -1 && value > node->right->data) return _rightCaseRotate(node);
    if(balanceFactor < -1 && value < node->right->data) return _rightLeftCaseRotate(node);
    
    return node;
}

AVLNode *_findMinNode(AVLNode *node) {
    AVLNode *currNode = node;
    while (currNode && currNode->left != NULL)
        currNode = currNode->left;
    return currNode;
}

AVLNode *_remove_AVL(AVLNode *node, int value){
    if(node == NULL) return node;
    if(value > node->data) node->right = _remove_AVL(node->right, value);
    else if(value < node->data) node->left = _remove_AVL(node->left, value);
    else {
        AVLNode *temp;
        if((node->left == NULL) || (node->right == NULL)) {
            temp = NULL;
            if(node->left == NULL) temp = node->right;  
            else if(node->right == NULL) temp = node->left;
            
            if(temp == NULL) {
                temp = node;
                node = NULL;
            } else *node = *temp;   
            
            free(temp);
        } else {
            temp = _findMinNode(node->right);
            node->data = temp->data;
            node->right = _remove_AVL(node->right, temp->data);
        }
    }

	if(node == NULL) return node;
    
    node->height = _max(_getHeight(node->left), _getHeight(node->right)) + 1;

    int balanceFactor = _getBalanceFactor(node);
    
    if(balanceFactor > 1 && _getBalanceFactor(node->left) >= 0) return _leftCaseRotate(node);
    if(balanceFactor > 1 && _getBalanceFactor(node->left) < 0) return _leftRightCaseRotate(node);
    if(balanceFactor < -1 && _getBalanceFactor(node->right) <= 0) return _rightCaseRotate(node);
    if(balanceFactor < -1 && _getBalanceFactor(node->right) > 0) return _rightLeftCaseRotate(node);
    
    return node;
}

void avl_init(AVL *avl) {
    avl->_root = NULL;
    avl->_size = 0u;
}

bool avl_isEmpty(AVL *avl) {
    return avl->_root == NULL;
}

bool avl_find(AVL *avl, int value) {
    AVLNode *temp = _search(avl->_root, value, 0);
    if (temp == NULL) return false;
    
    if (temp->data == value) return true;
    else return false;
}

void avl_insert(AVL *avl, int value){
    if(!avl_find(avl, value)){
        avl->_root = _insert_AVL(avl, avl->_root, value);
        avl->_size++;
    }
}

void avl_remove(AVL *avl, int value){
    if(avl_find(avl, value)){
        avl->_root = _remove_AVL(avl->_root, value);
        avl->_size--;
    }
}

int main(){
    AVL tr;
    avl_init(&tr);
    
    int n, tmp;
    string command;
    cin >> n;
    while(n--) {
        cin >> command;
        cin >> tmp;
        if(command == "INSERT") {
            if(avl_find(&tr, tmp)) cout << tmp << "  is already in the tree\n";
            else {
                avl_insert(&tr, tmp);
                cout << tmp << " inserted\n";
            }
        } else if(command == "DELETE") {
            if(avl_find(&tr, tmp)) {
                avl_remove(&tr, tmp);
                cout << tmp << " deleted\n";
            } else {
                cout << tmp << " is not found in the tree\n";
            }
        } else if(command == "FIND") {
            if(avl_find(&tr, tmp)) {
                cout << tmp << " found with path: ";
                _search(tr._root, tmp, 1);
                cout << "\n";
            } else cout << tmp << " not found\n";
        }
    }
}