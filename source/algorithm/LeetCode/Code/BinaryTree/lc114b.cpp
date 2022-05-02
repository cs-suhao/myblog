//给你二叉树的根结点 root ，请你将它展开为一个单链表：
//1. 展开后的单链表应该同样使用 TreeNode ，其中 right 子指针指向链表中下一个结点，而左子指针始终为 null 。
//2. 展开后的单链表应该与二叉树 先序遍历 顺序相同。

#include "TreeNode.cpp"
#include <queue>

// solution b:使用递归的方法
//在每一个节点做什么:当左右节点已经拉平，只需要将它接上
class Solution {
public:
    void flatten(TreeNode* root) {
        if(root == nullptr) return;

        flatten(root->left);
        flatten(root->right);

        TreeNode* temp = root->right;
        root->right = root->left;
        root->left = nullptr;
        TreeNode* node = root;
        while(node->right!=nullptr){
            node = node->right;
        }
        node->right = temp;
    }
};