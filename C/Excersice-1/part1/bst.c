#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "bst.h"

int sum, count = 0;
// Your source code
Node* insertNode(Node *root, int value) {


    if (root == NULL) {

        root = (Node*)malloc(sizeof(struct Node));
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

    struct Node *replacementNode = NULL;
    struct Node *temp = NULL;

    if (root->data == value) {

        if (root->left != NULL && root->right != NULL) {

            replacementNode = root->right;

            while(temp == NULL) {

                if (replacementNode->left != NULL) {

                    replacementNode = replacementNode->left; 

                } else if (replacementNode->left == NULL) {

                    temp = replacementNode;
                    temp->left = root->left;
                    free(root);
                    root = temp;

                }

            }

        } else if (root->left == NULL && root->right != NULL) {

            replacementNode = root->right;
            free(root);
            return replacementNode;

        } else if (root->left != NULL && root->right == NULL) {

            replacementNode = root->left;
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

        countNodes(N->left);
        count = count + 1;
        countNodes(N->right);

    }

    return count;

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

        sumSubtree(N->left);
        sum = sum + N->data;;
        sumSubtree(N->right);

    }

    return sum;

}

void main() {

	Node *root=NULL;
	
	root=insertNode(root, 14);
	root=insertNode(root, 12);
	root=insertNode(root, 6);
	root=insertNode(root, 17);
	root=insertNode(root, 8);
	printSubtree(root);
	printf("\n");
	printf("%d\n\n", sumSubtree(root));
	root=deleteNode(root,14);
	root=deleteNode(root,8);
	printSubtree(root);
	root=freeSubtree(root);
	printf("Number of nodes=%d\n", countNodes(root));

}