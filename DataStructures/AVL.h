//
//  AVL.h
//  AdvancedDSA
//
//  Created by Dheeraj Vagavolu on 29/10/23.
//

#ifndef AVL_h
#define AVL_h

namespace avl {

template<class T>
struct Node {
    struct Node<T>* lc;
    struct Node<T>* rc;
    T data;
    int h;
    Node(T val, int height): data(val), h(height), lc(nullptr), rc(nullptr) {}
};

template<class T>
class AVL {
    Node<T>* root;
public:
    AVL(): root(nullptr) {}
    
//    Rotations Start
    
    Node<T>* LL_Rotation(Node<T>* p){
        Node<T>* pl = p->lc;
        Node<T>* plr = p->lc->rc;
        
        pl->rc = p;
        p->lc=plr;
        
        p->h = _height(p);
        pl->h = _height(pl);
        
        if(root == p){
            root = pl;
        }
        return root;
    }
    
    Node<T>* RR_Rotation(Node<T>* p){
        Node<T>* pr = p->rc;
        Node<T>* prl = p->rc->lc;
        
        pr->lc = p;
        p->rc = prl;
        
        p->h = _height(p);
        pr->h = _height(pr);
        
        if(root == p){
            root = pr;
        }
        return root;
    }
    
    Node<T>* LR_Rotation(Node<T>* p){
        Node<T>* pl = p->lc;
        Node<T>* plr = pl->rc;
        p->lc = plr->lc;
        pl->rc = plr->rc;
        
        plr->lc = pl;
        plr->rc = p;
        
        p->h = _height(p);
        pl->h = _height(pl);
        plr->h = _height(plr);
        
        if(root == p){
            root = plr;
        }
        
        return root;
    }
    
    Node<T>* RL_Rotation(Node<T>* p){
        Node<T>* pr = p->rc;
        Node<T>* prl = pr->lc;
        
        p->rc = prl->rc;
        pr->lc = prl->lc;
        
        prl->lc = p;
        prl->rc = pr;
        
        p->h = _height(p);
        pr->h = _height(pr);
        prl->h = _height(prl);
        
        if(root == p){
            root = prl;
        }
        
        return root;
    }
    
    
    
    
//    Rotations End
    
    int _height(Node<T>* n){
        if(!n){
            return 0;
        }
        int hl = (n->lc) ? n->lc->h:0;
        int hr = (n->rc) ? n->rc->h:0;
        return 1 + std::max(hl, hr);
    }
    
    int balanceFactor(Node<T>* n){
        if(!n){
            return 0;
        }
        return _height(n->lc) - _height(n->rc);
    }
    
    void insert(T val) {
        if(!root){
            root = new Node<T>(val, 1);
            return;
        }
        root = insert(root, val);
    }
    
    Node<T>* insert(Node<T>* t, T val){
        if(!t){
            return new Node<T>(val, 1);
        }
        
        if(val < t->data){
            t->lc = insert(t->lc, val);
        }else if(val > t->data){
            t->rc = insert(t->rc, val);
        }else{
            return t;
        }
        
        t->h = _height(t);
        
        if(balanceFactor(t) == 2 && balanceFactor(t->lc)==1){
            // LL rotation
            return LL_Rotation(t);
        }
        
        
        if(balanceFactor(t) == 2 && balanceFactor(t->lc)==-1){
            // LR rotation
            return LR_Rotation(t);
        }
//        
        if(balanceFactor(t) == -2 && balanceFactor(t->rc)==-1){
            // RR rotation
            return RR_Rotation(t);
        }
//        
        if(balanceFactor(t) == -2 && balanceFactor(t->rc)==1){
            // LL rotation
            return RL_Rotation(t);
        }
        
        return t;
    }
    
    void printTree() {
        _printTree(root);
    }
    
    void _printTree(Node<T>* n, std::string indent = "") {
        if(n){
            std::cout << indent << n->data << " (h:" << n->h << ")" << std::endl;
            _printTree(n->lc, indent + "  ");
            _printTree(n->rc, indent + "  ");
        }
    }
        
    
};


void testLL(){
    
    std::cout << std::string(40, '-') << "\n";
    std::cout << "Testing LL Rotations >> (20, 10, 5)" << std::endl;
    std::cout << std::string(40, '-') << "\n";
    
    AVL tree = AVL<int>();
    tree.insert(20);
    tree.insert(10);
    tree.insert(5);
    tree.printTree();
    std::cout << std::string(40, '-') << "\n\n";
}

void testRR(){
    
    std::cout << std::string(40, '-') << "\n";
    std::cout << "Testing RR Rotations >> (20, 30, 40)" << std::endl;
    std::cout << std::string(40, '-') << "\n";
    
    AVL tree = AVL<int>();
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.printTree();
    std::cout << std::string(40, '-') << "\n\n";
}

void testLR(){
    
    std::cout << std::string(40, '-') << "\n";
    std::cout << "Testing LR Rotations >> (20, 10, 15)" << std::endl;
    std::cout << std::string(40, '-') << "\n";
    
    AVL tree = AVL<int>();
    tree.insert(20);
    tree.insert(10);
    tree.insert(15);
    tree.printTree();
    std::cout << std::string(40, '-') << "\n\n";
}

void testRL(){
    
    std::cout << std::string(40, '-') << "\n";
    std::cout << "Testing RL Rotations >> (20, 30, 25)" << std::endl;
    std::cout << std::string(40, '-') << "\n";
    
    AVL tree = AVL<int>();
    tree.insert(20);
    tree.insert(30);
    tree.insert(25);
    tree.printTree();
    std::cout << std::string(40, '-') << "\n\n";
}


void runTests(){
    testLL();
    testRR();
    testLR();
    testRL();
}

}


#endif /* AVL_h */
