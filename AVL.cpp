#include <iostream>
#include <string>
#include <vector>
using namespace std;

template <typename T>
T getValidInput(string prompt) {
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        }
        cout << "Error: Invalid input! Please enter correct data type.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

struct node {
    string filename;            
    vector<string> paths;       
    int height;                 
    node* left;
    node* right;
};

class AVL {
private:    // ==========================================
    node* root;
    node* createNode(string fname, string fullPath) {
        node* newNode     = new node;
        newNode->filename = fname;
        newNode->paths.push_back(fullPath);
        newNode->height   = 1;          
        newNode->left     = nullptr;
        newNode->right    = nullptr;
        return newNode;
    }

    int getHeight(node* n) {
        if (n == nullptr) return 0;
        return n->height;
    }

    void updateHeight(node* n) {
        n->height = 1 + max(getHeight(n->left), getHeight(n->right));
    }

    int getBalance(node* n) {
        if (n == nullptr) return 0;
        return getHeight(n->left) - getHeight(n->right);
    }

    node* rotateRight(node* y) {
        node* x  = y->left;
        node* T2 = x->right;

        x->right = y;
        y->left  = T2;

        updateHeight(y);
        updateHeight(x);
        return x;
    }

    node* rotateLeft(node* x) {
        node* y  = x->right;
        node* T2 = y->left;

        y->left  = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);
        return y;
    }

    node* avl_insert(node* avlNode, string fname, string fullPath) {
        if (avlNode == nullptr) return createNode(fname, fullPath);

        if (fname == avlNode->filename) {
            avlNode->paths.push_back(fullPath);
            return avlNode;
        }
        else if (fname < avlNode->filename)    avlNode->left  = avl_insert(avlNode->left,  fname, fullPath);
        else    avlNode->right = avl_insert(avlNode->right, fname, fullPath);

        updateHeight(avlNode);
        int balance = getBalance(avlNode);
        if (balance > 1 && fname < avlNode->left->filename)    return rotateRight(avlNode);
        if (balance < -1 && fname > avlNode->right->filename)    return rotateLeft(avlNode);

        if (balance > 1 && fname > avlNode->left->filename) {
            avlNode->left = rotateLeft(avlNode->left);
            return rotateRight(avlNode);
        }

        if (balance < -1 && fname < avlNode->right->filename) {
            avlNode->right = rotateRight(avlNode->right);
            return rotateLeft(avlNode);
        }
        return avlNode;
    }

    node* findMin(node* root) {
        node* current = root;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    node* avl_delete(node* root, string fname) {
        if (root == nullptr) return nullptr;  

        if (fname < root->filename)
            root->left  = avl_delete(root->left,  fname);
        else if (fname > root->filename)
            root->right = avl_delete(root->right, fname);

        else {
            // No Child (Leaf)
            if (root->left == nullptr && root->right == nullptr) {
                delete root;
                return nullptr;
            }
            // One Child
            else if (root->left == nullptr) {
                node* temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr) {
                node* temp = root->left;
                delete root;
                return temp;
            }
            // Two Children
            else {
                node* successor  = findMin(root->right);
                root->filename   = successor->filename;
                root->paths      = successor->paths;
                root->right      = avl_delete(root->right, successor->filename);
            }
        }

        updateHeight(root);
        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0)
            return rotateRight(root);

        // Left Right
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        // Right Right
        if (balance < -1 && getBalance(root->right) <= 0)
            return rotateLeft(root);

        // Right Left
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    void inorder(node* root) {
        if (root) {
            inorder(root->left);
            cout << root->filename << " ";
            inorder(root->right);
        }
    }

    void preorder(node* root) {
        if (root) {
            cout << root->filename << " ";
            preorder(root->left);
            preorder(root->right);
        }
    }

    void postorder(node* root) {
        if (root) {
            postorder(root->left);
            postorder(root->right);
            cout << root->filename << " ";
        }
    }

    node* copyTree(node* root) {
        if (root == nullptr) return nullptr;

        node* newNode    = createNode(root->filename, "");
        newNode->paths   = root->paths;         // copy all paths
        newNode->height  = root->height;
        newNode->left    = copyTree(root->left);
        newNode->right   = copyTree(root->right);
        return newNode;
    }

    void freeTree(node* root) {
        if (root) {
            freeTree(root->left);
            freeTree(root->right);
            delete root;
        }
    }

    node* searchNode(node* root, string fname) {
        if (root == nullptr)         return nullptr;
        if (root->filename == fname) return root;

        if (fname < root->filename)    return searchNode(root->left,  fname);
        else    return searchNode(root->right, fname);
    }

public:     // ==========================================
    

    AVL()  { root = nullptr; }      

    ~AVL() {                        
        freeTree(root);
        root = nullptr;
    }

    void insert(string fname, string fullPath) { root = avl_insert(root, fname, fullPath); }
    void remove(string fname)                  { root = avl_delete(root, fname);           }

    void inorder()   { inorder(root);   }
    void preorder()  { preorder(root);  }
    void postorder() { postorder(root); }

    
    node* search(string fname) { return searchNode(root, fname); }
    bool isEmpty() { return (root == nullptr); }
    AVL copyTree() {
        AVL newAVL;
        newAVL.root = copyTree(root);
        return newAVL;
    }

    void refresh(string rootPath);
};