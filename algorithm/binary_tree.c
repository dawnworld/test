/*
 * *********************************************************************
 * binary_tree.c
 * Create 2014-05-15 10:01 By Author: qiyl <qiyl@certusnet.com.cn>
 * Distributed under terms of the CERTUSNET license.
 * *********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>

#define LOG_TAG " [Binary Tree]: "
#define BT_LOG(...)     printf(LOG_TAG __VA_ARGS__);

#define BT_ALLOC(A, B)  calloc(A, B);
#define BT_FREE(A, B)   free(A); A=NULL;

#define MAX_DEPTH   10
#define MAX_COLS    20

typedef struct _Tree_Node {
    int                data;
    int                bf;
    struct _Tree_Node *left;
    struct _Tree_Node *right;
} Tree_Node;

typedef struct _List {
    void *  node;
    struct _List * next;
} List;

static char node_array[MAX_DEPTH][MAX_COLS];
static Tree_Node* root;

static void print_tree(Tree_Node* root);
static void print_array();
static void print_offset(int offset);
/*
 * compare data, 0:left, 1:right 
 */
static int compare(int a, int b)
{
    return a < b;
}

static int calc_abs_bf(int left_bf, int right_bf)
{
    int bf;
    int abs_left_bf, abs_right_bf;

    abs_left_bf = left_bf < 0 ? 0 - left_bf : left_bf;
    abs_right_bf = right_bf < 0 ? 0 - right_bf : right_bf;

    bf = abs_left_bf - abs_right_bf;
    return bf;
}

static Tree_Node * tree_node_rotate_left(Tree_Node *node)
{
    Tree_Node *right;
    int bf, right_bf;

    right = node->right;
    
    if(NULL != right->left)
        node->right = right->left;
    else
        node->right = NULL;
    
    right->left = node;
    
    bf = node->bf;
    right_bf = right->bf;

#if 0
    if (right_bf <= 0) {
        if( right_bf > bf) right->bf = right_bf + 1;
        else right->bf = bf + 2;
        
        node->bf = bf - right_bf + 1;
//        if(bf <= -1) right->bf = bf + right_bf + 2;
    } else {
        
    }
#endif

    node->bf = bf - right_bf + 1;
    right->bf = bf + 2;

    printf(" node data:%d, bf:%d\n", node->data, node->bf);
    printf(" right data:%d, bf:%d\n", right->data, right->bf);

    if(node == root) root = right;

    return right;
}

static Tree_Node * tree_node_rotate_right(Tree_Node *node)
{
    Tree_Node *left;
    int bf, left_bf;

    left = node->left;
    
    if(NULL != left->right)
        node->left = left->right;
    else
        node->left = NULL;
    
    left->right = node;
    
    bf = node->bf;
    left_bf = left->bf;

    node->bf = left_bf - bf + 1;
    left->bf = bf - 2;
    
    printf(" node data:%d, bf:%d\n", node->data, node->bf);
    printf(" left data:%d, bf:%d\n", left->data, left->bf);

    if(node == root) root = left;

    return left;
}

static Tree_Node * tree_node_balance(Tree_Node * node)
{
    if(node->bf > 1) {
        if(node->left->bf < 0) {
            node->left = tree_node_rotate_left(node->left);
        }
        node = tree_node_rotate_right(node);
    }
    else if(node->bf < -1) {
        if(node->right->bf > 0) {
            node->right = tree_node_rotate_right(node->right);
        }
        node = tree_node_rotate_left(node);
    }
    return node;
}

static int calc_node_bf(Tree_Node ** pPath, int idx)
{
    int res;
    int inode;
    int left_bf, right_bf;
    Tree_Node *node;

    res = 0;
    inode = idx - 1;
    node = pPath[inode];
    while(1){
        Tree_Node* bparent  = pPath[--inode];
        int left_mode = (bparent && node == bparent->left);
        
        if(node->bf < -1 || node->bf > 1)
            node = tree_node_balance(node);
        if(NULL == bparent) {
            root = node;
        } else if (left_mode) {
            bparent->left = node;
        } else {
            bparent->right = node;
        }

        if(node->bf == 0 || bparent == NULL)
            break;

        if(left_mode) bparent->bf += 1;
        else bparent->bf -= 1;

        printf(" bparent data:%d, bf:%d\n", bparent->data, bparent->bf);
        printf(" node data:%d, bf:%d\n", node->data, node->bf);
        
        node = bparent;
    }

    return res;
}

