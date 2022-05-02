//给你一棵二叉树的根节点 root ，翻转这棵二叉树，并返回其根节点。
#include "TreeNode.cpp"

// 后序遍历
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if(root == nullptr) return root;
        TreeNode* left = invertTree(root->left);
        TreeNode* right = invertTree(root->right);
        root->left = right;
        root->right = left;
        return root;
    }
};