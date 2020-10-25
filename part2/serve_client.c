pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

void* ServeClient(char *client) {
    
	pthread_rwlock_wrlock(&lock);
	
	FILE *fp = fopen(client, "r");
	char ch = ' ';

	if (fp == NULL) {

		printf("CANNOT OPEN FILE: %s\n", client);
		exit(-1);

	}

	ch = getc(fp);

	while (ch != EOF) {

		char stringChar[11] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'};
		char stringNum[6] = {' ', ' ', ' ', ' ', ' ', '\0'};
		int i = 0;
		int j = 0;
		int num = 0;

		while (ch != '\n') {
			
			while (ch != ' ' && ch != '\n') {

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

			sscanf(stringNum, "%d", &num);
			root = insertNode(root, num);
			printf("[%s]insertNode %d\n", client, num);

		} else if (strcmp(stringChar, "deleteNode") == 0) {

			sscanf(stringNum, "%d", &num);
			root = deleteNode(root, num);
			printf("[%s]deleteNode %d\n", client, num);

		} else if (strcmp(stringChar, "countNodes") == 0) {

			num = countNodes(root);
			printf("[%s]countNodes = %d\n", client, num);

		} else if (strcmp(stringChar, "sumSubtree") == 0) {

			num = sumSubtree(root);
			printf("[%s]sumSubtree = %d\n", client, num);

		}

		ch = getc(fp);
		
	}

	fclose(fp);

	pthread_rwlock_unlock(&lock);

	return NULL;

}