static int add_to_tree(Tree_Node* root, int data)
{
    int res;
//    int depth;
    int inode;
    Tree_Node *node;
    Tree_Node* path[MAX_DEPTH];

//    memset(path, 0, sizeof(path));

    inode = 0;
    path[inode++] = NULL;
    node = root;
    while(NULL != node) {
        res = compare(data, node->data);
        /* record path then we will calc banlance factor */
        path[inode++] = node;
        if(res && node->left){
            node = node->left;
            continue;
        } else if (!res && node->right) {
            node = node->right;
            continue; 
        }

        Tree_Node * new_node = BT_ALLOC(1, sizeof(Tree_Node));
        new_node->data = data;

        if(res) {
            node->left = new_node;
            node->bf++;
        } else {
            node->right = new_node;
            node->bf--;
        }

        break;
    }
    res = calc_node_bf(path, inode);

    return MAX_DEPTH - inode;
}

static int create_bt(int *data_array, int size, int * depth)
{
    int i;
    int res;

    if(NULL == data_array || !size) return -1;

    root = BT_ALLOC(1, sizeof(Tree_Node));
    // memset(root, 0, sizeof(Tree_Node));
    root->data = data_array[0];
    *depth = 1;
    res = 0;

    for(i = 1; i < size; i++) {
        printf("## input data: %d ##\n", data_array[i]);
        res = add_to_tree(root, data_array[i]);
        *depth = res > *depth ? res : *depth;
        printf("----------------------------\n");
    }
    (*depth)++;

    return res;
}

static void print_node(Tree_Node* node, char depth)
{
    char m_depth;
    char m_cols;

    m_depth = depth;
    m_cols = node_array[m_depth][MAX_COLS - 1];

    if(NULL == node) {
        node_array[m_depth][m_cols] = '#';
        node_array[m_depth][MAX_COLS - 1] = m_cols + 1; 
        return;
    }

    node_array[m_depth][m_cols] = node->data;
    node_array[m_depth][MAX_COLS - 1] = m_cols + 1; 

    print_node(node->left, m_depth + 1);
    print_node(node->right, m_depth + 1);

}

static void print_offset(int offset)
{
    int i;
    for(i = 0; i < offset; i ++) {
        printf(" ");
    }
}

static void print_array()
{
    char m_depth;
    char m_cols;
    int  i;
    int  j;

    for(i = 0; i < MAX_DEPTH; i++) {
        m_cols = node_array[i][MAX_COLS - 1];
        if(!m_cols) continue;

        print_offset(MAX_DEPTH - i);
        
        for(j = 0; j < m_cols; j++) {
            print_offset((MAX_DEPTH - i) / 3);
            if(node_array[i][j] == '#')
                printf(" _");
            else
                printf(" %d", node_array[i][j]);
        }
        printf("\n");
    }
}

static void print_tree(Tree_Node* root)
{
    Tree_Node* node;

    // memset(node_array, 0, sizeof(node_array));

    printf(" root:%d\n", root->data);
    node = root;
    print_node(node, 0);
    print_array();
}

int main()
{
    int res;
    // int array[] ={20, 16, 3};
    // int array[] ={20,16,3,-1, 7, -2, 1, 11, 9, 26}; 
    int array[] ={20,16,3,-1,7,-2,1,11,9,26,15,14, 13, 12}; 
    int depth;

    res  = 0;

    BT_LOG("Welcome to Binary Tree world\n");
    res = create_bt(array, sizeof(array) / sizeof(int), &depth);
    BT_LOG("Tree depth is :%d\n", depth);
    print_tree(root);
    
//    print_tree(root);

    return res;
}


