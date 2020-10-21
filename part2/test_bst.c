#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <unistd.h>

#include "bst.h"

Node *root=NULL;

#include "unique_rng.c"
#include "serve_client.c"

int f_verbose = 0;

void clean(){
	/*****************************************************/
	/******   Free resources before program ends  ********/
	/*****************************************************/

	root=freeSubtree(root);
	root= NULL;
	return;
}


void test_tack2(){

	printf("/******** TEST OF PART 2 ********/\n\n");
	unsigned int i;
	char *client_names[5] = {"client1_commands", "client2_commands", "client3_commands",
	                         "client4_commands", "client5_commands"};

	pthread_t threads[6];

	// spawn all threads
	pthread_create(&threads[0], NULL, (void *) ServeClient, client_names[0]);
	pthread_create(&threads[1], NULL, (void *) ServeClient, client_names[1]);
	pthread_create(&threads[2], NULL, (void *) ServeClient, client_names[2]);
	pthread_create(&threads[3], NULL, (void *) ServeClient, client_names[3]);
	pthread_create(&threads[4], NULL, (void *) ServeClient, client_names[4]);


	// join all readers
	for (i = 0; i < 5; i++) {
		pthread_join(threads[i], NULL);
	}

	// The tree should only have one node now
	int count = countNodes(root);
	int sum = sumSubtree(root);

	if (count == 1 && sum == 1){
		printf("Test for Part2 seems OK\n");
	} else{
		printf("Test for Part2 fail\n");
	}

	// Free the tree
	clean();
}

// TODO: You could add more test functions here

int main(int argc, char *argv[]){

	if(argc == 2){
	  if(strcmp(argv[1],"-v") == 0)
	    {
	      f_verbose = 1;
	    }
 	}

	test_tack2();

	// TODO: You could call your test functions at here

	return 0;
}