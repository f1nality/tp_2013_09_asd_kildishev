#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stack>
struct CNode {
    int Key;
    int Height;
    int Number;
    CNode* Left;
    CNode* Right;

    CNode(int key) : Key(key), Height(1), Left(0), Right(0), Number(1) {}
};

int Height(CNode* node)
{
    return node == 0 ? 0 : node->Height;
}

int BalanceFactor(CNode* node)
{
    return Height(node->Right) - Height(node->Left);
}

void FixHeight(CNode* node)
{
    int hleft = Height(node->Left);
    int hright = Height(node->Right);
    node->Height = ((hleft > hright) ? hleft : hright) + 1;
}
void FixNumber(CNode * p) {
    p->Number = ((p->Right == 0) ? 0 : p->Right->Number) + ((p->Left == 0) ? 0 : p->Left->Number) + 1;
}
CNode* RotateRight(CNode* p)
{
    CNode* pR = p->Left;
    p->Left = pR->Right;
    pR->Right = p;
    //pR->Number = p->Number;
    //p->Number = pR->Number - ((pR->Right == 0) ? 0 : pR->Right->Number) - 1;
    FixNumber(p);
    FixNumber(pR);
    FixHeight(p);
    FixHeight(pR);
    return pR;
}

CNode* RotateLeft(CNode* p)
{
    CNode* pL = p->Right;
    p->Right = pL->Left;
    pL->Left = p;
    //pL->Number = p->Number;
    //p->Number = pL->Number - ((pL->Left == 0) ? 0 : pL->Left->Number) - 1;
    FixNumber(p);
    FixNumber(pL);
    FixHeight(p);
    FixHeight(pL);
    return pL;
}

CNode* Balance(CNode* p)
{
    FixHeight(p);
    if( BalanceFactor(p) == 2 )
    {
        if( BalanceFactor(p->Right) < 0 )
            p->Right = RotateRight(p->Right);
        return RotateLeft(p);
    }
    if( BalanceFactor(p) == -2 )
    {
        if( BalanceFactor(p->Left) > 0  )
            p->Left = RotateLeft(p->Left);
        return RotateRight(p);
    }
    return p;
}

CNode* Insert(CNode* p, int key, int &pos)
{
    if( p == 0 ) {
        return new CNode(key);
    }
    ++(p->Number);
    if( key < p->Key ) {
        pos += ((p->Right == 0) ? 0 : p->Right->Number) + 1;
        p->Left = Insert(p->Left, key, pos);
    }
    else{
        p->Right = Insert(p->Right, key, pos);
    }
    return Balance(p);
}

CNode* FindMin(CNode* p)
{
    return p->Left == 0 ? p : FindMin(p->Left);
}

CNode* RemoveMin(CNode* p)
{
    if( p->Left == 0 )
        return p->Right;
    p->Left = RemoveMin(p->Left);
    --p->Number;
    return Balance(p);
}

CNode* Remove(CNode* p, int position)
{
    if( p == 0 ) return 0;
    if (position >= p->Number) return p;
    int sum = 0;
    std::stack<CNode*> nodes;

    while (true) {
        int rightNumber = (p->Right == 0) ? 0 : p->Right->Number;
        
        if (position - sum > rightNumber) {
            nodes.push(p);
            p = p->Left;
            sum += rightNumber + 1;
        } else if (position - sum < rightNumber) {
            if (p->Right != 0) {
                nodes.push(p);
                p = p->Right; 
            }
            else break;
        } else {
            CNode* left = p->Left;
            CNode* right = p->Right;
            int key = p->Key;
            delete p;
            if(right == 0) {
                if (left == 0) {
                    if (!nodes.empty()) {
                        p = nodes.top();
                        nodes.pop();
                        if (p->Key > key) {
                            p->Left = 0;
                        } else {
                            p->Right = 0;
                        }
                        --p->Number;
                    }
                    else return 0;
                }
                else
                    p = left;
            }
            else {
                CNode* min = FindMin(right);
                min->Right = RemoveMin(right);
                min->Left = left;
                FixNumber(min);
                p = Balance(min);
            } 
            break;
        }
    }
    CNode *p1;
    while (!nodes.empty()) {
        p1 = nodes.top();
        --p1->Number;
        if (p1->Key > p->Key) {
            p1->Left = p;
        } else {
            p1->Right = p;
        }
        p = Balance(p1);
        nodes.pop();
    }
    return p;
}

void Delete(CNode* node)
{
    if (!node) return;
    Delete(node->Left);
    Delete(node->Right);
    delete node;
}
void printtree(CNode *p) {
    if (p == 0) return;
    printtree(p->Left);
    std::cout << p->Key << ": " << p->Number << std::endl;
    printtree(p->Right);

}
int main()
{
    CNode* root = 0;
    int n;
    std::cin >> n;
    std::vector<int> result;
    int key;
    int position;
    int operation;
    for (int i = 0; i < n; ++i) {
        std::cin >> operation;
        if (operation == 1) {
            position = 0;
            std::cin >> key;
            root = Insert(root, key, position);
            result.push_back(position);
            //std::cout << position << std::endl;
        }
        else if (operation == 2) {
            std::cin >> key;
            root = Remove(root, key);
        }
        else if (operation == 3) {
            printtree(root);
        }
    }
    for (int i = 0; i < result.size(); ++i) {
        std::cout << result[i] << std::endl;
    }
    Delete(root);


    return 0;
}