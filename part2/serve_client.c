pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;


void* ServeClient(char *client){

	// TODO: Open the client command file and read commands line by line

	// TODO: match and execute commands

	// TODO: Handle command: "insertNode <some unsigned int value>"
	// print: "[ClientName]insertNode <SomeNumber>\n"
	// e.g. "[client1_commands]insertNode 1\n"

	// TODO: Handle command: "deleteNode <some unsigned int value>"
	// print: "[ClientName]deleteNode <SomeNumber>\n"
	// e.g. "[client1_commands]deleteNode 1\n"

	// TODO: Handle command: "countNodes"
	// print: "[ClientName]countNodes = <SomeNumber>\n"
	// e.g. "[client1_commands]countNodes 1\n"


	// TODO: Handle command: "sumSubtree"
	// print: "[ClientName]sumSubtree = <SomeNumber>\n"
	// e.g. "[client1_commands]sumSubtree 1\n"
    
	FILE *fp = fopen(client, "r");
	char ch = ' ';

	//if (fp == NULL) {
	//	printf("The File is Empty");
	//	exit(-1);
	//}

	pthread_rwlock_rdlock(&lock);

	ch = getc(fp);
	while (ch != EOF) {

		char stringChar[11] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'};
		char stringNum[6] = {' ', ' ', ' ', ' ', ' ', '\0'};
		int i = 0;
		int j = 0;

		while (ch != '\n')
		{
			
			while (ch != ' ') {

				stringChar[i] = ch;
				ch = getc(fp);
				i++;

			}

			if (ch == ' ') {

				ch = getc(fp);

			}

			while (ch != '\n') {

				stringNum[j] = ch;
				ch = getc(fp);
				j++;

			}

		}

		if (strcmp(stringChar, "insertNode") == 0) {

			int num = 0;
			sscanf(stringNum, "%d", &num);
			root = insertNode(root, num);
			printf("[%s]insertNode %s\n", client, stringNum);

		} else if (strcmp(stringChar, "deleteNode") == 0) {

			int num = 0;
			sscanf(stringNum, "%d", &num);
			root = deleteNode(root, num);
			printf("[%s]deleteNode %s\n", client, stringNum);

		} else if (strcmp(stringChar, "countNodes") == 0) {

			printf("[%s]countNodes = 0\n", client);

		} else if (strcmp(stringChar, "sumSubtree") == 0) {

			printf("[%s]sumSubtree = 0\n", client);

		}

		ch = getc(fp);
		
	}

	pthread_rwlock_unlock(&lock);

	fclose(fp);

	return NULL;

}