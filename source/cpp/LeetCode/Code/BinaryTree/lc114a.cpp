//给你二叉树的根结点 root ，请你将它展开为一个单链表：
//1. 展开后的单链表应该同样使用 TreeNode ，其中 right 子指针指向链表中下一个结点，而左子指针始终为 null 。
//2. 展开后的单链表应该与二叉树 先序遍历 顺序相同。

#include "TreeNode.cpp"
#include <queue>

// solution a:使用队列先序访问然后构造
class Solution {
public:
    void traverse(TreeNode* root, std::queue<TreeNode*>& q){
        if(root==nullptr) return;
        q.push(root);
        traverse(root->left,q);
        traverse(root->right,q);
    }
    void flatten(TreeNode* root) {
        std::queue<TreeNode*> q;
        traverse(root,q);
        q.pop();
        while(!q.empty()){
            root->left = nullptr;
            root->right = q.front();
            q.pop();
            root = root->right;
        }
    }
};