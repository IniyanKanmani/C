#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "bst.h"

Node* insertNode(Node *root, int value) {

    if (root == NULL) {

        root = (Node*)malloc(sizeof(struct Node));

        if (root == NULL) {

            printf("ALLOCATION FAILED");
            exit(-1);

        }

        root->data = value;
        root->left = NULL;
        root->right = NULL;

    } else if (root->data > value) {

        root->left = insertNode(root->left, value);

    } else if (root->data < value) {

        root->right = insertNode(root->right, value);

    }

    return root;

}

Node* deleteNode(Node *root, int value) {

    if (root->data == value) {

        if (root->left != NULL && root->right != NULL) {

            struct Node *replacementNode = root->right;
            int flag = 0;

            while (flag != 1) {

                if (replacementNode->left != NULL) {

                    if ((replacementNode->left)->left == NULL && (replacementNode->left)->right != NULL) {

                        root->data = (replacementNode->left)->data;
                        root->right = deleteNode(root->right, root->data);

                        flag = 1;

                    } else {

                        replacementNode = replacementNode->left;

                    }

                } else if (replacementNode->left == NULL) {

                    root->data = replacementNode->data;
                    root->right = deleteNode(root->right, root->data);

                    flag = 1;

                }

            }

        } else if (root->left == NULL && root->right != NULL) {

            struct Node *replacementNode = root->right;
            free(root);

            return replacementNode;

        } else if (root->left != NULL && root->right == NULL) {

            struct Node *replacementNode = root->left;
            free(root);

            return replacementNode;

        } else if (root->left == NULL && root->right == NULL) {

            free(root);

            return NULL;

        }

    } else if (root->data > value) {

        root->left = deleteNode(root->left, value);

    } else if (root->data < value) {

        root->right = deleteNode(root->right, value);

    }

    return root;

}

void printSubtree(Node *N) {

    if (N != NULL) {

        printSubtree (N->left);
        printf("%d\n", N->data);
        printSubtree (N->right);

    }

}

int countNodes(Node *N) {

    if (N != NULL) {

        return (countNodes(N->left) + countNodes(N->right) + 1);

    } else if (N == NULL) {

        return 0;

    }

}

Node* freeSubtree(Node *N) {

    if (N != NULL) {

        N->left = freeSubtree(N->left);
        N->right = freeSubtree(N->right);
        free(N);

    }

    return NULL;

}

int sumSubtree(Node *N) {

    if (N != NULL) {

        return (sumSubtree(N->left) + sumSubtree(N->right) + N->data);

    } else if (N == NULL) {

        return 0;

    }

}