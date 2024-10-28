#include<stdio.h>
#include "../allocator/allocator.h"
typedef struct TreeNode{
    char val;
    struct TreeNode*left;
    struct TreeNode*right;
}TreeNode_t;
void tree_gen(TreeNode_t * root,size_t height){
    root->val=(char)('a'+height);
    if (height>0){
        root->left=(TreeNode_t*)mid_alloc(sizeof(TreeNode_t));
        root->right=(TreeNode_t*)mid_alloc(sizeof(TreeNode_t));
        tree_gen(root->left,height-1);
        tree_gen(root->right,height-1);
    }
    else{
        root->left=NULL;
        root->right=NULL;
    }
    return;
}
bool tree_test(){
    TreeNode_t * tree_root=(TreeNode_t*)mid_alloc(sizeof(TreeNode_t));
    tree_gen(tree_root,3);
    TreeNode_t * left_most_node=tree_root;
    while(true)
    {
        if(left_most_node->left)
        left_most_node=left_most_node->left;
        else
        break;
    }
    TreeNode_t * right_most_node=tree_root;
    while(true)
    {
        if(right_most_node->right)
        right_most_node=right_most_node->right;
        else 
        break;
    }
    if(right_most_node->val!='a' && left_most_node->val!='a')
    return false;
    if(tree_root->val!='d')
    return false;
    if(tree_root->left->val!='c')
    return false;
    if(tree_root->right->left->val!='b')
    return false;
    return true;
}